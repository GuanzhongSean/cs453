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

  char c1 = 'A', c2 = 'B';
  int i = 4;
  if (len >= 2 && input[0] == c1 && input[1] == c2) {
    crash_func = abort;
    c1 = 'y';
    c2 = 'z';
    i = 999;
  }

  if (len == i && input[2] == c1 && input[3] == c2) {
    crash_func();
  }

  out(input);
  return 0;
}
