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
typedef void*            data; 
typedef void**           hashmap; 
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


#define BUFFER_SIZE       4096
#define HASHMAP_SIZE      1024

#define H32_BASE          3323198485ul
#define H64_BASE          525201411107845655ull
#define H32_MUL           0x5bd1e995
#define H64_MUL           0x5bd1e9955bd1e995

// hashmap
#define DELETE_H(k,h)     memset(ADDRESS_H(k,h),0,sizeof(void*))
#define INSERT_H(k,h,x)   NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(void*)))
#define ADDRESS_H(k,h)    h[k % HASHMAP_SIZE]
#define INSPECT_H(k,h)    *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)    INSPECT_H(k,h) ? true : false 

// Regex
#define EPSILON           0xFFFFFFFF

#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct scanner
{
    byte           buffer[BUFFER_SIZE]; 
    fileptr        fp;
}
scanner;

typedef struct monolith 
{
    mlStatus       status;
} 
monolith;


// global structs
monolith   Monolith;
scanner    Scanner;

// hash
uint32_t h32(string key) 
{
    // MurmurOAAT32 hash
    uint32_t h = H32_BASE;
    for (;*key;++key){h^=*key;h*=H32_MUL;h^=h>>15;}
    return h;
}

uint64_t h64(string key) 
{
    // MurmurOAAT64 hash
    uint64_t h = H64_BASE;
    for (;*key;++key){h^=*key;h*=H64_MUL;h^=h>>47;}
    return h;
}


// Trigon

#define TRIGON              Trigon* 
#define TRIGON_HEAD(T)      ((TRIGON)T->x)
#define TRIGON_TAIL(T)      ((TRIGON)T->y)

typedef struct Trigon 
{
    data x;
    data y;
    data z;
}
Trigon, List, Stack, Tuple, Pair;

TRIGON set(TRIGON T, data x, data y, data z) 
{
    T->x = x;
    T->y = y;
    T->z = z;
    return T;
}

TRIGON initTrigon() 
{
    return set((TRIGON)malloc(sizeof(Trigon)), NULL, NULL, NULL);
}

TRIGON prtTrigon(TRIGON T) 
{
    printf("TRIGON: "); hexdump(T, sizeof(data)*3);
    return T;
}


// Reconginizer

#define L_BRCKT             0x28
#define R_BRCKT             0x29
#define L_RANGE             0x5B
#define R_RANGE             0x5D

#define IS_CLO(x)           (x == '*')
#define IS_ALT(x)           (x == '|')
#define IS_LBK(x)           (x == L_BRCKT)
#define IS_RBK(x)           (x == R_BRCKT)
#define IS_RAN(x)           (x == L_RANGE)
#define IS_NUM(x)           (x >= '0' && x <= '9')
#define IS_LOW(x)           (x >= 'a' && x <= 'z')
#define IS_UPP(x)           (x >= 'A' && x <= 'Z')
#define IS_SYM(x)           (IS_RAN(x) || IS_NUM(x) || IS_LOW(x) || IS_UPP(x))

#define INIT_NFA            (initPair(initList(), initList()))
#define NFA_STRUCT(x)       ((NFA*)x->data)
#define NFA_START(x)        ((state*)x->head)
#define NFA_END(x)          ((state*)x->tail)

#define REGEX_CLO_CASE(x)   (IS_CLO(x) ? REGEX_CONCAT        : REGEX_CLOSURE)
#define REGEX_ALT_CASE(x)   (IS_ALT(x) ? REGEX_CLO_CASE(x)   : REGEX_ALTERNATION)
#define REGEX_CLOSE_CASE(x) (IS_RBK(x) ? REGEX_ALT_CASE(x)   : REGEX_CLOSE)
#define REGEX_OPEN_CASE(x)  (IS_LBK(x) ? REGEX_CLOSE_CASE(x) : REGEX_OPEN)
#define REGEX_CASE(x)       (IS_SYM(x) ? REGEX_OPEN_CASE(x)  : REGEX_SYMBOL)

