/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 03 20:47:31
*/
#include <assert.h>
#include "hset.h"
#include "leak.h"

hset_t *hset_new(int unit_size, hash_func_t hash_func, cmp_func_t key_cmp)
{
    hset_t *hset = (hset_t*)cstl_malloc(sizeof(hset_t));
    hset->data = hmap_new(unit_size, 0, hash_func, key_cmp);
    return hset;
}

void hset_free(hset_t *hset)
{
    assert(hset);

    hmap_free(hset->data);
    cstl_free(hset);
}

void hset_insert(hset_t *hset, void *value)
{
    assert(hset && value);
    hmap_insert(hset->data, value, NULL);
}

void hset_erase(hset_t *hset, void *value)
{
    assert(hset && value);
    hmap_erase(hset->data, value);
}

void *hset_find(hset_t *hset, void *value)
{
    return hmap_find(hset->data, value);
}

bool hset_contains(hset_t *hset, void *key)
{
    return (NULL != hset_find(hset, key));
}

void hset_clear(hset_t *hset)
{
    hmap_clear(hset->data);
}

bool hset_empty(hset_t *hset)
{
    return hmap_empty(hset->data);
}

int hset_size(hset_t *hset)
{
    return hmap_size(hset->data);
}
