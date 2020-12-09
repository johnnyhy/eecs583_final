#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int goodFunc1() { printf("%d\n", 1); return 0; }
int goodFunc2() { printf("%d\n", 2); return 0; }
int goodFunc3() { printf("%d\n", 3); return 0; }
int badFunc() { return 1; }

void foo(int (*fptr)()) {
    fptr();
}

int main(int argc, char** argv) {
    char buf[8];
    int (*fptr)() = nullptr;

    srand(time(NULL));
    int random = rand() % 3 + 1;

    switch (random) {
    case 1: fptr = &goodFunc1; break;
    case 2: fptr = &goodFunc2; break;
    case 3: fptr = &goodFunc3; break;
    }

    strcpy(buf, argv[1]);

    foo(fptr);

    return 0;
}
