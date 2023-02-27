#ifndef PRINT_H 
#define PRINT_H

#include "trigon.h"

///////////////////////////////////////////////////////////////////////////////
//                                Print                                      //
///////////////////////////////////////////////////////////////////////////////

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

#define PRED                  printf(RED)
#define PLRED                 printf(LRED)
#define PGRN                  printf(GRN)
#define PYEL                  printf(YEL)
#define PBLU                  printf(BLU)
#define PMAG                  printf(YEL)
#define PCYN                  printf(CYN)
#define PWHT                  printf(WHT)
#define PLIM                  printf(LIM)
#define PRESET                printf(RESET)

#define PSPACE                printf(" ")
#define PTAB                  printf("\t")
#define PNL                   printf("\n")
#define PCOM                  printf(",")
#define PRT(x)                printf(YEL x RESET)
#define RARROW                PRT(" -> ") 
#define LARROW                PRT(" <- ") 

#define CHAR_REPR(x)          (x < 0x20 || x > 0x7E) ? 0x2E : x
#define PPTR(x)               x ? printf(LRED "%p" RESET,x) :                  \
                                  printf(LRED "0x000000000000" RESET)
#define PHEX(x)               printf(BLU"%02x"RESET,                           \
	                             *((unsigned char*)x));
#define PCHAR(x)              printf(GRN"%c"RESET,                             \
	                             CHAR_REPR(*((unsigned char*)x)));
#define PSTR(x)               printf(YEL"%s"RESET, x);
#define PBOOL(x)              x ? printf(BLU"True"RESET) : printf(BLU"False"RESET)

#define PBUFFER(x,n)          for(int i=0;i<n;i++){                            \
                              PCHAR( ((unsigned char*)x+i) );}  
#define PHEXBYTE(x,n)         for(int i=0;i<n;i++){                            \
                              PHEX ( ((unsigned char*)x+i) ); PSPACE; }  
#define PHEXDUMP(x,n)         PPTR(x); PSPACE; PHEXBYTE(x,n); PBUFFER(x,n);

#define PU64(x)               printf(GRN "%llu" RESET, *((uint64_t*)x));
#define PI64(x)               printf(GRN "%lld" RESET, *((int64_t *)x));


#define PVERTEX(x)            PTAB; PRT("vertex at ");                         \
		              PHEXDUMP(x, SIZEOF_VERTEX);              PSPACE; \
                              PRT("u64=");     PU64(x);                PSPACE; \
                              PRT("i64=");     PI64(x);                PSPACE; \
                              PRT("ptr=");     PPTR( TRIGON(x) );      PNL; 

#define PTRIGON(x)            PRT("TRIGON at "); PPTR(x); PNL;                 \
                              PRT("[X]"); PVERTEX(TRIGON_X(x));                \
                              PRT("[Y]"); PVERTEX(TRIGON_Y(x));                \
                              PRT("[Z]"); PVERTEX(TRIGON_Z(x));

#define PLIST(L)              PRT("List: \n");                                 \
                              for(Trigon T=TRIGON_HEAD(L);T;T=TRIGON_NEXT(T))  \
		              {PTRIGON(T)}
#endif
