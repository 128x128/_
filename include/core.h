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

#include "bits.h"
#include "util.h"
#include "types.h"


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


#endif
