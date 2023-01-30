#ifndef HASH_H 
#define HASH_H

#include "common.h"
#include "list.h"

#define HASH_BUCKET_SIZE 1024*1024

uint32_t inline hash32(const char * key) {
    // MurmurOAAT32 hash
    uint32_t h = (3323198485ul);
    for (;*key;++key) {
	h ^= *key;
	h *= 0x5bd1e995;
	h ^= h >> 15;
    }
    return h;
}

uint64_t inline hash64(const char * key) {
    // MurmurOAAT64 hash
    uint64_t h = (525201411107845655ull);
    for (;*key;++key) {
	h ^= *key;
	h *= 0x5bd1e9955bd1e995;
	h ^= h >> 47;
    }
    return h;
}

typedef struct hash {
    void** data[HASH_BUCKET_SIZE];
    size_t size;
} hash;

hash* initHash() {
    hash* h = (hash*)malloc(sizeof(hash));
    h->size = 0;
    return h;
}
void* inspectHash(hash* h, uint32_t key) {
    return h->data[(key % HASH_BUCKET_SIZE)];
}
int insertHash(hash* h, uint32_t key, void* data) {
    if (inspectHash(h, key)) return 1;
    memcpy(&h->data[(key % HASH_BUCKET_SIZE)], &data, sizeof(void*));
    h->size++;
    return 0;
}
int deleteHash(hash* h, uint32_t key) {
    memset(&h->data[(key % HASH_BUCKET_SIZE)], 0, sizeof(void*));
    h->size--;
    return 0;
}

// hashmap

#endif
