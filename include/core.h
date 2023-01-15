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


typedef struct buffer
{
    void* data;
    void* ptr;
    uint32_t dtype;
    uint64_t tsize;
    uint64_t n;
    uint64_t used;
    uint64_t bytes;
} buffer;

//types
typedef char byte;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

#define _8BITS_  1
#define _16BITS_ 2
#define _32BITS_ 4
#define _64BITS_ 8
#define PAIR     2
#define nTYPES  16

typedef enum dtype 
{
    _i8_,
    _u8_,
    _i16_,
    _u16_,
    _i32_,
    _u32_,
    _i64_,
    _u64_,
    _str_,
} dtype;

byte _dtype_[nTYPES][PAIR] = {
    {_i8_,   _8BITS_},
    {_u8_,   _8BITS_},
    {_i16_, _16BITS_},
    {_u16_, _16BITS_},
    {_i32_, _32BITS_},
    {_u32_, _32BITS_},
    {_i64_, _64BITS_},
    {_u64_, _64BITS_},
    {_str_,  _8BITS_},
};

void bshow(buffer* x) {
    printf("%p\n", x);
    printf("\tbase: %p\n", x->data);
    printf("\tptr:  %p\n", x->ptr);
    printf("\tbytes: [%llu / %llu]\n", x->used, x->bytes);
    printf("\titems: [%llu / %llu]\n", x->n, (x->bytes/x->tsize));
}
 
buffer* balloc(dtype type) {
    buffer* x = (buffer*)malloc(sizeof(buffer));
    x->used = 0; x->n = 0; 
    x->tsize = _dtype_[type][1]; 
    x->bytes = x->tsize; 
    x->data = malloc(x->bytes);
    x->ptr = x->data;
    x->dtype = type;
    return x;
}

void bdelloc(buffer* x) {
    free(x->data); free(x);
}

void brelloc(buffer* x) {
    void* r = malloc(x->bytes*2);
    memcpy(r, x->data, x->bytes);
    x->bytes *= 2;
    free(x->data); 
    x->data = r;
    x->ptr = r+(x->n*x->tsize);
}

void* boffset(buffer* x, i32 i) {
    return x->data + (i * x->tsize);
}

void bpush(buffer* x, void* i) {
    if (x->used == x->bytes) brelloc(x);
    memcpy(x->ptr, i, x->tsize);
    x->n++; x->used += x->tsize;
    x->ptr += x->tsize;
}

void bpop(buffer* x, void* i) {
    x->n--; x->used -= x->tsize;
    x->ptr -= x->tsize;
}

void bhexdump(buffer* x) {
    i32 perLine = 16; i32 i;
    i32 len = x->bytes; void* addr = x->data;
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
    while ((i % perLine) != 0) printf ("   "); i++;
    printf ("  %s\n", buff);
}

buffer* bstr(const char* s) {
    buffer* x = balloc(_str_);
    for (i32 i=0; i<strlen(s); i++) bpush(x, (void*)&s[i]);
    return x;
}

char qckgetchar() {
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




#endif