hashmap SYMBOL_TABLE[HASHMAP_SIZE];


enum regexType 
{
    REGEX_SYMBOL,
    REGEX_OPEN,
    REGEX_CLOSE,
    REGEX_ALTERNATION,
    REGEX_CLOSURE,
    REGEX_CONCAT,
};

typedef enum regexTransform
{
    ALTERNATION,
    CONCAT,
    CLOSURE,
}
regexTransform;

idx nextOccurance(string x, byte c) 
{
    string y = x;
    while(*y++!=c){}
    return y-x;
}

regexTransform regexStatus = CONCAT;
NFA* currNFA = NULL;
NFA* prevNFA = NULL;

NFA* alterationNFA(NFA* x, NFA* y) 
{
    NFA*  G = INIT_NFA;
    initTransition(NFA_START(G), NFA_START(x), EPSILON);
    initTransition(NFA_START(G), NFA_START(x), EPSILON);
    initTransition(NFA_END(x), NFA_END(G), EPSILON);
    initTransition(NFA_END(y), NFA_END(G), EPSILON);
    return G;
}

NFA* concatNFA(NFA* x, NFA* y) 
{
    NFA* G = INIT_NFA;
    initTransition(NFA_START(G), NFA_START(x), EPSILON);
    initTransition(NFA_END(y), NFA_END(G), EPSILON);
    return G;
}

NFA* closureNFA(NFA* x) 
{
    NFA* G = INIT_NFA;
    initTransition(NFA_END(x), NFA_START(x), EPSILON);
    initTransition(NFA_START(G), NFA_END(G), EPSILON);
    return G;
}

NFA* constructNFA() 
{
    switch(regexStatus) 
    {
	case CONCAT:      currNFA = concatNFA(prevNFA, currNFA);
			  break; 
	case ALTERNATION: currNFA = alterationNFA(prevNFA, currNFA); 
			  break; 
	case CLOSURE:     currNFA = closureNFA(currNFA);
			  break; 
	default:          break;  
    }
    return currNFA;
}

string nextSymbol(string x) 
{
    while (IS_SYM(*x++)==0){}
    NFA*         G = INIT_NFA;
    idx          n = (IS_RAN(*x) ? nextOccurance(x, R_RANGE) : 1);
    string symbol = (string)calloc(n+1, 1); memcpy(symbol, x, n);
    INSERT_H(h64(symbol), &SYMBOL_TABLE, symbol);
    initTransition(NFA_START(G), NFA_END(G), h64(symbol));

    prevNFA = currNFA;
    currNFA = G;
    constructNFA();

    return x + n;
}


NFA* initNFA(string x)
{
    while(*x)
    {
	switch(REGEX_CASE(*x)) 
	{
	    case REGEX_SYMBOL:      x            = nextSymbol(x);  
				    break;
	    case REGEX_OPEN:        currNFA = initNFA(++x); 
				    break;
	    case REGEX_ALTERNATION: regexStatus  = ALTERNATION; 
				    break;
	    case REGEX_CLOSURE:     regexStatus  = CLOSURE;
				    break;
	    case REGEX_CONCAT:      regexStatus  = CONCAT;
				    break;
	    case REGEX_CLOSE:       return NULL;
	    default:                regexStatus  = CONCAT;
				    x++;
	}
    }
    return NULL;
}


// Main
void run() 
{
    TRIGON x = initTrigon();
    prtTrigon(x);
    //initNFA("a(b|c)");
}

void execute(filename* file)
{
    Scanner.fp = fopen(file, "r");
    while(fgets((char*)&Scanner.buffer[0], BUFFER_SIZE, Scanner.fp)) 
    {
	run();
    }
    fclose(Scanner.fp);
}

void prompt()
{
    while(true) 
    {
	printf(BLU "> " GRN); 
	scanf("%s", &Scanner.buffer[0]); 
	run();
    }
}

#endif
