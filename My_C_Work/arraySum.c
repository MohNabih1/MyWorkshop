#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int arr1[] = {10,20,30,40,50};
    int arr2[] = {3,2,6,2,7,3,8,2,2,7};

    printf("The sum of arr1 is %d\n", arraySum(arr1, (sizeof(arr1)/sizeof(int))));
    printf("The sum of arr2 is %d\n", arraySum(arr2, (sizeof(arr2)/sizeof(int))));
    
    return 0;
}

int arraySum(int array[], int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {

        sum = sum + array[i]; }

        return sum;
}