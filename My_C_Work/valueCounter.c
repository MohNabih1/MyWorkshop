#include <stdio.h>

int main() {
    
    int arr1[] = { 4,2,3,4,5,6,3,6,5,4};
    int arr2[] = { 3,4,3,3,3,5,7,8,8,1,1,1,1,1};

    int to_find1 = 4;
    int to_find2 = 1;

    int result1 = find_occurrences(arr1,10, to_find1);
    int result2 = find_occurrences(arr2,14, to_find2);

    printf ("number of 4's found in arr1: %d\n", result1);
    printf ("number of 1's found in arr2: %d\n",result2);

    return 0;
}

int find_occurrences (int arr[], int arr_size, int num) {

    int counter = 0;

    for (int i = 0; i < arr_size; i++) {
        
        if (arr[i] == num) counter++;
    }
     return counter;
}