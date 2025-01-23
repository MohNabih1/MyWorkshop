#include <stdio.h>
#include <string.h>

int main() {
   
   char str [] = "what lllll dglk ; ;;l  the hell is that";
   char letter = 'l';
   int length = strlen(str);
 
 for (int i = 0 ; i < length  ; i++) {
    if (str[i] == letter) str[i] = '\0';
 }

 for (int i = 0 ; i < length ; i++) {
    if (str[i] != '\0') printf("%c", str[i]);
 }
   
    return 0;
}