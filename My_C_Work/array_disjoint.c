#include <stdio.h>

int main() {

    int ar1[] = {1,5,3,6};
    int ar2[] = {2,7,8,9};

    for (int i = 0; i <sizeof(ar1); i++) {
        for(int j = 0; j < sizeof(ar2); j++) {
            if (ar1[i] == ar2[j]) {
                printf(" the arrays have a joint;\n");
                break;
            }
          
        }
  else { 
                printf("The two arrays are disjointed.\n");
            }
        
        }



return 0;
}