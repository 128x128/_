#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

//#include <curses.h>
#include "bits.h"
#include "util.h"
#include "types.h"

typedef struct buffer {
    void* data;
    dtype type;
    size_t size;
    size_t used;
    size_t bytes;
} buffer;

typedef struct node {
    void* data;
    dtype type;
    buffer* in;
    buffer* out;
} node;

void printb(buffer* _) {
    printf("Buffer %p: dtype=%s items=%zu using %zu bytes out of %zu bytes\n", _, _->type.str, _->size, _->used, _->bytes);
    for (int i=0; i<_->size;i++) {
	printf("\t");
	_->type.print(_->data+(i*_->type.size));
	printf("\n");
    }
}

void printn(node* _) {
    printf("Node %p: dtype=%s \n", _, _->type.str);
    if (_->data) _->type.print(_->data);
}


// alloc & delloc ops

buffer* balloc(struct dtype _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    b->data = malloc(_.size);
    b->type = _;
    b->bytes = _.size;
    b->used = 0;
    b->size = 0;
    return b;
}

node* nalloc(struct dtype _) {
    node* n = (node*)malloc(sizeof(node));
    n->type = _;
    n->in = balloc(Ptr);
    n->out = balloc(Ptr);
    return n;
}

void bdelloc(buffer* _) {
    if (_->data) free(_->data); free(_);
}

void ndelloc(node* _) {
    if (_->data) free(_->data); free(_);
    bdelloc(_->in); bdelloc(_->out);
}

// buffer ops

void brelloc(buffer* _) { // 2x resize
    void* __ = malloc(_->bytes*2);
    memcpy(__, _->data, _->bytes);
    _->bytes *= 2;
    free(_->data); 
    _->data = __;
}

void bpush(buffer* _, void* __) {
    if (_->used == _->bytes) brelloc(_);
    //char* d = (char*)_->data+_->used;
    //char* s = (char*)__;
    //for (int i=0;i<_->type.size;i++) {
	//d[i] = s[_->type.size-i-1];
    //}
    memcpy(_->data+_->used, __, _->type.size);
    _->size++; _->used += _->type.size;
}

void bpop(buffer* _) {_->size--; _->used -= _->type.size;}

// node ops

void direct(node* x, node* y) {
    void** _x = (void**)&x;
    void** _y = (void**)&y;
    hexdump(&y, 8);
    bpush(x->out, _y); 
    bpush(y->in, _x);
}

void undirect(node* x, node* y) {
    direct(x, y); direct(y, x);
}


//void dlistpush(dlist* x, const char *c) {
    //node* s = node(c);
    //if (x->head==NULL) {
	//x->head = s;
	//x->tail = s;
	//return;
    //}
    //node* t = x->tail;
    //s->prev = t;
    //t->next = s;
    //x->tail = s;
//}

//void dlistpop(dlist* x) {
    //if (x->head==NULL) return;
    //if (x->head==x->tail) {
	//dlistdelloc(x->head);
	//x->head = NULL;
	//x->tail = NULL;
	//return;
    //}
    //node* last = x->tail;
    //node* slast = last->prev;
    //if (slast!=NULL) {
	//slast->next = NULL;
	//x->tail = slast;
	//dlistdelloc(last);
    //}
//}


void bhexdump(buffer* _) {hexdump(_->data, _->bytes);}

void nhexdump(node* _) {
    if (_->data) hexdump(_->data, _->type.size);
    printf("in\n");
    bhexdump(_->in);
    printf("out\n");
    bhexdump(_->out);
}

#endif
