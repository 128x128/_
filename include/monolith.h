#ifndef MONOLITH 
#define MONOLITH

#include "common.h"

typedef const char string; 
typedef unsigned char byte;
typedef unsigned char bitpos;
typedef unsigned char width;
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

static bool BITS[128];

typedef struct monolith {
    byte*  structure;
    size_t size;
} monolith;


monolith* Monolith() {return (monolith*)malloc(sizeof(monolith));}
//monolith* M = Monolith();

void initStructure(monolith* m, size_t n) {m->structure = malloc(n);}
void freeStructure(monolith* m) {free(m->structure);}

// bits
byte* bc(void* x){return (byte*)x;}
uint64_t* u64c(byte* x){return (uint64_t*)x;}
uint128_t* u128c(byte* x){return (uint128_t*)x;}
// ops
bool rdbit (byte* x,bitpos i){return (*(u128c(x))>>i)%2;};
void ldbits(byte* x,width n){for(int i=0;i<n;i++){BITS[n-i-1]=rdbit(x, i);}}
// printers
void ra(){printf(YEL" >> "RESET);}
void la(){printf(YEL" << "RESET);}
void endl(){printf("\n");}
void prtBitsGrn(width n){for (int i=0;i<n;i++){printf(GRN"%d"RESET,BITS[i]);}}
void prtBitsRed(width n){for (int i=0;i<n;i++){printf(RED"%d"RESET,BITS[i]);}}
void prtbits(byte* x, width n){ldbits(x, n);prtBitsGrn(n);}
void prtdb(byte* x, width n){prtbits(x+n/2,n/2);printf(" ");prtbits(x,n/2);}
//void prtqd(byte* x, width n){prtdb(x,n/2);printf(" ");prtdb(x+n/2,n/2);}

void bhd(byte* x,int n){for(int i=0;i<n;i++){hd(x+i,1);ra();prtbits(x+i,8);endl();}}
#endif
