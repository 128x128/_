#ifndef NODE_H 
#define NODE_H

#include "bits.h"
#include "types.h"
//#include "buffer.h"


// props
#define tsize(n) typemap(n->type)->size 

typedef struct buffer buffer;
typedef struct node node;
typedef struct list list;

typedef struct buffer {
    void* data;
    size_t size;
    size_t used;
    size_t bytes;
} buffer;

typedef struct node {
    void* data;
    type type;
    list* in;
    list* out;
} node;

typedef struct list {
    node* head;
    node* tail;
    size_t size;
} list;


// setter ops
void  set_type(node* n, type t)           {n->type = t;   }
void  set_data(node* n, void *data)       {n->data = data;}
//void  set_edge(node* n)                   {n->in = balloc(NODE); n->out = balloc(NODE);}
//node* set_node(node* n, type t, void *d)  {set_type(n, t); set_data(n, d);}

// node alloc ops
node* node_malloc(type t, void* data) {
    init_node(n, t, d);
    node* n = (node*)malloc(sizeof(node));
    n->type = t; n->data = data;
    n->in = 
    //n->in = balloc(NODE); n->out = balloc(NODE);
    return n;
}

void node_free(node* _) {
    if (_->data) free(_->data); free(_);
    //bdelloc(_->in); bdelloc(_->out);
}

//node* nstring(const char* _) {
    //return  nalloc(STRING, string(_));
//}


// initializers

buffer* buffer_malloc(node* _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    //b->data = malloc(typemap(_)->size);
    //b->type = _; b->bytes = typemap(_)->size;
    //b->used = 0; b->size = 0;
    return b;
}

list* list_malloc() {
    list* l = (list*)malloc(sizeof(list));
    l->head = NULL; l->tail = NULL; l->size = 0;
    return l;
}

// edge ops
//void direct(node* x, node* y) {
    //void** _x = (void**)&x;
    //void** _y = (void**)&y;
    //bpush(x->out, _y); 
    //bpush(y->in, _x);
//}

//void undirect(node* x, node* y) {
    //direct(x, y); direct(y, x);
//}

//void nhexdump(node* _) {
    //bhexdump(_->data);
//}

// regex ops
//node* regex_union(buffer* _) {
    //buffer* b = string("(");
    //for (int i = 0; i < _->size; i++) {
	//node* n = (node*)offset(_, i);
	//concat(b, (buffer*)n->data);
	//bpush(b, &"|");
    //}
    //bpop(b); bpush(b, &")");
    //return nalloc(STRING, b);
//}

//node* regex_concat(node* x, node* y) {
    //buffer* b = string("");
    //concat(b, x->data);
    //concat(b, y->data);
    //return nalloc(STRING, b);
//}

//node* regex_closure(node* _) {
    //buffer* b = string("");
    //concat(b, _->data);
    //bpush(b, &"*");
    //return nalloc(STRING, b);
//}


//void lexically_anaylse(node* _) {
    //return;
//}


//struct node_t {
    //node* (*string)(const char*);
    //void (*direct)(node*, node*);
    //void (*undirect)(node*, node*);
    //node* (*Union)(buffer*);
    //node* (*Concat)(node*, node*);
    //node* (*Closure)(node*);
//};

//struct node_t Node = {
    //&nstring,
    //&direct,
    //&undirect,
    //&regex_union,
    //&regex_concat,
    //&regex_closure,
//};

#endif
