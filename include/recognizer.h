#ifndef RECOGNIZER_H 
#define RECOGNIZER_H

#include "common.h"
#include "node.h"

#define MAX_REGEX 4096

typedef struct rexp {
    buffer* buffer;
    regex_t regex;
} rexp;

rexp* recognize(const char* _) {
    rexp* r = (rexp*)malloc(sizeof(rexp));
    if (regcomp(&r->regex, _, REG_EXTENDED)) exit(1);
    r->buffer = string(_);
    return r;
}

void rexp_delloc(rexp* _) {
    bdelloc(_->buffer); free(_);
}


int match(rexp* x, const char* y) {
    return !regexec(&x->regex, y, 0, NULL, 0);
}

#endif
