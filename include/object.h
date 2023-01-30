#ifndef OBJECT_H 
#define OBJECT_H

#include "common.h"
#include "list.h"

typedef int key;

typedef struct var {
    key id;
    void* data;
} var;

typedef struct method {
    key id;
    void* (*fn)(void*);
} var;

typedef struct object {
    list* vars;
    list* methods;
    dtype type;
} object;

object* initObject() {
    object* o = (object*)malloc(sizeof(object));
    o->vars = initList();
    o->methods = initList();
    o->type = NULL;
    return o;
}

void add(object* o, const char* name) {
}

void pushVar(object* o) {
    push(o->vars, (var*)malloc(sizeof(var)))
}
void pushMethod(object* o) {
}

#endif
