#ifndef SCANNER_H 
#define SCANNER_H

#include "list.h"

typedef struct scanner {
    int start;
    int current;
    int line;
} scanner;

list* scanTokens(const char* src) {
    return NULL;
}


#endif
