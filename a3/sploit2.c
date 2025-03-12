#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FLAG_SIZE 1024

int main() {
	char flag[FLAG_SIZE];

	// Use syscall(SYS_open) instead of open() which wraps syscall(SYS_openat)
	int fd = syscall(SYS_open, "./flag", O_RDONLY, 0);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	ssize_t len = read(fd, flag, FLAG_SIZE);
	write(1, flag, len);
	close(fd);

	return 0;
}
