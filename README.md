# Lib-Hash
**Lib-Hash** is a light-weight C-library for hashmap. It can be used to store any
primitive or user-defined datas. It provides very easy and intuitive API for creating,
deleting, reading, writing and iterating through the elements in hash-table/map.

_Under the hood, It maps from `char*` to `void*`. The reason behind this is to be
able to store any data type in the `hash-table` by casting the reference to it into
`void*`._

_The hashing algorithm used in it is [**FNV-1A**](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash)._

# Using it
In order to use it in your project, simply clone the repo inside your project and include
`libhash.h` header file into the c program you want to use.

1. Go to your project Directory
    ```bash
    cd my-project/
    ```

2. Clone the library inside the project
    ```bash
    git clone https://github.com/therealsunx/libhash
    ```

3. Simply include the header file wherever you want to use `libhash`
    ```c
    #include "libhash/libhash.h"
    ```

And you're good to go.

# API

## Data Types
__Note: the data-members are not to be modified directly. Use provided methods to operate on data.__

```c
typedef struct {
  char* key;
  void* value;
  hash_t __hash__;
} hpair_t;

typedef struct {
  hpair_t* entries;
  size_t __capacity__, __size__;
} hashmap_t;

typedef struct {
  char* key;
  void* value;
  hashmap_t* __map__;
  size_t __index__;
} hm_iterator_t;
```

## Methods

1. Create a new hashmap
```c
hashmap_t* hashmap_create(void);
```

2. Destroy/Free the hashmap
```c
void hashmap_destroy(hashmap_t* map);
```

3. Get the number of items in hashmap
```c
size_t hashmap_getCount(hashmap_t *map);
```

4. Get the current total capacity of hashmap
```c
size_t hashmap_getCapacity(hashmap_t *map);
```

5. Get the item using `key`. Returns `NULL` pointer if key is not in the hashmap
```c
void* hashmap_get(hashmap_t* map, char* key);
```

6. Add a key-value pair in the map. Returns 1 if successful and 0 if the operation fails.
```c
uint32_t hashmap_set(hashmap_t* map, char* key, void* value);
```

7. Removes an entry from the hashmap. If key is not in map, then nothing's changes.
```c
void hashmap_pop(hashmap_t* map, char* key);
```

8. Returns an iterator for the hashmap
```c
hm_iterator_t hashmap_iterator(hashmap_t* map);
```

9. Iterates over to the next entry in the provided hashmap's iterator. Returns 0 if no more entry left.
```c
uint32_t hashmap_next(hm_iterator_t* iterator);
```
if it returns `1`, then you can access the `key` and `value` from the iterator.
