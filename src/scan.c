/*#include "../include/node.h"*/
#include "../include/monolith.h"

int main(int argc, char **argv) {
    uint64_t x = 1;
    for (int i=0; i<16;i++) {
	printf("x=%lu\n", x);
    	prtdb((byte*)&x, 32);
	printf("\n");
    	bhd((byte*)&x, 8);
	x *= 2;
    }
    /*prtdb((byte*)&x, 32);*/
    return 0; 
}


/*0000000000000000 0000000000000001*/
/*0000000000000000 0000000000000000 >> 1*/
