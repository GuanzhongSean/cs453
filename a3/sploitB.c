#define _GNU_SOURCE
#include <fcntl.h>
#include <liburing.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FLAG_SIZE 1024

int main() {
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

	struct io_uring ring;
	struct io_uring_sqe *sqe;
	struct io_uring_cqe *cqe;
	if (io_uring_queue_init(8, &ring, 0) < 0) {
		perror("io_uring_queue_init");
		return 1;
	}

	// Get a submission queue entry (SQE)
	sqe = io_uring_get_sqe(&ring);
	if (!sqe) {
		fprintf(stderr, "Failed to get SQE\n");
		io_uring_queue_exit(&ring);
		return 1;
	}

	// Prepare the write operation
	io_uring_prep_write(sqe, STDOUT_FILENO, flag, strlen(flag), 0);

	// Submit the request
	io_uring_submit(&ring);

	// Wait for completion
	if (io_uring_wait_cqe(&ring, &cqe) < 0) {
		fprintf(stderr, "Failed to wait for CQE\n");
		io_uring_queue_exit(&ring);
		return 1;
	}

	// Check result
	if (cqe->res < 0) {
		fprintf(stderr, "Write failed: %s\n", strerror(-cqe->res));
	}

	// Mark completion as done
	io_uring_cqe_seen(&ring, cqe);
	io_uring_queue_exit(&ring);

	return 0;
}
