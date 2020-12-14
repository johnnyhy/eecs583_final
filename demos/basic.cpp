#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void goodFunc1() { printf("%d\n", 1); return; }
void goodFunc2() { printf("%d\n", 2); return; }
void goodFunc3() { printf("%d\n", 3); return; }
void badFunc() { printf("chaos!!!"); return; }

int main(int argc, char** argv) {
    char buf[2];
    void (*fptr)() = nullptr;

    srand(time(NULL));
    int random = rand() % 3 + 1;

    switch (random) {
    case 1: fptr = &goodFunc1; break;
    case 2: fptr = &goodFunc2; break;
    case 3: fptr = &goodFunc3; break;
    }

    memcpy(buf, argv[1], 16);

    (*fptr)();
    return 0;
}
