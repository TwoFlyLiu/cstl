#ifndef HSET_H_H
#define HSET_H_H

#include "hmap.h"

typedef struct {
    hmap_t *data;
} hset_t, HSET;

hset_t *hset_new(int unit_size, hash_func_t hash_func, cmp_func_t key_cmp);
void hset_free(hset_t *hset);

void hset_insert(hset_t *hset, void* value);
void hset_erase(hset_t *hset, void *value);

void *hset_find(hset_t *hset, void *value);
bool hset_contains(hset_t *hset, void *value);

void hset_clear(hset_t *hset);
bool hset_empty(hset_t *hset);
int hset_size(hset_t *hset);

#endif //HSET_H_H
