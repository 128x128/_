#ifndef HASHMAP_H 
#define HASHMAP_H

#include <unistd.h>

typedef uint32_t              hash32;
typedef uint64_t              hash64;
typedef char*                 string;
typedef void**                hashmap; 

// hashmap
#define HASHMAP_SIZE          1024
#define H32_BASE              3323198485ul
#define H64_BASE              525201411107845655ull
#define H32_MUL               0x5bd1e995
#define H64_MUL               0x5bd1e9955bd1e995

#define HASHMAP_ALLOC         (hashmap)calloc(1, sizeof(void*) * HASHMAP_SIZE)

hash32 h32(string key) 
{
    // MurmurOAAT32 hash
    hash32 h = H32_BASE; for(;*key;++key) {h^=*key;h*=H32_MUL;h^=h>>15;} return h;
}

hash64 h64(string key) 
{
    // MurmurOAAT64 hash
    hash64 h = H64_BASE; for(;*key;++key) {h^=*key;h*=H64_MUL;h^=h>>47;} return h;
}

hashmap address_h(hashmap H, hash64 h) {
    return H + (h % HASHMAP_SIZE);
}

void* inspect_h(hashmap H, hash64 h) 
{
    return *address_h(H, h);
}

bool isfree_h(hashmap H, hash64 h) 
{
    return inspect_h(H, h) ? false : true;
}

bool insert_h(hashmap H, hash64 h, void* x) 
{
    return isfree_h(H, h) ? !*((int*)memcpy(address_h(H, h), &x, sizeof(void*))) : 1;
}

bool delete_h(hashmap H, hash64 h) 
{
    return memset(address_h(H, h),0,sizeof(void*));
}


#endif
