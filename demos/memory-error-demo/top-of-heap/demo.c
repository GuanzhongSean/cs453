#include <stdlib.h>
#include <stdio.h>

int main(void) {
    void *a = malloc(9);

    malloc(1);
    malloc(0);

    malloc(24);
    malloc(25);

    free(a);

    void *b = malloc(9);

    strcpy(b, "cs453");
    printf("hello %s\n", a);
}
