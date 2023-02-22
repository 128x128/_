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
typedef unsigned char    kernel;
typedef void*            data; 
typedef void**           hashmap; 
typedef FILE*            fileptr;

typedef enum mlStatus 
{
    SUCCESS = -1, 
    RUNNING =  0, 
    ERROR   =  1,
} 
mlStatus;


#define BUFFER_SIZE           4096

// hashmap
#define HASHMAP_SIZE          1024
#define H32_BASE              3323198485ul
#define H64_BASE              525201411107845655ull
#define H32_MUL               0x5bd1e995
#define H64_MUL               0x5bd1e9955bd1e995

#define DELETE_H(k,h)         memset(ADDRESS_H(k,h),0,sizeof(void*))
#define INSERT_H(k,h,x)       NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(void*)))
#define ADDRESS_H(k,h)        h[k % HASHMAP_SIZE]
#define INSPECT_H(k,h)        *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)        INSPECT_H(k,h) ? true : false 

// hash
uint32_t h32(string key) 
{
    // MurmurOAAT32 hash
    uint32_t h = H32_BASE;
    for (;*key;++key) {
	h^=*key;h*=H32_MUL;h^=h>>15;
    }
    return h;
}

uint64_t h64(string key) 
{
    // MurmurOAAT64 hash
    uint64_t h = H64_BASE;
    for (;*key;++key) {
	h^=*key;h*=H64_MUL;h^=h>>47;
    }
    return h;
}

// Regex
#define EPSILON               0x00000000

#define MLSTATUS(x)           (x ? RUNNING : (Monolith.status=ERROR)) 

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


#define RED                   "\x1B[31m"
#define LIM                   "\x1B[32m"
#define YEL                   "\x1B[33m"
#define BLU                   "\x1B[34m"
#define MAG                   "\x1B[35m"
#define CYN                   "\x1B[36m"
#define WHT                   "\x1B[37m"
#define GRN                   "\033[92m"
#define LRED                  "\033[91m"
#define RESET                 "\x1B[0m"

#define PRT_RED               printf(RED)
#define PRT_LRED              printf(LRED)
#define PRT_GRN               printf(GRN)
#define PRT_YEL               printf(YEL)
#define PRT_BLU               printf(BLU)
#define PRT_MAG               printf(YEL)
#define PRT_CYN               printf(CYN)
#define PRT_WHT               printf(WHT)
#define PRT_LIM               printf(LIM)
#define PRT_RESET             printf(RESET)

#define PRT_SPACE             printf(" ")
#define PRT_TAB               printf("\t")
#define PRT_NL                printf("\n")
#define PRT(x)                printf(YEL x RESET)
#define RARROW                PRT(" -> ") 
#define LARROW                PRT(" <- ") 

#define CHAR_REPR(x)          (x < 0x20 || x > 0x7E) ? 0x2E : x
#define PRT_PTR(x)            (x ? printf(LRED "%p" RESET,x) : printf(LRED "0x000000000000" RESET));
#define PRT_HEX(x)            printf(BLU "%02x" RESET, *((byte*)x));
#define PRT_CHAR(x)           printf(GRN "%c"   RESET, CHAR_REPR(*((byte*)x)));

#define PRT_BUFFER(x,n)       for(int i=0;i<n;i++){ PRT_CHAR( ((byte*)x+i) );            }  
#define PRT_HEX_RANGE(x,n)    for(int i=0;i<n;i++){ PRT_HEX ( ((byte*)x+i) ); PRT_SPACE; }  

#define PRT_U64(x)            printf(GRN "%llu" RESET, *((uint64_t*)x));
#define PRT_I64(x)            printf(GRN "%lld" RESET, *((int64_t *)x));

#define PRT_HEXDUMP(x,n)      PRT_PTR(x); PRT_SPACE; PRT_HEX_RANGE(x,n); PRT_BUFFER(x,n);

#define PRT_VERTEX(x)         PRT_TAB; PRT("vertex at "); PRT_HEXDUMP(x, SIZEOF_VERTEX); PRT_SPACE; \
                                       PRT("ptr=");     PRT_PTR( TRIGON(x) );            PRT_SPACE; \
                                       PRT("u64=");     PRT_U64(x);                      PRT_SPACE; \
                                       PRT("i64=");     PRT_I64(x);                      PRT_NL;

#define PRT_TRIGON(x)         PRT("TRIGON at "); PRT_PTR(x) PRT_NL; \
                              PRT("[X]"); PRT_VERTEX(TRIGON_X(x)); \
                              PRT("[Y]"); PRT_VERTEX(TRIGON_Y(x)); \
                              PRT("[Z]"); PRT_VERTEX(TRIGON_Z(x));

