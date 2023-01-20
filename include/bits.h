#ifndef BITS_H
#define BITS_H

#include "common.h"

void hexdump(void* addr, int len) {
    int perLine = 16; int i;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;
    for (i = 0; i < len; i++) {
	printf (" %02x", pc[i]);
	if ((pc[i] < 0x20) || (pc[i] > 0x7e))
	    buff[i % perLine] = '.';
	else
	    buff[i % perLine] = pc[i];
	buff[(i % perLine) + 1] = '\0';
    }
    printf ("  %s\n", buff);
}

#endif
