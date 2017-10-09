/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 01 18:01:12
*/
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hmap.h"
#include "leak.h"

#define KEY(entry) (entry)
#define VALUE(entry, key_size) ((char*)(entry)+ key_size)

static inline void
__free(void *ptr)
{
    if (ptr != NULL) {
        cstl_free(ptr);
    }
}

static inline void
__destroy_entry(void *entry, int key_size,
        destroy_func_t key_destroy, destroy_func_t val_destroy)
{
    if (key_destroy) {
        (*key_destroy)(KEY(entry));
    }

    if (val_destroy && key_size > 0) {
        (*val_destroy)(VALUE(entry, key_size));
    }
}

// 工厂函数
HMAP *hmap_new(int key_size, int value_size
        , hash_func_t hash_func
        , cmp_func_t key_cmp_func)
{
    return hmap_new_with_destroy_func(key_size, value_size, hash_func, key_cmp_func,
            NULL, NULL);
}

HMAP *hmap_new_with_destroy_func(int key_size, int value_size
        , hash_func_t hash_func
        , cmp_func_t key_cmp_func
        , destroy_func_t key_destroy
        , destroy_func_t val_destroy)
{
    assert(hash_func && key_cmp_func);

    HMAP *hmap = (HMAP *)cstl_malloc(sizeof(HMAP));
    int entry_size = key_size + value_size;

    for (int i = 0; i < BUCKET_SIZE; i++) {
        hmap->buckets[i] = vec_new(entry_size, NULL);
    }

    hmap->key_size = key_size;
    hmap->value_size = value_size;
    hmap->hash_func = hash_func;
    hmap->key_cmp_func = key_cmp_func;
    hmap->len = 0;
    hmap->key_destroy = key_destroy;
    hmap->val_destroy = val_destroy;

    return hmap;
}

void hmap_free(HMAP *hmap)
{
    assert(hmap);

    for (int i = 0; i < BUCKET_SIZE; i++) {
        for (int j = 0; j < vec_size(hmap->buckets[i]); j++) {
            __destroy_entry(vec_get(hmap->buckets[i], j), hmap->key_size,
                    hmap->key_destroy, hmap->val_destroy);
        }
        vec_free(hmap->buckets[i]);
    }
    __free(hmap);
}

static inline void *
__entry_new(int key_size, int value_size, void *key, void *value)
{
    void *entry = cstl_malloc(key_size + value_size);

    // 设置数值
    memcpy(KEY(entry), key, key_size);
    memcpy(VALUE(entry, key_size), value, value_size);

    return entry;
}

static inline void
__entry_free(void *entry) 
{
    __free(entry);
}

// 插入输出
// 如果key已经存在了，则覆盖掉原来的值
// 否则插入新的值
void hmap_insert(HMAP *hmap, void *key, void *value)
{
    void *entry;

    assert(hmap && key 
            && ((0 == hmap->value_size) || ((hmap->value_size > 0) && value)));

    entry = hmap_get(hmap, key);
    if (entry != NULL) {
        memcpy(VALUE(entry, hmap->key_size), value, hmap->value_size);
    } else {
        entry = __entry_new(hmap->key_size, hmap->value_size, key, value);
        vec_push_back(hmap->buckets[hmap->hash_func(key) % BUCKET_SIZE], entry);
        __entry_free(entry);
        ++ hmap->len;
    }

}

// 查找数据
void *hmap_get(const HMAP *hmap, void *key)
{
    assert(hmap && key);

    int hash_value = hmap->hash_func(key) % BUCKET_SIZE;
    VEC *bucket = hmap->buckets[hash_value];
    void *entry;

    assert(hmap && key);

    for (int i = 0; i < vec_size(bucket); i++) {
        entry = vec_get(bucket, i);
        if (hmap->key_cmp_func(KEY(entry), key) == 0) {
            return VALUE(entry, hmap->key_size);
        }        
    }

    return NULL;
}

void *hmap_find(const HMAP *hmap, void *key)
{
    return hmap_get(hmap, key);
}

bool hmap_has_key(const HMAP *hmap, void *key)
{
    return (hmap_get(hmap, key) != NULL);
}

// 修改数据
void hmap_set(const HMAP *hmap, void *key, void *new_value)
{
    void *value;
   
    assert(hmap && key && new_value);
    value = hmap_get(hmap, key);

    assert(hmap && key && new_value);

    if (value != NULL) {
        memcpy(value, new_value, hmap->value_size);
    }
}

// 获取内部元素的数目
int hmap_size(const HMAP *hmap)
{
    assert(hmap);
    return hmap->len;
}

bool hmap_empty(const HMAP *hmap)
{
    return (0 == hmap_size(hmap));
}
 
void hmap_for_each(HMAP *hmap, HMAP_FOR_EACH for_each_func, void *user_data)
{
    assert(hmap && for_each_func);

    for (int i = 0; i < BUCKET_SIZE; i++) {
        VEC *bucket = hmap->buckets[i];
        if (!vec_empty(bucket)) {
            for (int j = 0; j < vec_size(bucket); j++) {
                void *entry = vec_get(bucket, j);
                for_each_func(KEY(entry), VALUE(entry, hmap->key_size), user_data);
            }
        }
    }
}

// 删除指定的元素
// 新的元素
void hmap_erase(HMAP *hmap, void *key)
{
    int hash;
    VEC *bucket;

    assert(hmap && key);

    hash = hmap->hash_func(key) % BUCKET_SIZE;
    bucket = hmap->buckets[hash];

    for (int i = 0; i < vec_size(bucket); i++) {
        void *entry = vec_get(bucket, i);
        if (0 == hmap->key_cmp_func(KEY(entry), key)) {
            __destroy_entry(entry, hmap->key_size, hmap->key_destroy, hmap->val_destroy);
            vec_erase(bucket, i);
            hmap->len --;
            return;
        }
    }
}

// 删除所有的元素
void hmap_clear(HMAP *hmap)
{
    int entry_size = hmap->key_size + hmap->value_size;

    assert(hmap);

    // 删除老的，构建新的
    for (int i = 0; i < BUCKET_SIZE; i++) {
        for (int j = 0; j < vec_size(hmap->buckets[i]); j++) {
            __destroy_entry(vec_get(hmap->buckets[i], j), hmap->key_size,
                    hmap->key_destroy, hmap->val_destroy);
        }
        vec_free(hmap->buckets[i]);
    }

    // 构建新的
    for (int i = 0; i < BUCKET_SIZE; i++) {
        hmap->buckets[i] = vec_new(entry_size, NULL);
    }
    hmap->len = 0;
}

#undef KEY
#undef VALUE
