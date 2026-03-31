// basics.c

#include "basics.h"
#include <stdlib.h>
#include <string.h>

char* add_string(char* a, char* b);

char* add_string(char* a, char* b){
    /*Concatenate two strings, the returning value is stocked in the pile*/
    int na = strlen(a);
    int nb = strlen(b);
    char *result = malloc((na + nb + 1)*sizeof(char));
    for (int i = 0; i < na; i++){       // Concatenates the first string
        result[i] = a[i];
    }
    for (int i = na; i < na + nb; i++){ // Concatenates the second string
        result[i] = b[i-na];
    }
    return result;
}