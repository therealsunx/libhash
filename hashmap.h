#pragma once

#include <stdlib.h>

// hash function default values
#define FNV_offset_basis 0x811c9dc5
#define FNV_prime 0x01000193

// capacity is always a power of 2 for quick & easier calculation
#define DEF_MAP_CAPACITY 1<<4


typedef unsigned long uint64_t;
typedef signed long int64_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned char byte;

typedef uint32_t hash_t;

typedef struct {
  char* key;
  void* value;
  // cache in the hash for quick lookup in probing
  hash_t _hash;
} hpair_t;

typedef struct {
  hpair_t* entries;
  size_t _capacity, _size;
} hashmap_t;

/*Creates  a fucking hashmap*/
hashmap_t* hashmap_create(void);
void hashmap_destroy(hashmap_t* map);

size_t hashmap_getSize(hashmap_t *map);
size_t hashmap_getCapacity(hashmap_t *map);

void* hashmap_get(hashmap_t* map, char* key);
void hashmap_set(hashmap_t* map, char* key, void* value);
void hashmap_pop(hashmap_t* map, char* key);

typedef struct {
  char* key;
  void* value;

  hashmap_t* _map;
  size_t _index;
} hm_iterator_t;

hm_iterator_t hashmap_iterator(hashmap_t* map);
uint32_t hashmap_next(hm_iterator_t* iterator);

