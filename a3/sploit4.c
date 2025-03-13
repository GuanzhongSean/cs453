#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FLAG_SIZE 1024
#define X32 0x40000000

int main(void) {
	char flag[FLAG_SIZE] = {0};
	int fd = open("./flag", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	ssize_t n = read(fd, flag, FLAG_SIZE);
	long w = syscall(X32 | SYS_write, 1, flag, n);
	if (w < 0) {
		perror("x32 write");
		return 1;
	}

	return 0;
}
