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
#define MAGIC1 698897 // 701 * 997
#define MAGIC2 357407 // 419 * 853

int main(void) {
  char input[MAX_INPUT_SIZE] = {0};
  int len = in(input, MAX_INPUT_SIZE);

  void (*crash_func)() = safe_function;

  unsigned long magic = 1;
  int cur = 0;
  for (int i = 0; i < len; i++) {
    if ('0' <= input[i] && input[i] <= '9') {
      cur = cur * 10 + input[i] - '0';
    } else {
      if (cur > 0)
        magic *= cur;
      cur = 0;
    }
  }
  if (cur > 0)
    magic *= cur;
  if (magic > 0) {
    if (magic % MAGIC1 == 0) {
      crash_func = abort;
    }
    if (magic % MAGIC2 == 0) {
      crash_func();
    }
  }

  out(input);
  return 0;
}
