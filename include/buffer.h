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
    b->data = malloc(_->size);
    b->type = _; b->bytes = _->size;
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

// buffer ops
void* bidx(buffer* _, int i) {
    return _->data + (i * typemap(_->type)->size);
}

void bpush(buffer* _, void* __) {
    if (_->used == _->bytes) brelloc(_);
    memcpy(_->data+_->used, __, _->type->size);
    _->size++; _->used += _->type->size;
}

void bpop(buffer* _) {_->size--; _->used -= _->type->size;}


// string buffer ops
buffer* strbuffer(const char* _) {
   buffer* b = balloc(&String);
   while(*_) {bpush(b, (void*)_++);}
   return b;
}

void bconcat(buffer* _, const char* __) {
   while(*__) {bpush(_, (void*)__++);}
}


// buffer printers
void bhexdump(buffer* _) {
    hexdump(_->data, _->bytes);
}

void printb(buffer* _) {
    printf("Buffer: dtype=%s ",_->type->str); 
    printf("using [%zu bytes out of %zu bytes]\n",_->used,_->bytes);
    for (int i=0; i<_->used;i+=_->type->size) {
	printf("\t%p", _->data+i); 
	hexdump(_->data+i, _->type->size);
    }
}

#endif
