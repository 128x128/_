#ifndef ITEM_H 
#define ITEM_H

#include "common.h"

typedef struct item {
    void* data;
    struct item* next;
    struct item* prev;
} item;

item* initItem(void* data) {
    item* i = (item*)malloc(sizeof(item));
    i->data = data;
    i->next = NULL;
    i->prev = NULL;
    return i;
}

#endif
