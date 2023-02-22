#ifndef BITS_H
#define BITS_H

#include "common.h"

#define MAX_BITS 128

static bool bitBuffer[MAX_BITS];

typedef unsigned char byte;
typedef unsigned char bitpos;
typedef unsigned char width;
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;


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


#define PRT_BUFFER(x,n)       for(int i=0;i<n;i++){ PRT_CHAR( ((byte*)x+i) ); }  
#define PRT_HEX_RANGE(x,n)    for(int i=0;i<n;i++){ PRT_HEX ( ((byte*)x+i) ); PRT_SPACE; }  
#define PRT_HD_RANGE(x,n,m)   for(int j=0;j<m;j++){ PRT_HEXDUMP( ((byte*)x)+(j*n) ,n); PRT_NL; }
#define PRT_HD_INDENT(x,n,m)  for(int j=0;j<m;j++){ PRT_TAB; PRT_HEXDUMP( ((byte*)x)+(j*n) ,n); PRT_NL; }

#define PRT_HEXDUMP(x,n)      PRT_PTR(x); PRT_SPACE; PRT_HEX_RANGE(x,n); PRT_BUFFER(x,n);

void space(){printf(" ");}
void endl(){printf("\n");}
void tab(){printf("\t");}
void prtptr(void* addr) {printf("\033[91m%p\033[0m", addr);}

void prthex(void* addr, int n) 
{
    for (int i = 0; i < n; i++) {
	printf("\033[94m%02x \033[0m", *((unsigned char*)addr+i));
    }
}
void prtchar(void* addr, int n) 
{
    for (int i = 0; i < n; i++) {
	char c = *((unsigned char*)addr+i);
	if (c < 0x20 || c > 0x7e) {c = '.';}
    	printf("\033[92m%c\033[0m", c);
    }
}

void hd(void* x, int n) 
{
    prtptr(x); space(); prthex(x,n); prtchar(x,n); space();
}

void hexdump(void* x, int n) 
{
    prtptr(x); space(); prthex(x,n); prtchar(x,n); endl();
}


// bits
byte* bytecast(void* x){return (byte*)x;}
uint64_t* u64cast(byte* x){return (uint64_t*)x;}
uint128_t* u128cast(byte* x){return (uint128_t*)x;}
// ops
bool rdbit (byte* x,bitpos i){return (*(u128cast(x))>>i)%2;};
void ldbits(byte* x,width n){for(int i=0;i<n;i++){bitBuffer[n-i-1]=rdbit(x, i);}}
//printers
void rarrow(){printf(YEL " >> "RESET);}
void larrow(){printf(YEL " << "RESET);}
void prtBitsGrn(width n){for (int i=0;i<n;i++){printf(GRN"%d"RESET,bitBuffer[i]);}}
void prtBitsRed(width n){for (int i=0;i<n;i++){printf(RED"%d"RESET,bitBuffer[i]);}}
void prtbits(void* x,width n){ldbits(x, n);prtBitsGrn(n);}
void prtdb(byte* x,width n){prtbits(x+n/2,n/2);printf(" ");prtbits(x,n/2);}
void bhd(byte* x,int n){for(int i=0;i<n;i++){hd(x+i,1);rarrow();prtbits(x+i,8);endl();}}

void printu8(void* _) {printf("%d", *(uint8_t*)_);}
void printi8(void* _) {printf("%d", *(int8_t*)_);}
void printu16(void* _) {printf("%d", *(uint16_t*)_);}
void printi16(void* _) {printf("%d", *(int16_t*)_);}
void printu32(void* _) {printf("%d", *(uint32_t*)_);}
void printi32(void* _) {printf("%d", *(int32_t*)_);}
void printu64(void* _) {printf("%llu", *(uint64_t*)_);}
void printi64(void* _) {printf("%lld", *(int64_t*)_);}
void printstr(void* _) {printf("%s", (char*)_);}
void printptr(void* _) {printf("\033[91m%p\033[0m", _);}
#endif
