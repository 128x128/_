#include "../include/core.h"
#include "../include/lexer.h"


int main(int argc, char **argv) { 
    /*char c = qckgetchar();*/
    node* a = nalloc(I32);
    node* b = nalloc(I32);
    undirect(a,b);
    printn(a);
    printn(b);
    return 0; 
}
