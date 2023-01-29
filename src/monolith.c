/*#include "../include/node.h"*/
#include "../include/list.h"

int main(int argc, char **argv) {
    list* l = initList();
    printf("%d\n", isEmpty(l));
    return 0; 
}
