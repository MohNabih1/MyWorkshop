#include <string.h>
#include <stdio.h>
#include <ctype.h>

void letterFlip (char* string);

int main() {
    char str[] = "mY NAME IS mOHAMAD nABIH";
    letterFlip(str);

    printf("%s\n", str);
    

}

void letterFlip (char* string) {

 for (int i = 0; i < strlen(string); i++) {
    if (islower(string[i])) {
    string[i] = toupper(string[i]);
    }
    else if(isupper(string[i])) {
    string[i] = tolower(string[i]);
    }
    }
    return 0;
}