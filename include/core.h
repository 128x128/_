#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

//main struct
typedef struct _ 
{
    void* data;
    void* ptr;
    uint32_t dtype;
    uint64_t tsize;
    uint64_t n;
    uint64_t used;
    uint64_t bytes;
} _;

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
#define BASE    32

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

void show(_* x) {
    printf("%p\n", x);
    printf("\tbase: %p\n", x->data);
    printf("\tptr:  %p\n", x->ptr);
    printf("\tbytes: [%llu / %llu]\n", x->used, x->bytes);
    printf("\titems: [%llu / %llu]\n", x->n, (x->bytes/x->tsize));
}
 
_* alloc(void* data, dtype type) {
    _* x = (_*)malloc(sizeof(_));
    x->used = 0; x->n = 0; 
    x->tsize = _dtype_[type][1]; 
    x->bytes = BASE * x->tsize; 
    x->data = malloc(x->bytes);
    x->ptr = x->data;
    x->dtype = type;
    if (data != NULL) memcpy(x->data, data, x->bytes);
    return x;
}

void delloc(_* x) {
    free(x->data); free(x);
}


void relloc(_* x) {
    void* r = malloc(x->bytes*2);
    memcpy(r, x->data, x->bytes);
    x->bytes *= 2;
    free(x->data); 
    x->data = r;
}

void* offset(_* x, i32 i) {
    return x->data + (i * x->tsize);
}

void push(_* x, void* i) {
    if (x->used == x->bytes) relloc(x);
    memcpy(x->ptr, x->data, x->tsize);
    x->n++; x->used += x->tsize;
    x->ptr += x->tsize;
}

void pop(_* x, void* i) {
    x->n--; x->used -= x->tsize;
    x->ptr -= x->tsize;
}


#endif
