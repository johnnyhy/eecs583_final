#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void goodFunc1() { printf("%d\n", 1); return; }
void goodFunc2() { printf("%d\n", 2); return; }
void goodFunc3() { printf("%d\n", 3); return; }
void badFunc() { return; }

int main(int argc, char** argv) {
    char buf[8];
    void (*fptr)() = nullptr;

    srand(time(NULL));
    int random = rand() % 3 + 1;

    switch (random) {
    case 1: fptr = &goodFunc1; break;
    case 2: fptr = &goodFunc2; break;
    case 3: fptr = &goodFunc3; break;
    }

    strcpy(buf, argv[1]);

    (*fptr)();
    return 0;
}
