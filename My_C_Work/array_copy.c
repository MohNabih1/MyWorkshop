#include <stdlib.h>
#include <stdio.h>

int* arr_cpy( int* arr, int length);


int main () {

    int a1[] = { 0, 1, 2, 3};

    int* a1Cpy = arr_cpy(a1, 4);

for (int i = 0; i < 4; i++) { 

    printf("a1Cpy[%d] = %d \n",i, a1Cpy[i]); 
}

    return 0;

}

int* arr_cpy( int* arr, int length) {

    // create memory for the array on the heap
    int* cpy =(int*)malloc(length * sizeof(int)); 

    for (int i = 0; i < length; i++) {
        cpy[i] = arr[i];
    }
   
    return cpy;
    free(cpy);

}