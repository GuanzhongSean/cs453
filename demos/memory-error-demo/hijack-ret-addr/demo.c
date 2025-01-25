#include <stdio.h>
#include <unistd.h>

int vuln() {
    char buf[80];
    int r;
    r = read(0, buf, 400);
    printf("\nRead %d bytes\n", r);
    return 0;
}

int main(int argc, char *argv[]) {
    printf("Try to exec /bin/ls");
    vuln();
    return 0;
}