#define PRT_LIST(L)           PRT("List: \n"); for(Trigon T=TRIGON_HEAD(L);T!=NULL;T=TRIGON_NEXT(T)) {PRT_TRIGON(T)}

// Trigon

typedef void*                 Trigon;

#define TRIGON(_)             (*((void**)   _))
#define I64(_)                (*((int64_t *)_))
#define U64(_)                (*((uint64_t*)_))

#define SIZEOF_VERTEX         8
#define SIZEOF_TRIGON         SIZEOF_VERTEX * 3 
#define TRIGON_MALLOC         calloc(1, SIZEOF_TRIGON)

#define X_OFFSET              0
#define Y_OFFSET              8
#define Z_OFFSET              16

#define TRIGON_X(T)           (T + X_OFFSET)
#define TRIGON_Y(T)           (T + Y_OFFSET)
#define TRIGON_Z(T)           (T + Z_OFFSET)



#define TRIGON_SET_V(V,_)     memcpy(V, _, SIZEOF_VERTEX)
#define TRIGON_SET_X(T,_)     TRIGON_SET_V(TRIGON_X(T), _)
#define TRIGON_SET_Y(T,_)     TRIGON_SET_V(TRIGON_Y(T), _) 
#define TRIGON_SET_Z(T,_)     TRIGON_SET_V(TRIGON_Z(T), _)
#define TRIGON_SET_E(V,X)     TRIGON_SET_V(V, &X)
#define TRIGON_SET(T,x,y,z)   TRIGON_SET_X(T,x); TRIGON_SET_Y(T,y); TRIGON_SET_Z(T,z);

#define TRIGON_CLR_V(V)       memset(V, 0x00, SIZEOF_VERTEX)

#define TRIGON_NEXT(T)        TRIGON(TRIGON_Y(T))
#define TRIGON_PREV(T)        TRIGON(TRIGON_X(T))
#define TRIGON_HEAD(L)        TRIGON(TRIGON_X(L))
#define TRIGON_TAIL(L)        TRIGON(TRIGON_Y(L))

#define TRIGON_ADD_BACK(L,T)  TRIGON_SET_E(TRIGON_X(T), TRIGON_TAIL(L));  \
			      TRIGON_SET_E(TRIGON_Y(TRIGON_TAIL(L)), T);  \
			      TRIGON_SET_E(TRIGON_Y(L), T);     \
                              U64 ( TRIGON_Z(L) )++;
#define TRIGON_DEL_BACK(L)    Trigon TAIL = TRIGON_TAIL(L); \
			      TRIGON_SET_E(TRIGON_Y(L), TRIGON_PREV(TAIL) ); \
			      TRIGON_CLR_V(TRIGON_Y(TRIGON_PREV(TAIL)));   \
			      TRIGON_CLR_V(TRIGON_X(TAIL));   \
                              U64(TRIGON_Z(L))--;

#define TRIGON_IS_EMPTY(T)    ( U64(TRIGON_Y(T)) && U64(TRIGON_X(T)) ) ? 0 : 1
#define TRIGON_IS_SINGLE(T)   ( U64(TRIGON_Y(T)) == U64(TRIGON_X(T)) ) ? 1 : 0

#define TRIGON_ADD_EMPTY(L,T) TRIGON_SET_E(TRIGON_X(L), T); TRIGON_SET_E(TRIGON_Y(L), T); U64(TRIGON_Z(L))++;
#define TRIGON_DEL_LIST(L)    TRIGON_CLR_V(TRIGON_X(L)); TRIGON_CLR_V(TRIGON_Y(L)); U64(TRIGON_Z(L))--;
#define TRIGON_PUSH(L,T)      if ( TRIGON_IS_EMPTY(L) ) { TRIGON_ADD_EMPTY(L,T) } else { TRIGON_ADD_BACK(L,T) }
#define TRIGON_POP(L)         if ( !TRIGON_IS_EMPTY(L) ) { if (TRIGON_IS_SINGLE(L)) {TRIGON_DEL_LIST(L)} else {TRIGON_DEL_BACK(L)} } 
 




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

    Trigon L = TRIGON_MALLOC;
    Trigon A = TRIGON_MALLOC;
    Trigon B = TRIGON_MALLOC;
    
    TRIGON_PUSH(L, A);
    TRIGON_PUSH(L, B);
    PRT_LIST(L);

    //PRT_TRIGON(L);
    //PRT_LIST(L);

    
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
