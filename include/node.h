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

// alloc ops
node* nalloc(type _, void* __) {
    node* n = (node*)malloc(sizeof(node));
    n->data = __;
    n->type = _;
    n->in = balloc(NODE);
    n->out = balloc(NODE);
    return n;
}

void ndelloc(node* _) {
    if (_->data) free(_->data); free(_);
    bdelloc(_->in); bdelloc(_->out);
}

node* nstring(const char* _) {
    return  nalloc(STRING, string(_));
}

// edge ops
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
    bhexdump(_->data);
    //switch(_->type) {
	//case BUFFER: bhexdump(_->data); break;
	//default: break;
    //}
}


// regex ops
node* regex_union(buffer* _) {
    buffer* b = string("(");
    for (int i = 0; i < _->size; i++) {
	node* n = (node*)offset(_, i);
	concat(b, (buffer*)n->data);
	bpush(b, &"|");
    }
    bpop(b); bpush(b, &")");
    return nalloc(STRING, b);
}

node* regex_concat(node* x, node* y) {
    buffer* b = string("");
    concat(b, x->data);
    concat(b, y->data);
    return nalloc(STRING, b);
}

node* regex_closure(node* _) {
    buffer* b = string("");
    concat(b, _->data);
    bpush(b, &"*");
    return nalloc(STRING, b);
}

#endif
