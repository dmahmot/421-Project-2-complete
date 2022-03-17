#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


int main(){

    char* str = (char*)malloc(3*sizeof(char));

    char *a= "a";
    char *b= "b";
    char *c= "c";
    char *d= "d";

    printf("%s\n", a);

    strncat(str, a, 1);

    printf("%s\n", str);

    strncat(str, b, 1);

    printf("%s\n", str);

    strncat(str, c, 1);

    printf("%s\n", str);

    strncat(str, d, 1);

    printf("%s\n", str);

    free(str);

    return 0;
}