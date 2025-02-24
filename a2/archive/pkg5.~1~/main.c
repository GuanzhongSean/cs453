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

#define MAX_INPUT_SIZE 10

int main(void) {
  char input[MAX_INPUT_SIZE] = {0};
  int len = in(input, MAX_INPUT_SIZE);

  void (*crash_func)() = safe_function;

  char c1 = 'A', c2 = 'B', c3 = 'C', c4 = 'D', c5 = 'E';
  if (len == 10 && input[0] == c1 && input[1] == c2 && input[2] == c3 &&
      input[3] == c4 && input[4] == c5) {
    crash_func = abort;
    c1 = '\x05';
    c2 = '\x00';
    c3 = '\x7F';
    c4 = '\x1A';
    c5 = '\x0E';
  }

  if (len == 10 && input[5] == c1 && input[6] == c2 && input[7] == c3 &&
      input[8] == c4 && input[9] == c5) {
    crash_func();
  }

  out(input);
  return 0;
}
