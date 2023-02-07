#ifndef MONOLITH 
#define MONOLITH

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

//#include "common.h"
//#include "list.h"
//#include "token.h"
//#include "node.h"

#define MAX_CHARS 4096

typedef char src;
typedef const char filename;

typedef enum mlStatus {SUCCESS=-1, RUNNING=0, ERROR=1} mlStatus;

#define mlSTATUS(x) (x ? RUNNING : (Monolith.status=ERROR)) 


typedef struct monolith {
    mlStatus  status;
    mlStatus  (*run)     (src*);
    mlStatus  (*execute) (filename*);
    mlStatus  (*prompt)  ();
    src       promptBuffer[MAX_CHARS];
} monolith;

mlStatus run     (src* code);
mlStatus execute (filename* file);
mlStatus prompt  ();

monolith Monolith = {
    RUNNING,
    &run,
    &execute,
    &prompt,
};


//list* scanTokens(const char* src) {
    //buffer* re = initStringBuffer("(s|[a-z]|j(eee)*)");
    //dfa* g = initDFA(re);
    //return NULL;
//}
void report(int line, char* where, char* msg) {
    printf("[Line: %d] Error %s :%s", line, where, msg);
}
void error(int line, char* msg) {
    report(line, "", msg);
}
// Main
mlStatus run(src* code) { 
    return 0;
    //list* tokens = scanTokens(src); return 0;
}
mlStatus execute(filename* file) {
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    src* code = (src*)malloc(n+1);
    fseek(f, 0, SEEK_SET);
    fread(code, n, 1, f);
    fclose(f);
    return mlSTATUS(run(code));
}
mlStatus runbuffer(){
    return mlSTATUS(run(&Monolith.promptBuffer[0]));
}
mlStatus scan(){
    char promptBuffer[MAX_CHARS];
    scanf("> %s",&promptBuffer[0]);
    //printf("%s\n",&promptBuffer[0]);
    return 0;}
    //return runbuffer();}
mlStatus prompt(){while(true){scan();} return ERROR;}


#endif
