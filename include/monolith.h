#ifndef MONOLITH 
#define MONOLITH

#include "common.h"

typedef struct monolith {
} monolith;

int runFile(const char* file) {
    FILE *fp = fopen(file, "r");
    char c;
    while(c != EOF) { 
	c = fgetc(fp);
	printf("%c\n", c);
    }
    return fclose(fp);
}
int prompt() {
    char buffer[4096];
    while (1) {printf("> "); scanf("%s", &buffer[0]);}
    return 0;
}



#endif
