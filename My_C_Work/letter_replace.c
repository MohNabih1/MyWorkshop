#include <stdio.h>
#include <string.h>

void letterSwap (char* a , char letter, char new);

int main () {

    char str[] = "hekko, workd!";
    char let = 'k';
    char newlet = 'l';
    
    letterSwap (str,let, newlet);

    printf ("%s\n",str);

    return 0;

}

void letterSwap (char* a , char letter, char new) {
 
    int len = strlen(a);
   
    for (int i = 0; i < len; i++) {
        if (a[i] == letter)   a[i] = new;        
}
}