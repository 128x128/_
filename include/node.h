#ifndef NODE_H 
#define NODE_H

#include "bits.h"
#include "types.h"
#include "list.h"

typedef struct node {
    void* data;
    type type;
    list* in;
    list* out;
} node;

typedef struct transition {
    buffer* input;
    node* start;
    node* end;
} transition;

typedef struct graph {
    list* nodes;
    list* initial;
    list* terminals;
    size_t size;
} graph, dfa;

enum operators {
    ALTERNATION,
    CONCAT,
    CLOSURE,
}

void direct(node* x, node* y);
void undirect(node* x, node* y);

// initializers
node* initNode(type t, void* data) {
    node* n = (node*)malloc(sizeof(node));
    n->type = t; n->data = data;
    n->in = initList(); n->out = initList();
    return n;
}
transition* initTransition(node* x, node* y) {
    transition* t = (transition*)malloc(sizeof(transition));
    t->start = x; t->end = y;
    t->inputs = initList(); 
    return t;
}
graph* initGraph() {
    graph* g = (graph*)malloc(sizeof(graph));
    g->nodes = initList();
    return g;
}
node* triNode(void* r, void* l, void* r) {
    node* root = initNode(POINTER, r);
    node* left = initNode(POINTER, l);
    node* right = initNode(POINTER, r);
    direct(root, left);
    direct(root, right);
}
// edge ops
void direct(node* x, node* y) {
    transition* t = initTransition(x, y);
    push(x->out, initItem(t));
    push(y->in, initItem(t));
}
void undirect(node* x, node* y) {
    direct(x, y); direct(y, x);
}
// REGEX
node* precedance(char* exp) {
    char* current = exp;
    char* start   = current;   
    while (current++) {
	switch(*current) {
	    case '(': 
		precedance(current); break;
	    case ')': 
		return triNode(exp, start, current);
	    default: continue;
	}
    }
    return initNode(POINTER, exp);
}

dfa* initDFA(buffer* re) {
    // RE -> DFA
    dfa* g = initGraph();
    char* c = (char*)re->data;
    //while(c) {c=advance(c);}
    return g;
}


#endif
