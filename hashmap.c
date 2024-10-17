#include "hashmap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

hash_t FNV_1a_hash(void *key, size_t size){
/* FNV-1a hash
 * libhash uses 32-bit FNV-1a hashing algorithm to generate hashes
 * For more information follow the link
 * https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
 */

  assert(key != NULL);
  hash_t hash = FNV_offset_basis;

  for(uint32_t i = 0;i<size;i++){
    hash ^= ((byte*)key)[i];
    hash *= FNV_prime;
  }
  return hash;
}

hashmap_t* hashmap_create(void){
  hashmap_t* nmap = malloc(sizeof(*nmap));
  if(!nmap) return NULL;

  nmap->_size = 0;
  nmap->_capacity = DEF_MAP_CAPACITY;
  nmap->entries = calloc(nmap->_capacity, sizeof(hpair_t));
  if(!nmap->entries) {
    free(nmap);
    return NULL;
  }
  return nmap;
}

void hashmap_destroy(hashmap_t* table){
  assert(table != NULL);

  for(size_t i=0; i < table->_capacity; i++){
    free(table->entries[i].key);
  }
  free(table->entries);
  free(table);
}

void _expand_hashmap(hashmap_t* table){
  assert(table != NULL);

  size_t ncap = table->_capacity<<1;
  hpair_t* entries = calloc(ncap, sizeof(*entries));

  for(uint32_t i=0, index; i<table->_capacity; i++){
    hpair_t _ent = table->entries[i];
    if(!_ent.key) continue;

    index = _ent._hash & (ncap-1);
    while(entries[index].key != NULL){
      index++;
      if(index == ncap) index=0;
    }
    entries[index].key = _ent.key;
    entries[index].value = _ent.value;
    entries[index]._hash = _ent._hash;
  }
  free(table->entries);
  table->_capacity = ncap;
  table->entries = entries;
}

size_t hashmap_getSize(hashmap_t *map){
  assert(map != NULL);
  return map->_size;
}
size_t hashmap_getCapacity(hashmap_t *map){
  assert(map != NULL);
  return map->_capacity;
}

void hashmap_pop(hashmap_t* table, char* key){
  assert(table != NULL && key != NULL);
  hash_t hash = FNV_1a_hash(key, strlen(key));

  uint32_t index = hash & (table->_capacity-1);
  for(uint32_t i=0; i<(uint32_t)table->_capacity; i++){
    hpair_t* entry = &table->entries[index];
    if(!entry->key && !entry->_hash) break;

    if(entry->key && hash == entry->_hash){
      entry->value = NULL;
      free(entry->key);
    }
    if(++index == table->_capacity) index = 0;
  }
}

void* hashmap_get(hashmap_t* table, char* key){
  assert(table != NULL && key != NULL);
  hash_t hash = FNV_1a_hash(key, strlen(key));

  uint32_t index = hash & (table->_capacity-1);
  for(uint32_t i=0; i<(uint32_t)table->_capacity; i++, index++){
    hpair_t entry =table->entries[index];

    if(!entry.key && !entry._hash) return NULL; // key doesnot exists
    if(entry.key && hash == entry._hash){
      return entry.value;
    }
    if(index == table->_capacity) index = 0;
  }

  return NULL;
}

void hashmap_set(hashmap_t* table, char* key, void* value){
  assert(table != NULL && key != NULL);

  if(table->_size+1 == table->_capacity){
    _expand_hashmap(table);
  }

  hash_t hash = FNV_1a_hash(key, strlen(key));
  uint32_t index = hash & (table->_capacity-1); // capacity must be power of 2
  while(table->entries[index].key != NULL){
    index++;
    if(index == table->_capacity) index=0;
  }
  table->entries[index].key = strdup(key);
  table->entries[index].value = value;
  table->entries[index]._hash = hash;
  table->_size++;
}

hm_iterator_t hashmap_iterator(hashmap_t* map){
  assert(map != NULL);
  hm_iterator_t iter;
  iter._map = map;
  iter._index = 0;
  return iter;
}

uint32_t hashmap_next(hm_iterator_t* iter){
  assert(iter != NULL);
  while(iter->_index < iter->_map->_capacity){
    if(iter->_map->entries[iter->_index].key){
      iter->key = iter->_map->entries[iter->_index].key;
      iter->value = iter->_map->entries[iter->_index].value;
      iter->_index++;
      return 1;
    }
    iter->_index++;
  }
  return 0;
}

