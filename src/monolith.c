/*#include "../include/node.h"*/
#include "../include/monolith.h"
#include "../include/hash.h"

int main(int argc, char **argv) {
    if (argc == 2) runFile(argv[1]);
    else prompt();
    return 0; 
}
