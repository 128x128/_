#ifndef TYPES_H
#define TYPES_H 

#define BYTE_SIZE 1
#define WORD_SIZE 2
#define LONG_SIZE 4
#define QUAD_SIZE 8


typedef enum TYPE {
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
    STR,
    PTR,
    BUFF,
    NODE,
} TYPE;

typedef struct dtype {
    TYPE id;
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
//void printbuf(void* _) {printb((buffer*)_);}
//void printnode(void* _) {printn((node*)_);}

dtype uint8   = {U8,   BYTE_SIZE, "uint8",  NULL}; 
dtype int8    = {I8,   BYTE_SIZE, "int8",   NULL}; 
dtype uint16  = {U16,  WORD_SIZE, "uint16", NULL}; 
dtype int16   = {I16,  WORD_SIZE, "int16",  NULL}; 
dtype uint32  = {U32,  LONG_SIZE, "uint32", NULL}; 
dtype int32   = {I32,  LONG_SIZE, "int32",  NULL}; 
dtype uint64  = {U64,  QUAD_SIZE, "uint64", NULL}; 
dtype int64   = {I64,  QUAD_SIZE, "int64",  NULL}; 
dtype fp16    = {FP16, WORD_SIZE, "fp16",   NULL}; 
dtype fp32    = {FP32, LONG_SIZE, "fp32",   NULL}; 
dtype fp64    = {FP64, QUAD_SIZE, "fp64",   NULL}; 
dtype String  = {STR,  BYTE_SIZE, "str",    NULL}; 
dtype Pointer = {PTR,  QUAD_SIZE, "ptr",    NULL}; 
dtype Buffer  = {BUFF, QUAD_SIZE, "buffer", NULL}; 
dtype Node    = {NODE, QUAD_SIZE, "node",   NULL}; 


dtype* tmap(TYPE _) {
    switch(_) {
	case U8:   return &uint8;
	case I8:   return &int8;
	case U16:  return &uint16;
	case I16:  return &int16;
	case U32:  return &uint32;
	case I32:  return &int32;
	case U64:  return &uint64;
	case I64:  return &int64;
	case FP16: return &fp16;
	case FP32: return &fp32;
	case FP64: return &fp64;
	case STR:  return &String;
	case PTR:  return &Pointer;
	case BUFF: return &Buffer;
	case NODE: return &Node;
	default: NULL;
    }
} 

#endif
