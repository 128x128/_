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
    r->buffer = strbuffer(_);
    return r;
}

void rexp_delloc(rexp* _) {
    bdelloc(_->buffer); free(_);
}

rexp* regex_union(rexp* x, rexp* y) {
    char uregex[MAX_REGEX];
    strcat(&uregex[0], (char*)x->buffer->data);
    strcat(&uregex[0], "|");
    strcat(&uregex[0], (char*)y->buffer->data);
    return recognize(&uregex[0]);
}

rexp* regex_cat(rexp* x, rexp* y) {
    char uregex[MAX_REGEX];
    strcat(&uregex[0], (char*)x->buffer->data);
    strcat(&uregex[0], (char*)y->buffer->data);
    return recognize(&uregex[0]);
}

rexp* regex_closure(rexp* x) {
    char uregex[MAX_REGEX];
    strcat(&uregex[0], "*");
    return recognize(&uregex[0]);
}

int match(rexp* x, const char* y) {
    return !regexec(&x->regex, y, 0, NULL, 0);
}

#endif
