#ifndef MONOLITH 
#define MONOLITH

#include "common.h"

typedef const char string; 
typedef unsigned char byte;
typedef unsigned char bitpos;
typedef unsigned char bitwidth;
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

static bool BITS[128];

typedef struct monolith {
    void*  structure;
    size_t size;
} monolith;


monolith* Monolith() {return (monolith*)malloc(sizeof(monolith));}
//monolith* M = Monolith();

void initStructure(monolith* m, size_t n) {m->structure = malloc(n);}
void freeStructure(monolith* m) {free(m->structure);}

// bits
void printbits(bitwidth n){for (int i=0;i<n;i++){printf(GRN"%d"RESET,BITS[i]);}}
bool rdbit (void* x, bitpos i){return ((*(uint128_t*)x)>>i)%2;};
void ldbits(void* x, bitwidth n){for(int i=0;i<n;i++){BITS[n-i-1]=rdbit(x, i);}}
void prtbits(void* x, bitwidth n){ldbits(x, n); printbits(n);}

#endif
