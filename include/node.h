#ifndef NODE_H 
#define NODE_H

#include "bits.h"
#include "types.h"
#include "buffer.h"

typedef struct node {
    void* data;
    dtype* type;
    buffer* in;
    buffer* out;
} node;

void printn(node* _) {
    //printf("Node %p: dtype=%d \n", _, _->type);
    //if (_->data) _->type.print(_->data);
}


node* nalloc(TYPE _) {
    node* n = (node*)malloc(sizeof(node));
    n->type = tmap(_);
    n->in = balloc(&Pointer);
    n->out = balloc(&Pointer);
    return n;
}

void ndelloc(node* _) {
    if (_->data) free(_->data); free(_);
    //bdelloc(_->in); bdelloc(_->out);
}

void direct(node* x, node* y) {
    void** _x = (void**)&x;
    void** _y = (void**)&y;
    //bpush(x->out, _y); 
    //bpush(y->in, _x);
}

void undirect(node* x, node* y) {
    direct(x, y); direct(y, x);
}

void nhexdump(node* _) {
    //if (_->data) hexdump(_->data, _->type.size);
    //printf("in\n");
    //bhexdump(_->in);
    //printf("out\n");
    //bhexdump(_->out);
}

#endif
