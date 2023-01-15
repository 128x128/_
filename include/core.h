#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

//#include <curses.h>

#define _1B_ 1
#define _2B_ 2
#define _4B_ 4
#define _8B_ 8

typedef enum dtype_id {
    _u8_,
    _i8_,
    _u16_,
    _i16_,
    _u32_,
    _i32_,
    _u64_,
    _i64_,
    _f8_,
    _f16_,
    _f32_,
    _f64_,
    _str_,
    _ptr_,
    _buff_,
    _node_,
} dtype_id;

typedef struct dtype {
    dtype_id id;
    size_t size;
    char str[64];
    void (*print)(void* _);
    void* (*init)(struct dtype);
} dtype;

typedef struct buffer {
    void* data;
    dtype type;
    size_t size;
    size_t used;
    size_t bytes;
} buffer;

typedef struct node {
    void* data;
    dtype type;
    buffer* in;
    buffer* out;
} node;

void printb(buffer* _) {
    printf("Buffer: dtype=%s items=%zu using %zu bytes out of %zu bytes\n", _->type.str, _->size, _->used, _->bytes);
}

void printn(node* _) {
    printf("Node: dtype=%s \n\t", _->type.str);
    if (_->data) _->type.print(_->data);
}


void printu8(void* _) {printf("%d", *(uint8_t*)_);}
void printi8(void* _) {printf("%d", *(int8_t*)_);}
void printu16(void* _) {printf("%d", *(uint16_t*)_);}
void printi16(void* _) {printf("%d", *(int16_t*)_);}
void printu32(void* _) {printf("%d", *(uint32_t*)_);}
void printi32(void* _) {printf("%d", *(int32_t*)_);}
void printu64(void* _) {printf("%llu", *(uint64_t*)_);}
void printi64(void* _) {printf("%lld", *(int64_t*)_);}
void printstr(void* _) {printf("%s", (char*)_);}
void printptr(void* _) {printf("%p", _);}
void printbuf(void* _) {printb((buffer*)_);}
void printnode(void* _) {printn((node*)_);}

dtype U8   = {_u8_,   _1B_, "uint8", &printu8}; 
dtype I8   = {_i8_,   _1B_, "int8", &printi8}; 
dtype U16  = {_u16_,  _2B_, "uint16", &printu16}; 
dtype I16  = {_i16_,  _2B_, "int16", &printi16}; 
dtype U32  = {_u32_,  _4B_, "uint32", &printu32}; 
dtype I32  = {_i32_,  _4B_, "int32", &printi32}; 
dtype U64  = {_u64_,  _8B_, "uint64", &printu64}; 
dtype I64  = {_i64_,  _8B_, "int64", &printi64}; 
//dtype F8   = {_f8_,   _1B_, "float8"}; 
//dtype F16  = {_f16_,  _2B_, "float16"}; 
//dtype F32  = {_f32_,  _4B_, "float32"}; 
//dtype F64  = {_f64_,  _8B_, "float64"}; 
dtype Str  = {_str_,  _8B_, "str", &printstr}; 
dtype Ptr  = {_str_,  _8B_, "ptr", &printptr}; 
dtype Buff = {_buff_, sizeof(buffer), "buffer", &printbuf}; 
dtype Node = {_node_, sizeof(node),  "node", &printnode}; 

// alloc & delloc ops

buffer* balloc(struct dtype _) {
    buffer* b = (buffer*)malloc(sizeof(buffer));
    b->data = malloc(_.size);
    b->type = _;
    b->bytes = _.size;
    b->used = 0;
    b->size = 0;
    return b;
}

node* nalloc(struct dtype _) {
    node* n = (node*)malloc(sizeof(node));
    n->type = _;
    n->in = balloc(Ptr);
    n->out = balloc(Ptr);
    return n;
}

void bdelloc(buffer* _) {
    if (_->data) free(_->data); free(_);
}

void ndelloc(node* _) {
    if (_->data) free(_->data); free(_);
    bdelloc(_->in); bdelloc(_->out);
}

// buffer ops

void brelloc(buffer* _) { // 2x resize
    void* __ = malloc(_->bytes*2);
    memcpy(__, _->data, _->bytes);
    _->bytes *= 2;
    free(_->data); 
    _->data = __;
}

void bpush(buffer* _, void* __) {
    if (_->used == _->bytes) brelloc(_);
    memcpy(_->data+_->used, __, _->type.size);
    _->size++; _->used += _->type.size;
}

void bpop(buffer* _) {_->size--; _->used -= _->type.size;}

// node ops

void direct(node* x, node* y) {
    void** _x = (void**)&x;
    void** _y = (void**)&y;
    bpush(x->out, (void*)_y); bpush(y->in, (void*)_x);
}

void undirect(node* x, node* y) {
    direct(x, y); direct(y, x);
}


//void dlistpush(dlist* x, const char *c) {
    //node* s = node(c);
    //if (x->head==NULL) {
	//x->head = s;
	//x->tail = s;
	//return;
    //}
    //node* t = x->tail;
    //s->prev = t;
    //t->next = s;
    //x->tail = s;
//}

//void dlistpop(dlist* x) {
    //if (x->head==NULL) return;
    //if (x->head==x->tail) {
	//dlistdelloc(x->head);
	//x->head = NULL;
	//x->tail = NULL;
	//return;
    //}
    //node* last = x->tail;
    //node* slast = last->prev;
    //if (slast!=NULL) {
	//slast->next = NULL;
	//x->tail = slast;
	//dlistdelloc(last);
    //}
//}

// helpers

char qckgetchar() { // get char from stdin without enter
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
	    perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
	    perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
	    perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
	    perror ("tcsetattr ~ICANON");
    return (buf);
}

void bhexdump(buffer* x) {
    int perLine = 16; int i;
    int len = x->bytes; void* addr = x->data;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;
    for (i = 0; i < len; i++) {
	if ((i % perLine) == 0) {
	    if (i != 0) printf ("  %s\n", buff);
	    printf ("  %04x ", i);
	}
	printf (" %02x", pc[i]);
	if ((pc[i] < 0x20) || (pc[i] > 0x7e))
	    buff[i % perLine] = '.';
	else
	    buff[i % perLine] = pc[i];
	buff[(i % perLine) + 1] = '\0';
    }
    //while ((i % perLine) != 0) 
	//printf ("   "); 
            //i++;
    printf ("  %s\n", buff);
}

#endif
