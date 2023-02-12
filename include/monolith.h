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

#include "bits.h"
#include "util.h"
#include "types.h"
#include "buffer.h"
#include "list.h"


typedef char*            string;
typedef uint32_t         identifier;
typedef uint32_t         depth;
typedef uint32_t         idx;
typedef uint64_t         hash;
typedef const char       name;
typedef const char       filename;
typedef unsigned char    byte;
typedef unsigned char    kernel;
typedef void**           hashmap; 
typedef uint8_t          u8kernel;
typedef uint32_t         u32kernel; 
typedef uint64_t         u64kernel;
typedef FILE*            fileptr;

typedef enum mlStatus 
{
    SUCCESS = -1, 
    RUNNING =  0, 
    ERROR   =  1,
} 
mlStatus;

typedef enum regexTransform
{
    CONNAT,
    ALTERNATION,
    CLOSURE,
} 
regexTransform;


#define BUFFER_SIZE       4096
#define HASHMAP_SIZE      1024

#define H32_BASE          3323198485ul
#define H64_BASE          525201411107845655ull
#define H32_MUL           0x5bd1e995
#define H64_MUL           0x5bd1e9955bd1e995

// hashmap
#define DELETE_H(k,h)     memset(ADDRESS_H(k,h),0,sizeof(void*))
#define INSERT_H(k,h,x)   NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(void*)))
#define ADDRESS_H(k,h)    h[k % HASHMAP_SIZE]
#define INSPECT_H(k,h)    *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)    INSPECT_H(k,h) ? true : false 

// Regex
#define EPSILON           0xFFFFFFFF

#define MLSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 

typedef struct scanner
{
    byte           buffer[BUFFER_SIZE]; 
    fileptr        fp;
}
scanner;

typedef struct language 
{
    byte           symbol[BUFFER_SIZE]; 
}
language;


typedef struct monolith 
{
    mlStatus       status;
} 
monolith;


// global structs
monolith   Monolith;
scanner    Scanner;
language   Regex;

// hash
uint32_t h32(const char * key) 
{
    // MurmurOAAT32 hash
    uint32_t h = H32_BASE;
    for (;*key;++key){h^=*key;h*=H32_MUL;h^=h>>15;}
    return h;
}

uint64_t h64(const char * key) 
{
    // MurmurOAAT64 hash
    uint64_t h = H64_BASE;
    for (;*key;++key){h^=*key;h*=H64_MUL;h^=h>>47;}
    return h;
}



// Reconginizer

#define L_BRCKT             0x28
#define R_BRCKT             0x29
#define L_RANGE             0x5B
#define R_RANGE             0x5D

#define IS_LBK(x)           (x == L_BRCKT)
#define IS_RBK(x)           (x == R_BRCKT)
#define IS_RAN(x)           (x == L_RANGE)
#define IS_NUM(x)           (x >= '0' && x <= '9')
#define IS_LOW(x)           (x >= 'a' && x <= 'z')
#define IS_UPP(x)           (x >= 'A' && x <= 'Z')
#define IS_SYM(x)           (IS_RAN(x) || IS_NUM(x) || IS_LOW(x) || IS_UPP(x))

#define REGEX_CASE(x)       IS_SYM(x) ? REGEX_OPEN  : REGEX_SYMBOL

hashmap SYMBOL_TABLE[HASHMAP_SIZE]

enum regextype 
{
    REGEX_SYMBOL,
    REGEX_OPEN,
}

idx nextOccurance(string x, byte c) 
{
    string y = x;
    while(*y++!=c){}
    return y-x;
}

string nextSymbol(string x) 
{
    idx          n = (IS_RAN(*x) ? nextOccurance(x, R_RANGE) : 1);
    string* symbol = (string)calloc(n+1, 1); memcpy(symbol, x, n);
    hash         h = h32(symbol);
    INSERT_H(h, &SYMBOL_TABLE, symbol);
    return x + n;
}


pair* regex2NFA(string x)
{
    pair* NFA = PAIR;
    push(NFA, LIST);
    push(NFA, LIST);

    while(*x)
    {
	switch(REGEX_CASE(*x)) 
	{
	    case REGEX_SYMBOL: x = nextSymbol(x);
			       break;
	    case REGEX_OPEN:
		x++;
		break;
	    default: x++;
		     break;
	}
    }
}


// Main
void runScanner() 
{
    // ([0-3]|[b-k])
    // (b|c|([0-3]|[b-k]))
    printf("%d\n", IS_RAN(']'));
    regex2NFA("a(b|c|([0-33]|[b-k]))*");
}

void execute(filename* file)
{
    Scanner.fp = fopen(file, "r");
    while(fgets((char*)&Scanner.buffer[0], BUFFER_SIZE, Scanner.fp)) 
    {
	runScanner();
    }
    fclose(Scanner.fp);
}

void prompt()
{
    while(true) 
    {
	printf(BLU "> " GRN); 
	scanf("%s", &Scanner.buffer[0]); 
	runScanner();
    }
}

void report(int line,char* where,char* msg)
{
    printf("[Line: %d] Error %s :%s",line,where,msg);
}

void error(int line,char* msg)
{
    report(line, "", msg);
}

#endif
