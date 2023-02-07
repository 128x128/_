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

void pushBuffer(buffer* _, void* __);
void popBuffer(buffer* _);
void concatBuffer(buffer* x, buffer* y);

// alloc buffer
buffer* initBuffer(type _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    b->data = malloc(typemap(_)->size);
    b->type = _; b->bytes = typemap(_)->size;
    b->used = 0; b->size = 0;
    return b;
}
buffer* initStringBuffer(const char* _) {
   buffer* b = initBuffer(CHAR);
   while(*_) {pushBuffer(b, (void*)_++);}
   return b;
}

void dellocBuffer(buffer* _) {
    if (_->data) free(_->data); free(_);
}
void rellocBuffer(buffer* _) {
    void* __ = malloc(_->bytes*2);
    memcpy(__, _->data, _->bytes);
    _->bytes *= 2; free(_->data); 
    _->data = __;
}

// buffer props
size_t tsize(buffer* _){return typemap(_->type)->size;}
void* end(buffer* _){return _->data+_->used;}
void* offset(buffer* _, int i){return _->data+(i*tsize(_));}
// buffer ops
void pushBuffer(buffer* _, void* __) {
    if (_->used == _->bytes) rellocBuffer(_);
    memcpy(_->data+_->used, __, tsize(_));
    _->size++; _->used += tsize(_);
}
void popBuffer(buffer* _) {
    memset(end(_)-tsize(_), 0x00, tsize(_));
    _->size--; _->used -= tsize(_);
}
void concatBuffer(buffer* x, buffer* y) {
    char* c = (char*)y->data;
    for (int i=0;i<y->used;i++){pushBuffer(x, (void*)c++);}
}
// regex ops
buffer* regex_union(buffer* _) {
    buffer* b = initStringBuffer("(");
    for (int i=0;i<_->size;i++) {
	concatBuffer(b, _);
	pushBuffer(b, &"|");
    }
    popBuffer(b); pushBuffer(b, &")");
    return b;
}
buffer* regex_concat(buffer* x, buffer* y) {
    buffer* b = initStringBuffer("");
    concatBuffer(b, x->data);
    concatBuffer(b, y->data);
    return b;
}
buffer* regex_closure(buffer* _) {
    buffer* b = initStringBuffer("");
    concatBuffer(b, _->data);
    pushBuffer(b, &"*");
    return b;
}
//string ops
//int buffer2int(buffer* b) {
    //char* c=b->data; char* s=b->data;int l=0;int x=0;
    //while(c>='0'&&c<='9'){c++;l++;}
    //for(int i=0;i<l;i++){x+=(l*10*(s-'0'));s++;l--;}
    //return x;
//}
// buffer printers
void bufferhd(buffer* _) {hd(_->data, _->bytes);}
void prtBuffer(buffer* _) {
    printf("\033[93mBuffer: type=%s ",typemap(_->type)->str); 
    printf("\033[91m[%zu bytes out of %zu bytes]\033[0m\n",_->used,_->bytes);
    for (int i=0; i<_->used;i+=tsize(_)) {printf("\t");hd(_->data+i, tsize(_));}
}


#endif
