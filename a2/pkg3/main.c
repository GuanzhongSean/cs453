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

#define MAX_INPUT_SIZE 5

int main(void) {
  char input[MAX_INPUT_SIZE] = {0};
  int len = in(input, MAX_INPUT_SIZE);

  int a = 0;
  int *buffer[5] = {&a, &a, &a, &a, NULL};
  char sentinel = 'A';
  int cur = 0;
  for (int i = 0; i < len; i++) {
    if (input[i] == sentinel) {
      *buffer[cur++] = sentinel;
      sentinel += cur * (i + 1) * 2;
    }
  }
  out(input);

  return 0;
}
