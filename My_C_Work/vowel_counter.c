#include <stdio.h>
#include <string.h>
#include <ctype.h>

int VowelCounter(char string[]);

int main () {

    char str[] = "Hello, My name is Mohamad Alkateeb";

        printf("Vowel count in the string is %d",VowelCounter(str));

    return 0;
}

int VowelCounter(char string[]) {
    
     int count = 0;

    for (int i = 0; i <= strlen(string); i++) {

        switch (tolower(string[i])) {
            case 'i':
            case 'o':
            case 'u':
            case 'a':
            case 'e':
            count++;
        }
    }
    return count;
}