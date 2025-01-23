#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));

    int dice_number = 20;
    int roll = 0;

    for (int i = 1; i <= dice_number; i++) {
        roll = rand() % 6 + 1;
        printf("Rolling dice #%d: --> %d\n", i, roll);
    }
    return 0;
}