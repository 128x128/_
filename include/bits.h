#ifndef BITS_H
#define BITS_H

#include "common.h"

static bool BIT_REGISTER[128];

typedef unsigned char byte;
typedef unsigned char bitpos;
typedef unsigned char width;
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void hd(void* addr, int len) {
    int perLine = 16; int i;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;
    printf("\033[91m%p\033[0m ", addr);
    for (i = 0; i < len; i++) {
	printf ("\033[94m %02x \033[0m", pc[i]);
	if ((pc[i] < 0x20) || (pc[i] > 0x7e))
	    buff[i % perLine] = '.';
	else
	    buff[i % perLine] = pc[i];
	buff[(i % perLine) + 1] = '\0';
    }
    printf ("\033[92m  %s\033[0m", buff);
}


// bits
byte* bytecast(void* x){return (byte*)x;}
uint64_t* u64cast(byte* x){return (uint64_t*)x;}
uint128_t* u128cast(byte* x){return (uint128_t*)x;}
// ops
bool rdbit (byte* x,bitpos i){return (*(u128cast(x))>>i)%2;};
void ldbits(byte* x,width n){for(int i=0;i<n;i++){BIT_REGISTER[n-i-1]=rdbit(x, i);}}

void rarrow(){printf(YEL " >> "RESET);}
void larrow(){printf(YEL " << "RESET);}
void endl(){printf("\n");}

void prtBitsGrn(width n){for (int i=0;i<n;i++){printf(GRN"%d"RESET,BIT_REGISTER[i]);}}
void prtBitsRed(width n){for (int i=0;i<n;i++){printf(RED"%d"RESET,BIT_REGISTER[i]);}}
void prtbits(byte* x,width n){ldbits(x, n);prtBitsGrn(n);}
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
