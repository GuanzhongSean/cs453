#include "../shellcode.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void *aligned_addr = (void *)((size_t)shellcode & ~(pagesize - 1));

  // Change memory protection to executable
  if (mprotect(aligned_addr, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) !=
      0) {
    perror("mprotect");
    return 1;
  }

  ((void (*)())shellcode)();

  return 0;
}
