#include <stdlib.h>
#include <stdbool.h>

bool is_leap_year(int year);

int main() {
    int year;
   /* printf("Enter the year you want to check for leap\n");
    scanf ("%d", &year);
    if (is_leap_year(year)) {
        printf("year %d is leap", year);
        }
        else printf("year %d is not leap", year);*/

        for (int y = 1950; y <= 2023; y++) {

            if (is_leap_year(y)) {
                printf(" %d LEAP YEAR\n", y);
            }
                else printf (" %d\n", y);
        }
    

    return 0;

}

bool is_leap_year(int year) {

    if (year % 4 != 0) return false;
    if (year % 100 != 0) return true;
    if (year % 400 != 0) return false;

    else return true;
}