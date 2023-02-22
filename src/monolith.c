#include "monolith.h"

int main(int argc, char **argv) {
    if (argc == 2) execute(argv[1]);
    else prompt();
    return 0; 
}
