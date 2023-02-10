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

#include "bits.h"
#include "util.h"
#include "types.h"
#include "buffer.h"
#include "list.h"


typedef char*            string;
typedef uint32_t         identifier;
typedef uint32_t         depth;
typedef uint32_t         idx;
typedef uint64_t         hash;
typedef const char       name;
typedef const char       filename;
typedef unsigned char    byte;
typedef unsigned char    kernel;
typedef void**           table; 
typedef uint8_t          u8kernel;
typedef uint32_t         u32kernel; 
typedef uint64_t         u64kernel;
typedef FILE*            fileptr;

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


#define BUFFER_SIZE       4096
#define HASHMAP_SIZE      1024

#define H32_BASE          3323198485ul
#define H64_BASE          525201411107845655ull
#define H32_MUL           0x5bd1e995
#define H64_MUL           0x5bd1e9955bd1e995

// hashmap
#define DELETE_H(k,h)     memset(ADDRESS_H(k,h),0,sizeof(void*))
#define INSERT_H(k,h,x)   NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(void*)))
#define ADDRESS_H(k,h)    h->map[k%HASHMAP_SIZE]
#define INSPECT_H(k,h)    *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)    INSPECT_H(k,h) ? true : false 


#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct hashmap 
{
    table          map[HASHMAP_SIZE];
}
hashmap;

typedef struct scanner
{
    byte           buffer[BUFFER_SIZE]; 
    scStatus       scanStatus;
    fileptr        fp;
}
scanner;

typedef struct language 
{
    byte           symbol[BUFFER_SIZE]; 
    idx            symbolidx;
    byte           buffer[BUFFER_SIZE]; 
    hashmap        symbols;
    depth          precedence;
}
language;


typedef struct monolith 
{
    mlStatus       status;
} 
monolith;

typedef struct state 
{
    list*          edges;
} 
state;

typedef struct transition 
{
    hash           key;
    state*         dest;
} 
transition;

// global structs
monolith   Monolith;
scanner    Scanner;
language   Language;

uint32_t h32(const char * key) 
{
    // MurmurOAAT32 hash
    uint32_t h = H32_BASE;
    for (;*key;++key){h^=*key;h*=H32_MUL;h^=h>>15;}
    return h;
}

uint64_t h64(const char * key) 
{
    // MurmurOAAT64 hash
    uint64_t h = H64_BASE;
    for (;*key;++key){h^=*key;h*=H64_MUL;h^=h>>47;}
    return h;
}


//state* initState(){state* s=(state*)malloc(sizeof(state));s->edges=initList();return s;}
//edge*  initEdge (){edge* e=(edge*)malloc(sizeof(edge));return e;}
//void   setStates(edge* e,state* x,state* y){e->x=x,e->y=y;}
//void   setEdges (edge* e,state* x,state* y){push(x->edges,e);push(y->edges,e);}

int buff2int(char* b, int n) {
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

// language reconginizer

scStatus scanChar(char c) 
{
    switch(c) 
    {
	case '(': return OPEN;
	case ')': return CLOSE;
	case '|': return ALTERNATION;
	case '*': return CLOSURE; 
	case '[': return RANGE;
	case ']': return RANGE;
	case '-': return RANGE;
	default : return NONE;
    }
}

void pushsymbol(char c) 
{
}

void advance(char c) 
{
    if (c=='['||c==']') {}
    if (c>='0'&&c<='9') {}
    if (c>='A'&&c<='Z') {}
    if (c>='a'&&c<='z') {}
    else scanChar(c);
}

void regex2NFA(string x)
{
    while(*x++){advance(*x);}
}


// Main

void runScanner() 
{
    regex2NFA("a(b|c)*");
    hd(&Scanner.buffer[0], 32);
}

void execute(filename* file)
{
    Scanner.fp = fopen(file, "r");
    while(fgets((char*)&Scanner.buffer[0], BUFFER_SIZE, Scanner.fp)) 
    {
	runScanner();
    }
    fclose(Scanner.fp);
}

void prompt()
{
    while(true) 
    {
	printf(BLU "> " GRN); 
	scanf("%s", &Scanner.buffer[0]); 
	runScanner();
    }
}

void report(int line,char* where,char* msg)
{
    printf("[Line: %d] Error %s :%s",line,where,msg);
}

void error(int line,char* msg)
{
    report(line, "", msg);
}

#endif
