#ifndef BITS_H
#define BITS_H

#include "common.h"

void hexdump(void* addr, int len) {
    int perLine = 16; int i;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;
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

#endif
