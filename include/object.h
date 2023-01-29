#ifndef OBJECT_H 
#define OBJECT_H

#include "common.h"
#include "list.h"

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

void pushVar(object* o) {
}
void pushMethod(object* o) {
}

#endif
