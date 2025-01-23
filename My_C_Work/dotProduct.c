#include <stdio.h>


int dot_product (int vec1[], int vec2[], int length);

int main() {

    int v1[] = {5,8,7};
    int v2[] = {3,7,9};

    int dot = dot_product (v1,v2,3);

    printf("the dot product is %d\n", dot);

return 0;
}

int dot_product (int vec1[], int vec2[],int length) {
    int result = 0;
    for ( int i = 0; i < length; i++) {
        
       result += vec1[i] * vec2[i];
        
    }
    return result;
}