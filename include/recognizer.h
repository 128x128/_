#ifndef RECOGNIZER_H 
#define RECOGNIZER_H

#include "common.h"

typedef struct regex {
    buffer* buffer;
    regex_t regex;
} regex;

regex* initRegex(const char* _) {
    regex* r = (regex*)malloc(sizeof(regex));
    if (regcomp(&r->regex, _, REG_EXTENDED)) exit(1);
    r->buffer = initStringBuffer(_);
    return r;
}
void dellocRegex(regex* _) {
    dellocBuffer(_->buffer); free(_);
}
int match(regex* x, const char* y) {
    return !regexec(&x->regex, y, 0, NULL, 0);
}

#endif
