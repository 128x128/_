#ifndef MONOLITH 
#define MONOLITH

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <termios.h>
#include <stdarg.h>

#include "bits.h"
#include "util.h"
#include "types.h"
#include "buffer.h"
#include "list.h"
#include "hash.h"

//#include "common.h"
//#include "list.h"
//#include "token.h"
//#include "node.h"

#define MAX_CHARS 4096

typedef char       src;
typedef int        identifier;
typedef const char name;
typedef const char filename;

typedef enum mlStatus 
{
    SUCCESS = -1, 
    RUNNING =  0, 
    ERROR   =  1,
} 
mlStatus;

#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct monolith monolith;
typedef struct class    class;
typedef struct instance instance;

typedef struct monolith 
{
    mlStatus  status;
    mlStatus  (*run)     (src*);
    mlStatus  (*execute) (filename*);
    mlStatus  (*prompt)  ();
    src*      program;
    src       promptBuffer[MAX_CHARS];
} 
monolith;

mlStatus run     (src* code);
mlStatus execute (filename* file);
mlStatus prompt  ();

monolith Monolith = 
{
    RUNNING,
    &run,
    &execute,
    &prompt,
};


typedef char string;
typedef const char regex;
typedef struct exptree exptree;
typedef struct state state;

typedef struct charcter {
    regex data;
} charcter;

typedef struct pair {
    void* x;
    void* y;
} pair;

typedef struct edge {
    regex* data;
    size_t n;
    state* x;
    state* y;
} edge;

typedef struct state {
    list* transitions;
} state;

typedef struct exptree {
    regex* data;
    size_t* n;
    exptree* left;
    exptree* right;
} exptree;

typedef struct NFA {
    size_t n;
} NFA;

state* initState   (){state* s=(state*)malloc(sizeof(state));s->transition=initList();return s;}
edge*  initEdge    (){edge* t=(edge*)malloc(sizeof(edge));return t;}

void edgeAddStates (edge* e,state* x,state* y){e->x=x,e->y=y;}
void stateAddEdge  (edge* e,state* x,state* y){push(x->transitions,e);push(y->transitions,e);}

void advance(regex* c) {
    switch(*c) {
	case '[':
	    c = rdrange(c);
    }
}
NFA* regex2NFA(regex* x) {
    regex* c = x;
    while(*c++) {advance(*c);}
}


void report(int line, char* where, char* msg) {
    printf("[Line: %d] Error %s :%s", line, where, msg);
}
void error(int line, char* msg) {report(line, "", msg);}


// Main
mlStatus run(src* code) { 
    return SUCCESS;
}

mlStatus execute(filename* file) {

    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long n = ftell(f);

    fseek(f, 0, SEEK_SET);
    src* code = (src*)malloc(n+1);

    Monolith.program = code;
    fread(code, n, 1, f);
    fclose(f);

    return MLSTATUS(run(code));
}
mlStatus runbuffer(){return MLSTATUS(run(&Monolith.promptBuffer[0]));}
mlStatus scan(){scanf("%s",&Monolith.promptBuffer[0]); return runbuffer();}
mlStatus prompt(){while(1){printf(BLU"> "GRN); scan();} return SUCCESS;}

#endif
