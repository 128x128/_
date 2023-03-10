#ifndef LIST_H
#define LIST_H

#include "common.h"

typedef struct item 
{
    void* data;
    struct item* next;
    struct item* prev;
} item;

typedef struct list 
{
    item* head;
    item* tail;
    size_t size;
} 
list, stack, pair, tuple, state, transition,
NFA, NFAs, State, Transition;

void push(list* l, void* data);

// init
item* initItem(void* data) 
{
    item* i = (item*)malloc(sizeof(item));
    i->data = data;
    i->next = NULL;
    i->prev = NULL;
    return i;
}
list* initList() 
{
    list* l = (list*)malloc(sizeof(list));
    l->head = NULL; l->tail = NULL;
    l->size = 0;
    return l;
}
pair* initPair(void* x, void* y) 
{
    // p = (x, y)
    pair* p = initList();
    push(p, x); push(p, y);
    return p;
}
transition* initTransition(void* x, void* y, uint64_t z) 
{
    // t = x -> y
    transition* t = initPair((void*)z, y);
    push((state*)x, t);
    return t;
}
//free
void freeItem(item* i) {
    free(i->data);
    free(i);
}
void freeList(list* l) {
    free(l);
}
// property ops
bool isEmpty(list* l) {
    return (l->size==0) ? true : false;
}
// add ops
void addEmpty(list* l, item* i) {
    // [] -> [i]
    l->head = i; l->tail = i;
    l->size++;
}
void push(list* l, void* data) {
    // [3, 5, 2, 1] ->[3, 5, 2, 1, i]
    item* i = initItem(data);
    if (isEmpty(l)) return addEmpty(l, i);
    item* t = l->tail; 
    t->next = i; 
    i->prev = t;
    l->tail = i;
    l->size++;
}
void enqueue(list* l, void* data) {
    // [3, 5, 2, 1] ->[i, 3, 5, 2, 1]
    item* i = initItem(data);
    if (isEmpty(l)) return addEmpty(l, i);
    item* h = l->head; 
    h->prev = i; 
    i->next = h;
    l->head = i;
    l->size++;
}
// del ops
item* emptyList(list* l) {
    // [1] -> []
    if (isEmpty(l)) return NULL;
    item* i = l->head;
    l->head = NULL; 
    l->tail = NULL;
    l->size--;
    return i;
}
item* pop(list* l) {
    // [3, 5, 2, 1] -> [3, 5, 2]
    if (isEmpty(l)) return NULL;
    if (l->size==1) return emptyList(l);
    item* t = l->tail;
    if (t->prev != NULL) t->prev->next = NULL;
    l->tail = t->prev;
    l->size--;
    return t;
}
item* dequeue(list* l) {
    // [3, 5, 2, 1] -> [5, 2, 1]
    if (isEmpty(l)) return NULL;
    if (l->size==1) return emptyList(l);
    item* h = l->head;
    if (h->next != NULL) h->next->prev = NULL;
    l->head = h->next;
    l->size--;
    return h;
}

#endif
