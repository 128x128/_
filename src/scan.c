#include "../include/node.h"
#include "../include/recognizer.h"

int main(int argc, char **argv) {
    buffer* _ = balloc(NODE);
    bpush(_, nstring("1"));
    bpush(_, nstring("2"));
    bpush(_, nstring("3"));
    node* v = regex_union(_);
    printb(v->data);
    return 0; 
}
