#include <stddef.h>
#include <sys/types.h>

/* external interfaces */
ssize_t read(int fd, void *buffer, size_t count);
int puts(const char *buffer);

/* exposed interfaces */
inline __attribute__((always_inline)) ssize_t in(void *buffer, size_t count) {
	return read(0, buffer, count);
}

inline __attribute__((always_inline)) int out(const char *buffer) {
	return puts(buffer);
}

void abort(void);

void safe_function() { /* Do nothing */ }

#define MAX_INPUT_SIZE 1024

int main(void) {
	char input[MAX_INPUT_SIZE] = {0};
	int len = in(input, MAX_INPUT_SIZE);

	void (*crash_func)() = safe_function;
	char sentinal[] = "1AQ@#$!@#$#@DA{>_+#$KF<EP{P./{#!P(})}}cx,_a";
	int cur = 0;

	if (len > cur && input[cur] == sentinal[0]) {
		cur++;
		crash_func = abort;
	}
	for (int i = 0; i < 41; i++) {
		if (len > cur && input[cur] == sentinal[i + 1]) {
			cur++;
		} else {
			cur = 0;
		}
	}
	if (len > cur && input[cur] == sentinal[42]) {
		crash_func();
	}

	out(input);

	return 0;
}
