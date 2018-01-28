#ifndef INCLUDE_RMAP_H_H
#define INCLUDE_RMAP_H_H

#include "cstl_stddef.h"

typedef struct rmap_t rmap_t;
typedef rmap_t RMAP;

typedef cmp_func_t key_cmp_func_t;
typedef destroy_func_t key_destroy_func_t;
typedef destroy_func_t value_destroy_func_t;

typedef void (*RMAP_FOR_EACH)(const void *key, void *value, void *user_data);

CSTL_LIB rmap_t *rmap_new(size_t key_size, size_t value_size, 
        cmp_func_t key_cmp_func);

CSTL_LIB rmap_t *rmap_new_with_destroy_func(size_t key_size, size_t value_size, 
        key_cmp_func_t key_cmp_func,
        key_destroy_func_t key_destroy,
        value_destroy_func_t val_destroy);

CSTL_LIB void rmap_free(rmap_t *rmap);

CSTL_LIB void rmap_insert(rmap_t *rmap, const void *key, const void *value);

CSTL_LIB void rmap_erase(rmap_t *hmap, const void *key);

CSTL_LIB void *rmap_get(const rmap_t *hmap, const void *key);

CSTL_LIB bool rmap_has_key(const rmap_t *hmap, const void *key);

CSTL_LIB void rmap_set(rmap_t *rmap, const void *key, const void *new_value);
CSTL_LIB void rmap_clear(rmap_t *rmap);
CSTL_LIB size_t rmap_size(const rmap_t *hmap);

CSTL_LIB void rmap_for_each(rmap_t* rmap, RMAP_FOR_EACH for_each_func, void *user_data);

CSTL_LIB bool rmap_empty(const rmap_t *rmap);

#endif //INCLUDE/RMAP_H_H
