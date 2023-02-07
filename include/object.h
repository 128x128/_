#ifndef OBJECT_H 
#define OBJECT_H

#include "common.h"
#include "buffer.h"
#include "recognizer.h"

typedef struct object {
    void* self;
    type type;
} object;

object* initObject(void* self) {
    object* o = (object*)malloc(sizeof(object));
    o->self = 0;
    return o;
}
object* initStringObj(const char* str) {
    object* o = initObject(initStringBuffer(str));
    o->type = STRING;
    return o;
}
object* initRegexObj(const char* str) {
    object* o = initObject(initRegex(str));
    o->type = REGEX;
    return o;
}


#endif
