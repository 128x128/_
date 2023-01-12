#include "../include/core.h"
#include "../include/lexer.h"


int main(int argc, char **argv) { 
    _* x = alloc(NULL, _i32_);
    i32 y;
    push(x, (void*)&y);
    push(x, (void*)&y);
    push(x, (void*)&y);
    show(x);
    return 0; 
}
