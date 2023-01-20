#ifndef RECOGNIZER_H 
#define RECOGNIZER_H

#include "common.h"

#define MAX_REGEX 128

typedef struct rexp {
    char data[MAX_REGEX];
    regex_t regex;
} rexp;

rexp* recognize(const char* _) {
    rexp* r = (rexp*)malloc(sizeof(rexp));
    regcomp(&r->regex, _, 0);
    strcpy(&r->data[0], _);
    return r;
}

int match(rexp* x, const char* y) {
    return !regexec(&x->regex, y, 0, NULL, 0);
}


#endif
