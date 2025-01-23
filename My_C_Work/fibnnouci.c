#include <stdio.h>

int fib(int n);

int main() {

    int term1 = 0;
    int term2 = 1;
    int fibn = 0;
    int sequence_length = 0;

    printf("This program will calculate the fibonacci sequence up to a certain length in two ways\n");
   
    do {
    printf("Enter the sequence length your looking for:\n");
    scanf("%d", &sequence_length); 

    if (sequence_length < 3) {
        printf("The number you entered is too small\n");
        printf("Please enter a number greater than or equal to 3\n");}
     }  while(sequence_length < 3);

    printf("Iterative solution:\n");
    printf("%d,%d,", term1,term2);

    for (int i = 2; i < sequence_length; i++) {

        fibn = term1 + term2;
        printf("%d", fibn);
        term1 = term2;
        term2 = fibn;

        if (i < sequence_length-1) printf(","); 
    }

    printf("\nRecursive solution:\n");

    for (int i = 0; i < sequence_length; i++) {

        printf("%d",fib(i));

    if (i < sequence_length-1) printf(","); 

    }
    return 0;

}

    int fib(int n) {

    if (n > 1) return fib(n-1) + fib(n-2);
    else if ( n == 1) return 1;
    else if ( n == 0) return 0;
    else printf ("invalid entry\n");  // this soution is more demanding computatoin wise 
}


