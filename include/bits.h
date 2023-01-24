#ifndef BITS_H
#define BITS_H

#include "common.h"

void hexdump(void* addr, int len) {
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
    printf ("\033[92m  %s\n\033[0m", buff);
}

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
