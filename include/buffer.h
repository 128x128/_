#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include "bits.h"

typedef struct buffer {
    void* data;
    dtype* type;
    size_t size;
    size_t used;
    size_t bytes;
} buffer;


void bhexdump(buffer* _) {hexdump(_->data, _->bytes);}

buffer* balloc(dtype* _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    b->data = malloc(_->size);
    b->type = _; b->bytes = _->size;
    b->used = 0; b->size = 0;
    return b;
}

void bdelloc(buffer* _) {if (_->data) free(_->data); free(_);}

void brelloc(buffer* _) {
    void* __ = malloc(_->bytes*2);
    memcpy(__, _->data, _->bytes);
    _->bytes *= 2; free(_->data); 
    _->data = __;
}

void bpush(buffer* _, void* __) {
    if (_->used == _->bytes) brelloc(_);
    memcpy(_->data+_->used, __, _->type->size);
    _->size++; _->used += _->type->size;
}

void bpop(buffer* _) {_->size--; _->used -= _->type->size;}

buffer* salloc(const char* _) {
   buffer* b = balloc(&String);
   while(*_) {bpush(b, (void*)_++);}
   return b;
}


void printb(buffer* _) {
    printf("Buffer: dtype=%s ",_->type->str); 
    printf("using [%zu bytes out of %zu bytes]\n",_->used,_->bytes); 
    for (int i=0; i<_->size;i++) {
	printf("\t%p", _->data); 
	hexdump(_->data+(i*_->type->size), _->type->size);
    }
}

#endif
