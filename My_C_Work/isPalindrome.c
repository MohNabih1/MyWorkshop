#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool isPalindrome (char str[]);

int main() {

    char string1[] = "abccba";
    char string2[] = "abczgws";

    if (isPalindrome(string1)) 
         printf(" string 1 is  palindrome");
    else printf(" string 1 is not a palindrome");

    printf("\n");

     if  (isPalindrome(string2))  
          printf(" string 2 is palindrome");
     else printf(" string 2 is not a palindrome");
     
    return 0;
}
   bool isPalindrome (char str[])
 {
bool isPalindrome(char str[]) {
    int length = strlen(str) / 2;
    for (int i = 0; i < length; i++) {
        if (str[i] != str[strlen(str) - i - 1]) {
            return false;
        }
    }
    return true;
}

} 


