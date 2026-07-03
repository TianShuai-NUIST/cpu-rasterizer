#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(int argn, char ** argv) {
    char chr;
    const char *str = argv[1];
    unsigned int index = 0;
    do {
	   chr = str[index];
	   index ++;
    } while(chr != '\0');
    printf("%d %d",index, strlen(argv[1]));
}
