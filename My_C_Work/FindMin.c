#include <stdlib.h>
#include <stdio.h>

int main ()  {

    int arr1[] = {4,6,3,7,8,34,2};
    int arr2[] = {1,6,7,23,67,23};

    int min1 = FindMin(arr1,7);
    int min2 = FindMin(arr2,6);

    printf("Min 1: %d\n", min1);
    printf("Min 2: %d\n", min2);

    return 0;
}

int FindMin (int arr[], int length) {

    int min = arr[0];

    for (int i = 1; i < length ; i++) {

        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

