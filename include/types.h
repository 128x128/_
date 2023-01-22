#ifndef TYPES_H
#define TYPES_H 

#define BYTE_SIZE 1
#define WORD_SIZE 2
#define LONG_SIZE 4
#define QUAD_SIZE 8


typedef enum type {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    U64,
    I64,
    FP16,
    FP32,
    FP64,
    CHAR,
    STRING,
    POINTER,
    BUFFER,
    NODE,
} type;

typedef struct dtype {
    type id;
    size_t size;
    char str[32];
    void (*print)(void* _);
    void* (*init)(struct dtype);
} dtype;

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

dtype dt_uint8   = {U8,      BYTE_SIZE, "uint8",  NULL}; 
dtype dt_int8    = {I8,      BYTE_SIZE, "int8",   NULL}; 
dtype dt_uint16  = {U16,     WORD_SIZE, "uint16", NULL}; 
dtype dt_int16   = {I16,     WORD_SIZE, "int16",  NULL}; 
dtype dt_uint32  = {U32,     LONG_SIZE, "uint32", NULL}; 
dtype dt_int32   = {I32,     LONG_SIZE, "int32",  NULL}; 
dtype dt_uint64  = {U64,     QUAD_SIZE, "uint64", NULL}; 
dtype dt_int64   = {I64,     QUAD_SIZE, "int64",  NULL}; 
dtype dt_fp16    = {FP16,    WORD_SIZE, "fp16",   NULL}; 
dtype dt_fp32    = {FP32,    LONG_SIZE, "fp32",   NULL}; 
dtype dt_fp64    = {FP64,    QUAD_SIZE, "fp64",   NULL}; 
dtype dt_char    = {CHAR,    BYTE_SIZE, "char",   NULL}; 
dtype dt_string  = {STRING,  BYTE_SIZE, "str",    NULL}; 
dtype dt_pointer = {POINTER, QUAD_SIZE, "ptr",    NULL}; 
dtype dt_buffer  = {BUFFER,  QUAD_SIZE, "buffer", NULL}; 
dtype dt_node    = {NODE,    QUAD_SIZE, "node",   NULL}; 


dtype* typemap(type _) {
    switch(_) {
	case U8:      return &dt_uint8;
	case I8:      return &dt_int8;
	case U16:     return &dt_uint16;
	case I16:     return &dt_int16;
	case U32:     return &dt_uint32;
	case I32:     return &dt_int32;
	case U64:     return &dt_uint64;
	case I64:     return &dt_int64;
	case FP16:    return &dt_fp16;
	case FP32:    return &dt_fp32;
	case FP64:    return &dt_fp64;
	case CHAR:    return &dt_char;
	case STRING:  return &dt_string;
	case POINTER: return &dt_pointer;
	case BUFFER:  return &dt_buffer;
	case NODE:    return &dt_node;
	default: NULL;
    }
} 

#endif
