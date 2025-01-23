#include <stdio.h> 
#include <string.h> 
#include <math.h>

int binary_to_decimial(char *str);

int main() {

    char number[] = "00010010";
    char number2[] = "10011111";

    int decimal = binary_to_decimial(number);
    int decimal2 = binary_to_decimial(number2);

    printf("10101 is equivalent to %d\n", decimal);
    printf("10011 is equivalent to %d\n", decimal2);

    return 0;
}

int binary_to_decimial(char *str) {

    int length = strlen(str);
    int sum = 0;
    
    for (int i = 0; i < length; i++) {
        int asciival = (int)str[i];
        int result;

        if (asciival == 49) asciival = 1;
        else if (asciival == 48) asciival = 0;

        result = asciival * pow(2,length - i - 1);
        sum += result;
    }
    return sum;
}
