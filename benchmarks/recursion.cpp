#include <stdio.h>

int COUNT = 0;

void recurse(int input) {
    for (int i = 0; i < input; i++) {
        COUNT += 1;
        recurse(i);
    }
}

int main() {
    recurse(20);
    printf("count: %d\n", COUNT);
}