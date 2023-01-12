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
    uint32_t dtype;
    uint64_t size;
    uint64_t length;
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

typedef enum dtype 
{
    _i8_
    _u8_
    _i16_
    _u16_
    _i32_
    _u32_
    _i64_
    _u64_
    _char_
} dtype;
 
_* init(void* data, u64 size, u64 length, dtype type)
{
    _* x = (_*)malloc(sizeof(_));
    x->bytes = length * size;
    x->dtype = type;
    memcpy(x->data, data, x->bytes);
    return x;
}


#endif
