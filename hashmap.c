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

  nmap->__size__ = 0;
  nmap->__capacity__ = DEF_MAP_CAPACITY;
  nmap->entries = calloc(nmap->__capacity__, sizeof(hpair_t));
  if(!nmap->entries) {
    free(nmap);
    return NULL;
  }
  return nmap;
}

void hashmap_destroy(hashmap_t* table){
  assert(table != NULL);

  for(size_t i=0; i < table->__capacity__; i++){
    free(table->entries[i].key);
  }
  free(table->entries);
  free(table);
}

uint32_t _expand_hashmap(hashmap_t* table){
  assert(table != NULL);

  size_t ncap = table->__capacity__<<1;
  hpair_t* entries = calloc(ncap, sizeof(*entries));
  if(!entries) return 0;

  for(uint32_t i=0, index; i<table->__capacity__; i++){
    hpair_t _ent = table->entries[i];
    if(!_ent.key) continue;

    index = _ent.__hash__ & (ncap-1);
    while(entries[index].key != NULL){
      index++;
      if(index == ncap) index=0;
    }
    entries[index].key = _ent.key;
    entries[index].value = _ent.value;
    entries[index].__hash__ = _ent.__hash__;
  }
  free(table->entries);
  table->__capacity__ = ncap;
  table->entries = entries;
  return 1;
}

size_t hashmap_getCount(hashmap_t *map){
  assert(map != NULL);
  return map->__size__;
}
size_t hashmap_getCapacity(hashmap_t *map){
  assert(map != NULL);
  return map->__capacity__;
}

void hashmap_pop(hashmap_t* table, char* key){
  assert(table != NULL && key != NULL);
  hash_t hash = FNV_1a_hash(key, strlen(key));

  uint32_t index = hash & (table->__capacity__-1);
  for(uint32_t i=0; i<(uint32_t)table->__capacity__; i++){
    hpair_t* entry = &table->entries[index];
    if(!entry->key && !entry->__hash__) break;

    if(entry->key && hash == entry->__hash__){
      entry->value = NULL;
      free(entry->key);
      entry->key = NULL;
      table->__size__--;
      return;
    }
    if(++index == table->__capacity__) index = 0;
  }
}

void* hashmap_get(hashmap_t* table, char* key){
  assert(table != NULL && key != NULL);
  hash_t hash = FNV_1a_hash(key, strlen(key));

  uint32_t index = hash & (table->__capacity__-1);
  for(uint32_t i=0; i<(uint32_t)table->__capacity__; i++, index++){
    hpair_t entry =table->entries[index];

    if(!entry.key && !entry.__hash__) return NULL; // key doesnot exists
    if(entry.key && hash == entry.__hash__){
      return entry.value;
    }
    if(index == table->__capacity__) index = 0;
  }

  return NULL;
}

uint32_t hashmap_set(hashmap_t* table, char* key, void* value){
  assert(table != NULL && key != NULL);

  if(table->__size__ >= (table->__capacity__>>1)){
    if(!_expand_hashmap(table)) return 0;
  }

  hash_t hash = FNV_1a_hash(key, strlen(key));
  uint32_t index = hash & (table->__capacity__-1); // capacity must be power of 2
  while(table->entries[index].key != NULL){
    index++;
    if(index == table->__capacity__) index=0;
  }
  table->entries[index].key = strdup(key);
  table->entries[index].value = value;
  table->entries[index].__hash__ = hash;
  table->__size__++;
  return 1;
}

hm_iterator_t hashmap_iterator(hashmap_t* map){
  assert(map != NULL);
  hm_iterator_t iter;
  iter.__map__ = map;
  iter.__index__ = 0;
  return iter;
}

uint32_t hashmap_next(hm_iterator_t* iter){
  assert(iter != NULL);
  hpair_t _entry;
  while(iter->__index__ < iter->__map__->__capacity__){
    if((_entry = iter->__map__->entries[iter->__index__]).key){
      iter->key = _entry.key;
      iter->value = _entry.value;
      iter->__index__++;
      return 1;
    }
    iter->__index__++;
  }
  return 0;
}

