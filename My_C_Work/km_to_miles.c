#include <stdio.h>

double km_to_mi(double x); 

int main() {
    printf("This program is going to take a km value and and convert to miles.\n");

    double startval, endval;
    int step = 1;
    do {
        printf("Enter the starting value in km\n");
        scanf("%lf", &startval);
        if (startval < 0) printf("Invalid entry. Enter a number >= 0.\n");
    } while (startval < 0 );

    do {
        printf("Enter the ending value in km\n");
        scanf("%lf", &endval);
        if (endval < 0) printf("Invalid entry. Enter a number >= 0.\n");
    } while (endval < 0); 

    printf(" KM        Miles\n");
    printf("----------------\n");
    
    for ( startval; startval <= endval; startval += step) {
        double result = km_to_mi(startval);
        printf("%.2lf       %.2lf \n", startval, result);
    }
    
    return 0; 
}

double km_to_mi(double x) {

    x *= 0.621;
    return x;
}