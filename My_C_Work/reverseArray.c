#include <stdio.h>

int main() {

    int arr1[] = {1,2,3,4,5,6,7,8,9};
    int arr_length1 = sizeof(arr1)/sizeof(int);

    int arr2[] = {10,11,12,13,14,15,16,17};
    int arr_length2 = sizeof(arr2)/sizeof(int);

     reverse_arr(arr1, arr_length1);
     reverse_arr(arr2, arr_length2);

      for (int i = 0; i < arr_length1; i++) {

        printf("arr[%d]= %d\n", i, arr1[i]);
         }
         
         printf("\n");

         for (int i = 0; i < arr_length2; i++) { 
            printf("arr[%d]= %d\n", i, arr2[i]);
         }

    
    return 0;
}


void reverse_arr(int array[], int length) {
for (int i = 0; i < (length/2); i++) {
        int temp = 0;

        temp = array[i];
        array[i] = array[length-1-i];
        array[length-1-i] = temp; 
    }
   
}


/*int arr[] = { 1,2,3,4,5,6,7,8,9}; // my Code 

int arr_length = 9;


for (int i = 0; i < arr_length; i++) {

    arr[i] = arr[arr_length-i];

    printf(" arr[%d] = %d\n", i, arr_length-i); 
}*/