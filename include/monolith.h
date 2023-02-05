#ifndef MONOLITH 
#define MONOLITH

#include "common.h"
#include "list.h"
#include "token.h"

#define ERROR 1

typedef const char filename;

typedef struct monolith {
    bool error;
    int (*run)(char*);
    int (*execute)(filename*);
    int (*prompt)();
} monolith;

typedef struct scanner {
    int start;
    int current;
    int line;
} scanner;

int run(char* src);
int execute(const char* file);
int prompt();

monolith Monolith = {
    !ERROR,
    &run,
    &execute,
    &prompt,
};

list* scanTokens(const char* src) {
    return NULL;
}
int run(char* src) { 
    list* tokens = scanTokens(src); return 0;
}
void report(int line, char* where, char* msg) {
    printf("[Line: %d] Error %s :%s", line, where, msg);
}
void error(int line, char* msg) {
    report(line, "", msg);
}
int execute(filename* file) {
    FILE *f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    char* src = (char*)malloc(n+1);
    fseek(f, 0, SEEK_SET);
    fread(src, n, 1, f);
    run(src);
    return fclose(f);
}
int prompt() {
    char buffer[4096];
    while (1) {
	scanf("> %s", &buffer[0]);
	run(&buffer[0]);
	if (Monolith.error) exit(ERROR);
    }
    return 0;
}


#endif
