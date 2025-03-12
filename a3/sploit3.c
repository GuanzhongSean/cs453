#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define FLAG_SIZE 1024

int main() {
	char flag[FLAG_SIZE] = {0};
	int fd = open("./flag", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	read(fd, flag, FLAG_SIZE);
	printf("%s", flag);
	return 0;
}
