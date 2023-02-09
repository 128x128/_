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

#define INITIAL      0
#define MAX_CHARS    4096
#define HASHMAP_SIZE 1024*1024
#define STRING       char*
#define ADDRESS      void*
#define HASHMAP      void**

// hashmap
#define ADDRESS_H(k,h)  h[k%HASHMAP_SIZE]
#define INSPECT_H(k,h)  *(ADDRESS_H(k, h))
#define FREE_H(k,h)     INSPECT_H(k,h) ? true : false
#define DELETE_H(k,h)   memset(ADDRESS_H(k,h),0,sizeof(ADDRESS))
#define INSERT_H(k,h,x) memcpy(ADDRESS_H(k,h),x,sizeof(ADDRESS))

#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct monolith monolith;

mlStatus run     (src* code);
mlStatus execute (filename* file);
mlStatus prompt  ();

typedef struct monolith 
{
    mlStatus       status;
    mlStatus       (*run)     (src*);
    mlStatus       (*execute) (filename*);
    mlStatus       (*prompt)  ();
    idx            bufferIdx;
    idx            regexIdx;
    string         buffer[MAX_CHARS];
    string         regex[MAX_CHARS];
    string*        program;
    scStatus       scanStatus;
    depth          precedence;
    HASHMAP        table[HASHMAP_SIZE];
} 
monolith;

monolith Monolith = 
{
    RUNNING,
    &run,
    &execute,
    &prompt,
    INITIAL,
    INITIAL,
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

state* initState(){state* s=(state*)malloc(sizeof(state));s->edges=initList();return s;}
edge*  initEdge (){edge* e=(edge*)malloc(sizeof(edge));return e;}
void   setStates(edge* e,state* x,state* y){e->x=x,e->y=y;}
void   setEdges (edge* e,state* x,state* y){push(x->edges,e);push(y->edges,e);}

int buff2int(STRING b, int n) {
    int x=0;
    for(int i=0;i<Monolith.bufferIdx;i++){x+=((Monolith.bufferIdx-i)*10*(Monolith.buffer[i]-'0'));}
    Monolith.bufferIdx=0;
    return x;
}

void buffpush(char c) {Monolith.buffer[Monolith.bufferIdx++]=c;}

edge* range(char* x) {
    edge*  e = initEdge();
    int i = 0;
    while (*x++!=']') {i++;};
    //e->data = (string*)malloc(i);
    //memcpy(&e->data[0], x-i, i+1);
    return e;
}

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

scStatus advance(char c) 
{
    state* x = initState();
    state* y = initState();
    edge*  e = initEdge();
    setStates(e,x,y);
    setEdges (e,x,y);
    if (c>='0'&&c<='9') {return NUMBER;}
    if (c>='A'&&c<='Z') {return UPPER;}
    if (c>='a'&&c<='z') {return LOWER;}
    else return scanChar(c);
}

void regex2NFA(regex* x){while(*x++){Monolith.scanStatus=advance(*x);}}


// Main
mlStatus run(src* code) { 
    //regex2NFA("[a-z](b|c)*");
    //void* x = ADDRESS_H(k,&Monolith.table);
    //INSERT_H(k,&Monolith.table,(void*)0x01);
    
    printf("table->%p\n",&Monolith.table);
    //hd(ADDRESS_H(h64("0"),&Monolith.table), 8);
    //hd(ADDRESS_H(h64("1"),&Monolith.table), 8);
    printf("%p\n", Monolith.table[h64("1")%HASHMAP_SIZE]);
    //printf("%p\n",x);
    return SUCCESS;
}

mlStatus execute(filename* file) 
{
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
mlStatus runbuffer(){return MLSTATUS(run(&Monolith.buffer[0]));}
mlStatus scan(){scanf("%s",&Monolith.buffer[0]); return runbuffer();}
mlStatus prompt(){while(1){printf(BLU"> "GRN); scan();} return SUCCESS;}

void report(int line,char* where,char* msg){printf("[Line: %d] Error %s :%s",line,where,msg);}
void error(int line,char* msg){report(line, "", msg);}

#endif
