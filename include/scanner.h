#ifndef SCANNER_H 
#define SCANNER_H

#include "list.h"
#include "node.h"

typedef struct scanner {
    int start;
    int current;
    int line;
} scanner;

typedef struct regex {
    buffer* buffer;
} regex;



#endif
