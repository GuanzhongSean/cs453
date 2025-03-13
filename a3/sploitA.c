#define _GNU_SOURCE
#include <fcntl.h>
#include <linux/io_uring.h>
#include <linux/openat2.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FLAG_SIZE 1024

int main(void) {
	struct io_uring_params params;
	memset(&params, 0, sizeof(params));

	/* 1) Set up the ring with a single SQ/CQ entry. */
	int ring_fd = syscall(SYS_io_uring_setup, 1, &params);
	if (ring_fd < 0) {
		perror("io_uring_setup");
		return 1;
	}

	/* 2) mmap the SQ ring. */
	size_t sq_ring_sz =
		params.sq_off.array + (params.sq_entries * sizeof(unsigned));
	void *sq_ring =
		mmap(NULL, sq_ring_sz, PROT_READ | PROT_WRITE,
			 MAP_SHARED | MAP_POPULATE, ring_fd, IORING_OFF_SQ_RING);
	if (sq_ring == MAP_FAILED) {
		perror("mmap SQ ring");
		close(ring_fd);
		return 1;
	}

	/* 3) mmap the SQEs (submission queue entries). */
	size_t sqes_sz = params.sq_entries * sizeof(struct io_uring_sqe);
	struct io_uring_sqe *sqes =
		mmap(NULL, sqes_sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE,
			 ring_fd, IORING_OFF_SQES);
	if (sqes == MAP_FAILED) {
		perror("mmap SQEs");
		munmap(sq_ring, sq_ring_sz);
		close(ring_fd);
		return 1;
	}

	/* 4) mmap the CQ ring. */
	size_t cq_ring_sz =
		params.cq_off.cqes + (params.cq_entries * sizeof(struct io_uring_cqe));
	void *cq_ring =
		mmap(NULL, cq_ring_sz, PROT_READ | PROT_WRITE,
			 MAP_SHARED | MAP_POPULATE, ring_fd, IORING_OFF_CQ_RING);
	if (cq_ring == MAP_FAILED) {
		perror("mmap CQ ring");
		munmap(sq_ring, sq_ring_sz);
		munmap(sqes, sqes_sz);
		close(ring_fd);
		return 1;
	}

	/* Fetch pointers into the SQ and CQ rings based on offsets in params. */
	unsigned *sq_head = (unsigned *)((char *)sq_ring + params.sq_off.head);
	unsigned *sq_tail = (unsigned *)((char *)sq_ring + params.sq_off.tail);
	unsigned *sq_ring_mask =
		(unsigned *)((char *)sq_ring + params.sq_off.ring_mask);
	unsigned *sq_array = (unsigned *)((char *)sq_ring + params.sq_off.array);

	unsigned *cq_head = (unsigned *)((char *)cq_ring + params.cq_off.head);
	unsigned *cq_tail = (unsigned *)((char *)cq_ring + params.cq_off.tail);
	unsigned *cq_ring_mask =
		(unsigned *)((char *)cq_ring + params.cq_off.ring_mask);
	struct io_uring_cqe *cqe_array =
		(struct io_uring_cqe *)((char *)cq_ring + params.cq_off.cqes);

	/* 5) Prepare our buffer. */
	char flag[FLAG_SIZE] = {0};
	struct open_how how;
	memset(&how, 0, sizeof(how));
	how.flags = O_RDWR | O_CREAT;
	how.mode = 0644;
	int fd = syscall(SYS_openat2, AT_FDCWD, "./flag", &how, sizeof(how));
	if (fd < 0) {
		fprintf(stderr, "Failed to open flag: %m\n");
		return 1;
	}
	read(fd, flag, FLAG_SIZE);
	close(fd);

	/* 6) Get the current submission queue tail and find the SQE index. */
	unsigned tail = *sq_tail;
	unsigned index = tail & *sq_ring_mask;
	struct io_uring_sqe *sqe = &sqes[index];

	/* 7) Fill out the SQE for a write to STDOUT. */
	memset(sqe, 0, sizeof(*sqe));
	sqe->opcode = IORING_OP_WRITE;	 /* Write operation */
	sqe->fd = STDOUT_FILENO;		 /* to fd=1 (stdout) */
	sqe->off = 0;					 /* offset not used for stdout */
	sqe->addr = (unsigned long)flag; /* address of buffer */
	sqe->len = sizeof(flag) - 1;	 /* number of bytes to write */
	sqe->user_data = 42;			 /* arbitrary ID to track the request */

	/* 8) Place this SQE index in the submission queue array. */
	sq_array[index] = index;

	/* 9) Advance the SQ tail pointer to submit the entry. */
	*sq_tail = tail + 1;

	/* 10) Submit via io_uring_enter: tell kernel we have 1 SQE, want 1 CQE. */
	int ret = syscall(SYS_io_uring_enter, ring_fd, 1, 1, IORING_ENTER_GETEVENTS,
					  NULL, 0);
	if (ret < 0) {
		perror("io_uring_enter");
		munmap(sq_ring, sq_ring_sz);
		munmap(sqes, sqes_sz);
		munmap(cq_ring, cq_ring_sz);
		close(ring_fd);
		return 1;
	}

	/* 11) Wait for the completion (spin here for simplicity). */
	for (;;) {
		unsigned head_val = *cq_head;
		if (head_val != *cq_tail) {
			/* There's a completion in the queue. */
			struct io_uring_cqe cqe = cqe_array[head_val & *cq_ring_mask];

			if (cqe.user_data == 42) { /* Our request? */
				if ((int)cqe.res < 0) {
					fprintf(stderr, "Write failed: %s\n", strerror(-cqe.res));
				}
				/* Mark this CQE as seen by advancing the head. */
				*cq_head = head_val + 1;
				break;
			} else {
				/* Not our request? Advance and keep looping. */
				*cq_head = head_val + 1;
			}
		}
	}

	/* 12) Cleanup. */
	munmap(sq_ring, sq_ring_sz);
	munmap(sqes, sqes_sz);
	munmap(cq_ring, cq_ring_sz);
	close(ring_fd);

	return 0;
}
