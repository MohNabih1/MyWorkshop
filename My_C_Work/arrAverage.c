#include <stdio.h>

double findArrAverage( double array[], int length );

int main(void) {

    double arr[] ={ 10.3,57.4,25.8,12.3,24.7};

    int arr_len = sizeof(arr)/sizeof(double);

    printf(" The average of the elements of arr is %.2lf\n", findArrAverage(arr, arr_len));

return 0;
}

double findArrAverage( double array[], int length ) {
    double average;
    double sum = 0;
     for (int i = 0; i < length; i++) {
        sum += array[i]; 
        average = sum/length;
    }
    return average;

}
