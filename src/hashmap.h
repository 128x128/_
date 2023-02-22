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

#define DELETE_H(k,h)         memset(ADDRESS_H(k,h),0,sizeof(void*))
#define INSERT_H(k,h,x)       NOTFREE_H(k,h) ? ERROR : !*((int*)memcpy(ADDRESS_H(k,h),x,sizeof(void*)))
#define ADDRESS_H(k,h)        h[k % HASHMAP_SIZE]
#define INSPECT_H(k,h)        *(ADDRESS_H(k, h))
#define NOTFREE_H(k,h)        INSPECT_H(k,h) ? true : false 

#define EPSILON               0x00000000
#define MLSTATUS(x)           (x ? RUNNING : (Monolith.status=ERROR)) 

// hash
hash32 h32(string key) 
{
    // MurmurOAAT32 hash
    uint32_t h = H32_BASE;
    for (;*key;++key) {
	h^=*key;h*=H32_MUL;h^=h>>15;
    }
    return h;
}

hash64 h64(string key) 
{
    // MurmurOAAT64 hash
    uint64_t h = H64_BASE;
    for (;*key;++key) {
	h^=*key;h*=H64_MUL;h^=h>>47;
    }
    return h;
}

#endif
