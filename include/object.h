#ifndef OBJECT_H 
#define OBJECT_H

#include "common.h"
#include "buffer.h"

typedef struct object {
    void* self;
    dtype type;
} object;

object* initObject(void* self) {
    object* o = (object*)malloc(sizeof(object));
    o->self = 0;
    return o;
}



#endif
