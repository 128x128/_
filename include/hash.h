#ifndef HASH_H 
#define HASH_H

#include "common.h"
#include "list.h"

#define HASHMAP_SIZE 2^16

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

typedef struct hashmap 
{
    void** data[HASHMAP_SIZE];
    size_t size;
} hashmap;

hashmap* initHashmap() {
    hashmap* h = (hashmap*)malloc(sizeof(hashmap));
    h->size = 0;
    return h;
}
void* inspectHash(hashmap* h, uint32_t key) {
    return h->data[(key % HASHMAP_SIZE)];
}
int insertHash(hashmap* h, uint32_t key, void* data) {
    if (inspectHash(h, key)) return 1;
    memcpy(&h->data[(key % HASHMAP_SIZE)], &data, sizeof(void*));
    h->size++;
    return 0;
}
int deleteHash(hashmap* h, uint32_t key) {
    memset(&h->data[(key % HASHMAP_SIZE)], 0, sizeof(void*));
    h->size--;
    return 0;
}

#endif
