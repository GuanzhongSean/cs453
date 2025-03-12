#include <fcntl.h>
#include <stdio.h>
#include <sys/uio.h>
#include <unistd.h>

#define FLAG_SIZE 1024

int main() {
	struct iovec iov[1];
	char flag[FLAG_SIZE];
	int fd = open("./flag", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	ssize_t len = read(fd, flag, FLAG_SIZE);
	iov[0].iov_base = flag;
	iov[0].iov_len = len;
	writev(1, iov, 1);
	return 0;
}
