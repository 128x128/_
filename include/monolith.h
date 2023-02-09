#ifndef MONOLITH_H
#define MONOLITH_H

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

uint32_t h32(const char * key) {
    // MurmurOAAT32 hash
    uint32_t h = (3323198485ul);
    for (;*key;++key) {
	h ^= *key;
	h *= 0x5bd1e995;
	h ^= h >> 15;
    }
    return h;
}

uint64_t h64(const char * key) {
    // MurmurOAAT64 hash
    uint64_t h = (525201411107845655ull);
    for (;*key;++key) {
	h ^= *key;
	h *= 0x5bd1e9955bd1e995;
	h ^= h >> 47;
    }
    return h;
}

#include "bits.h"
#include "util.h"
#include "types.h"
#include "buffer.h"
#include "list.h"


typedef char       src;
typedef char       string;
typedef uint32_t   identifier;
typedef uint32_t   depth;
typedef uint32_t   idx;
typedef uint64_t   hash;
typedef const char regex;
typedef const char name;
typedef const char filename;

typedef struct state state;

typedef enum mlStatus 
{
    SUCCESS = -1, 
    RUNNING =  0, 
    ERROR   =  1,
} 
mlStatus;

typedef enum scStatus 
{
    NONE,
    OPEN,
    CLOSE,
    UPPER,
    LOWER,
    NUMBER,
    RANGE,
    CONNAT,
    ALTERNATION,
    CLOSURE,
} 
scStatus;


#define MAX_KERNELS       3 
#define MAX_KERNEL_BYTES  4096
#define PROMPT_KERNEL     0
#define SYMBOL_KERNEL     1
#define REGEX_KERNEL      2
#define HASHMAP_SIZE      1024*1024

#define STRING            char*
#define ADDRESS           void*
#define HASHMAP           void**
#define BYTES             unsigned char

// monolith
#define MONOLITH          Monolith 

// kernel
#define KERNEL(i)         MONOLITH.kernels[i]
#define KERNEL_BASE(i)    &KERNEL(i).buffer[0]
#define PROMPT            KERNEL_BASE(PROMPT_KERNEL)
#define SYMBOL            KERNEL_BASE(SYMBOL_KERNEL)
#define REGEX             KERNEL_BASE(REGEX_KERNEL)

// hashmap
#define ADDRESS_H(k,h)  h[k%HASHMAP_SIZE]
#define INSPECT_H(k,h)  *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)  INSPECT_H(k,h) ? true : false 
#define DELETE_H(k,h)   memset(ADDRESS_H(k,h),0,sizeof(ADDRESS))
#define INSERT_H(k,h,x) NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(ADDRESS)))


#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct kernel 
{
    BYTES buffer[MAX_KERNEL_BYTES];
    idx   current;
}
kernel;

typedef struct monolith 
{
    mlStatus       status;
    scStatus       scanStatus;
    kernel         kernels[MAX_KERNELS];
    FILE*          src;
    depth          precedence;
    HASHMAP        symbols[HASHMAP_SIZE];
} 
monolith;

monolith Monolith = 
{
    RUNNING,
    NONE,
};

typedef struct edge 
{
    hash   key;
    state* x;
    state* y;
} 
edge;

typedef struct state 
{
    list* edges;
} 
state;

//state* initState(){state* s=(state*)malloc(sizeof(state));s->edges=initList();return s;}
//edge*  initEdge (){edge* e=(edge*)malloc(sizeof(edge));return e;}
//void   setStates(edge* e,state* x,state* y){e->x=x,e->y=y;}
//void   setEdges (edge* e,state* x,state* y){push(x->edges,e);push(y->edges,e);}

int buff2int(STRING b, int n) {
    int x=0;
    //for(int i=0;i<Monolith.bufferIdx;i++){x+=((Monolith.bufferIdx-i)*10*(Monolith.buffer[i]-'0'));}
    //Monolith.bufferIdx=0;
    return x;
}

void buffpush(char c) {
    //Monolith.buffer[Monolith.bufferIdx++]=c;
}

//edge* range(char* x) {
    //edge*  e = initEdge();
    //int i = 0;
    //while (*x++!=']') {i++;};
    //e->data = (string*)malloc(i);
    //memcpy(&e->data[0], x-i, i+1);
    //return e;
//}

scStatus scanChar(char c) 
{
    switch(c) 
    {
	case '(': Monolith.precedence++; return OPEN;
	case ')': Monolith.precedence--; return CLOSE;
	case '|': return ALTERNATION;
	case '*': return CLOSURE; 
	case '[': return RANGE;
	case ']': return RANGE;
	case '-': return RANGE;
	default : return NONE;
    }
}
scStatus ldsymbol() {return NONE;}

scStatus advance(char c) 
{
    //state* x = initState();
    //state* y = initState();
    //edge*  e = initEdge();
    //setStates(e,x,y);
    //setEdges (e,x,y);
    if (c>='0'&&c<='9') {return NUMBER;}
    if (c>='A'&&c<='Z') {return UPPER;}
    if (c>='a'&&c<='z') {return LOWER;}
    else return scanChar(c);
}

void regex2NFA(regex* x){while(*x++){Monolith.scanStatus=advance(*x);}}


// Main
void run() { 
    hd(PROMPT, 32);
    //regex2NFA("[a-z](b|c)*");
}

void movefp(int n) {fseek(Monolith.src, n, SEEK_CUR);}
long resetfp() {fseek(Monolith.src, 0, SEEK_SET); return ftell(Monolith.src);}
long filesize() {fseek(Monolith.src, 0, SEEK_END); return ftell(Monolith.src);}
void ldpromptkernel(int n) {fread(PROMPT, n, 1, Monolith.src);}
void runkernel(int n) {ldpromptkernel(n); movefp(n); run();} 
void runfile() {
    long n = filesize(); resetfp();
    for(int i=0;i<floor(n / MAX_KERNEL_BYTES);i++) {
	runkernel(MAX_KERNEL_BYTES);
    }
    runkernel(n % MAX_KERNEL_BYTES);
}
void execute(filename* file) {
    Monolith.src = fopen(file, "r");
    runfile();
    fclose(Monolith.src);
}
void prompt(){while(true){printf(BLU"> "GRN); scanf("%s",PROMPT); run();}}
void report(int line,char* where,char* msg){printf("[Line: %d] Error %s :%s",line,where,msg);}
void error(int line,char* msg){report(line, "", msg);}

#endif
