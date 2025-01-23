#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int word_counter (char *str, char *word);

int main (void) {

    char mystr[] = "hello my name name name is mohamad" ;
    char w[] = "name";

    int result = word_counter (mystr, w);

    printf ("%d\n", result);
    return 0;
}


int word_counter (char *str, char *word) {

    int strlength = strlen(str);
    int wordlength = strlen(word);
    int endlength = strlength - wordlength + 1;
    int count = 0;
   

    for (int i = 0; i < endlength; i++) {
         bool word_found = true;
        for (int j = 0; j < wordlength; j++) {
            if ( word[j] != str[j+i])  {
                word_found = false;
                break; 
            }
        }
        if (word_found) count++;
        
    } return count;
}
