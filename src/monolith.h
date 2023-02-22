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

#include "alloc.h"
#include "print.h"
#include "hashmap.h"
#include "regexp.h"

typedef unsigned char         byte;
typedef const char            filename;
typedef FILE*                 fileptr;
typedef void*                 Trigon;

typedef enum mlStatus 
{
    SUCCESS = -1, 
    RUNNING =  0, 
    ERROR   =  1,
} 
mlStatus;


#define BUFFER_SIZE           4096

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


///////////////////////////////////////////////////////////////////////////////
//                                Trigon                                     //
///////////////////////////////////////////////////////////////////////////////

#define TRIGON(_)             (*((void**)   _))
#define I64(_)                (*((int64_t *)_))
#define U64(_)                (*((uint64_t*)_))

#define SIZEOF_VERTEX         8
#define SIZEOF_TRIGON         SIZEOF_VERTEX * 3 
#define TRIGON_MALLOC         calloc(1, SIZEOF_TRIGON)

#define TRIGON_X(T)           (T + 0x00)
#define TRIGON_Y(T)           (T + 0x08)
#define TRIGON_Z(T)           (T + 0x10)

#define TRIGON_SET_V(V,_)     memcpy(V, _, SIZEOF_VERTEX)
#define TRIGON_SET_X(T,_)     TRIGON_SET_V(TRIGON_X(T), _)
#define TRIGON_SET_Y(T,_)     TRIGON_SET_V(TRIGON_Y(T), _) 
#define TRIGON_SET_Z(T,_)     TRIGON_SET_V(TRIGON_Z(T), _)
#define TRIGON_SET_E(V,X)     TRIGON_SET_V(V, &X)
#define TRIGON_SET(T,x,y,z)   TRIGON_SET_X(T,x);                               \
                              TRIGON_SET_Y(T,y);                               \
                              TRIGON_SET_Z(T,z);

#define TRIGON_CLR_V(V)       memset(V, 0x00, SIZEOF_VERTEX)

#define TRIGON_NEXT(T)        TRIGON(TRIGON_Y(T))
#define TRIGON_PREV(T)        TRIGON(TRIGON_X(T))
#define TRIGON_HEAD(L)        TRIGON(TRIGON_X(L))
#define TRIGON_TAIL(L)        TRIGON(TRIGON_Y(L))

#define TRIGON_ADD_BACK(L,T)  TRIGON_SET_E(TRIGON_X(T), TRIGON_TAIL(L));       \
			      TRIGON_SET_E(TRIGON_Y(TRIGON_TAIL(L)), T);       \
			      TRIGON_SET_E(TRIGON_Y(L), T);                    \
                              U64 ( TRIGON_Z(L) )++;
#define TRIGON_ADD_FRNT(L,T)  TRIGON_SET_E(TRIGON_Y(T), TRIGON_HEAD(L));       \
			      TRIGON_SET_E(TRIGON_X(TRIGON_HEAD(L)), T);       \
			      TRIGON_SET_E(TRIGON_X(L), T);                    \
                              U64 ( TRIGON_Z(L) )++;
#define TRIGON_DEL_BACK(L)    Trigon TAIL = TRIGON_TAIL(L);                    \
			      TRIGON_SET_E(TRIGON_Y(L), TRIGON_PREV(TAIL) );   \
			      TRIGON_CLR_V(TRIGON_Y(TRIGON_PREV(TAIL)));       \
			      TRIGON_CLR_V(TRIGON_X(TAIL));   \
                              U64(TRIGON_Z(L))--;
#define TRIGON_DEL_FRNT(L)    Trigon HEAD = TRIGON_HEAD(L);                    \
			      TRIGON_SET_E(TRIGON_X(L), TRIGON_NEXT(HEAD) );   \
			      TRIGON_CLR_V(TRIGON_X(TRIGON_NEXT(HEAD)));       \
			      TRIGON_CLR_V(TRIGON_Y(HEAD));                    \
                              U64(TRIGON_Z(L))--;

#define TRIGON_IS_EMPTY(T)    ( U64(TRIGON_Y(T)) && U64(TRIGON_X(T)) ) ? 0 : 1
#define TRIGON_IS_SINGLE(T)   ( U64(TRIGON_Y(T)) == U64(TRIGON_X(T)) ) ? 1 : 0

#define TRIGON_ADD_EMPTY(L,T) TRIGON_SET_E(TRIGON_X(L), T);                    \
                              TRIGON_SET_E(TRIGON_Y(L), T);                    \
                              U64(TRIGON_Z(L))++;
#define TRIGON_DEL_LIST(L)    TRIGON_CLR_V(TRIGON_X(L));                       \
                              TRIGON_CLR_V(TRIGON_Y(L));                       \
                              U64(TRIGON_Z(L))--;
#define PUSH(L,T)             if   ( TRIGON_IS_EMPTY(L)  ) {                   \
                                     TRIGON_ADD_EMPTY(L,T) }                   \
                              else { TRIGON_ADD_BACK(L,T ) }
#define POP(L)                if   ( !TRIGON_IS_EMPTY(L) ) {                   \
                              if   ( TRIGON_IS_SINGLE(L) ) {                   \
				     TRIGON_DEL_LIST(L)  }                     \
			      else { TRIGON_DEL_BACK(L)  } } 
#define ENQUEUE(L,T)          if   ( TRIGON_IS_EMPTY(L)  ) {                   \
                                     TRIGON_ADD_EMPTY(L,T) }                   \
                              else { TRIGON_ADD_FRNT(L,T ) }
#define DEQUEUE(L)            if   ( !TRIGON_IS_EMPTY(L) ) {                   \
                              if   ( TRIGON_IS_SINGLE(L) ) {                   \
				     TRIGON_DEL_LIST(L)  }                     \
                              else { TRIGON_DEL_FRNT(L)  } } 




// Main

void run() 
{

    Trigon L = TRIGON_MALLOC;
    Trigon A = TRIGON_MALLOC;
    Trigon B = TRIGON_MALLOC;
    
    //ENQUEUE(L, A);
    //ENQUEUE(L, B);
    //DEQUEUE(L);
    //PLIST(L);

    
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
