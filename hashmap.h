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
  hash_t __hash__;
} hpair_t;

typedef struct {
  hpair_t* entries;
  size_t __capacity__, __size__;
} hashmap_t;

/*Creates  an empty hashmap*/
hashmap_t* hashmap_create(void);
/*Frees the hashmap*/
void hashmap_destroy(hashmap_t* map);

/*Returns the number of items in the hashmap*/
size_t hashmap_getCount(hashmap_t *map);
/*Returns the current capacity of the hashmap
 * *Note: hashmap expands automatically* */
size_t hashmap_getCapacity(hashmap_t *map);

/* Returns the value associated to the 'key'.
 * *Returns NULL if key does not exists* */
void* hashmap_get(hashmap_t* map, char* key);

/* Adds an entry with provided key-value pair.
 * Returns 1 if successfull, 0 if failed*/
uint32_t hashmap_set(hashmap_t* map, char* key, void* value);
/* Removes an entry from the hashmap */
void hashmap_pop(hashmap_t* map, char* key);

typedef struct {
  char* key;
  void* value;
  hashmap_t* __map__;
  size_t __index__;
} hm_iterator_t;

/*Returns an iterator for the hashmap,
 * which can be iterated with hashmap_next() function */
hm_iterator_t hashmap_iterator(hashmap_t* map);
/* Iterates over to the next entry in the hashmap */
uint32_t hashmap_next(hm_iterator_t* iterator);

