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
#include "trigon.h"
#include "hashmap.h"
#include "regexp.h"

typedef const char            filename;
typedef FILE*                 fileptr;

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
    unsigned char  buffer[BUFFER_SIZE]; 
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


// Main

void run() 
{

    //Trigon L = TRIGON_MALLOC;
    //Trigon A = TRIGON_MALLOC;
    //Trigon B = TRIGON_MALLOC;
    
    //ENQUEUE(L, A);
    //PUSH(L, B);
    //PLIST(L);

    
    initNFA("a(b|[c-z])");
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
