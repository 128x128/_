#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "bits.h"

typedef struct buffer {
    void* data;
    type type;
    size_t size;
    size_t used;
    size_t bytes;
} buffer;

// alloc buffer
buffer* balloc(type _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    b->data = malloc(typemap(_)->size);
    b->type = _; b->bytes = typemap(_)->size;
    b->used = 0; b->size = 0;
    return b;
}

void bdelloc(buffer* _) {
    if (_->data) free(_->data); free(_);
}

void brelloc(buffer* _) {
    void* __ = malloc(_->bytes*2);
    memcpy(__, _->data, _->bytes);
    _->bytes *= 2; free(_->data); 
    _->data = __;
}

// init



// buffer ops

size_t tsize(buffer* _) {
    return typemap(_->type)->size;
}
void* end(buffer* _) {
    return _->data+_->used;
}
void* offset(buffer* _, int i) {
    return _->data + (i * tsize(_));
}
void bpush(buffer* _, void* __) {
    if (_->used == _->bytes) brelloc(_);
    memcpy(_->data+_->used, __, tsize(_));
    _->size++; _->used += tsize(_);
}

void bpop(buffer* _) {
    memset(end(_)-tsize(_), 0x00, tsize(_));
    _->size--; _->used -= tsize(_);
}

// string buffer ops
buffer* string(const char* _) {
   buffer* b = balloc(CHAR);
   while(*_) {bpush(b, (void*)_++);}
   return b;
}

void concat(buffer* x, buffer* y) {
    char* c = (char*)y->data;
    while(*c) {
	bpush(x, (void*)c++);
    }
}

// buffer printers
void bhexdump(buffer* _) {
    hexdump(_->data, _->bytes);
}

void printb(buffer* _) {
    printf("\033[93mBuffer: type=%s ",typemap(_->type)->str); 
    printf("\033[91m[%zu bytes out of %zu bytes]\033[0m\n",_->used,_->bytes);
    for (int i=0; i<_->used;i+=tsize(_)) {
	printf("\t"); 
	hexdump(_->data+i, tsize(_));
    }
}


//struct buffer_t {
    //buffer* ();
//}

#endif
