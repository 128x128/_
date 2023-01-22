#ifndef NODE_H 
#define NODE_H

#include "bits.h"
#include "types.h"
#include "buffer.h"

typedef struct node {
    void* data;
    type type;
    buffer* in;
    buffer* out;
} node;

node* nalloc(type _, void* __) {
    node* n = (node*)malloc(sizeof(node));
    n->data = __;
    n->type = _;
    n->in = balloc(&Pointer);
    n->out = balloc(&Pointer);
    return n;
}

void ndelloc(node* _) {
    if (_->data) free(_->data); free(_);
    bdelloc(_->in); bdelloc(_->out);
}

void direct(node* x, node* y) {
    void** _x = (void**)&x;
    void** _y = (void**)&y;
    bpush(x->out, _y); 
    bpush(y->in, _x);
}

void undirect(node* x, node* y) {
    direct(x, y); direct(y, x);
}

void nhexdump(node* _) {
    switch(_->type) {
	case BUFFER: bhexdump(_->data); break;
	default: break;
    }
}

node* nunion(buffer* _) {
    buffer* b = balloc(&Buffer);
    for (int i=0;i<_->size;i++) {
	bconcat(b, bidx(_, i));
	bconcat(b, "|");
    }
    bhexdump(b);
    //return nalloc(BUFF, data);
}
#endif
