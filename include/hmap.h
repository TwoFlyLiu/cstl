#ifndef HMAP_H_H
#define HMAP_H_H

// 因为c不支持模板，但是因为void*支持任何类型数据
// 
// 所以使用void *表示任何数据类型的存储地址，使用size来记录这个地址的长度
// 
// 那么ptr, size就可以表示任何数据类型了，可以用来存储任何类型的数据
// 
// 那么如果输入参数是int类型，
// 那么我只要分配者size(int)类型的长度的内部，并且将int的值拷贝到这个分配的内存中
// 那么新分配的内存就可以表示int类型了。
// 

#define BUCKET_SIZE 255

#include "cstl_stddef.h"
#include "vec.h"

// 计算hash值
typedef hash_func_t HMAP_HASH;

// 比较两个Key是否相等，0表示相等，非0表示不等
typedef cmp_func_t HMAP_KEY_CMP;

// vec_for_each遍历的回调函数
typedef void (*HMAP_FOR_EACH)(void *key, void *value, void *user_data);

typedef struct {
    VEC* buckets[BUCKET_SIZE];
    int key_size;
    int value_size;
    int len;
    hash_func_t hash_func;
    cmp_func_t key_cmp_func;
    destroy_func_t key_destroy;
    destroy_func_t val_destroy;
}hmap_t, HMAP;


// 工厂函数
// 
// 构造一个hmap
// 
// key_size - 表示键类型在内存中占用的长度
//  如果，键是字符串的话，那么就一定要把它保存到数组中（因为数组的长度是不可变的）
//  使用sizeof(type), type是时间键的数据类型
// value_size - 表示单个值在内存中占用的长度
// hash_func - 计算实际key的hash值
// key_cmp_func - 比较两个实际key的两者的大小关系 
CSTL_LIB HMAP *hmap_new(int key_size, int value_size, 
        hash_func_t hash_func,
        cmp_func_t key_cmp_func);

CSTL_LIB HMAP *hmap_new_with_destroy_func(int key_size, int value_size, 
        hash_func_t hash_func,
        HMAP_KEY_CMP key_cmp_func,
        destroy_func_t key_destroy,
        destroy_func_t val_destroy);

// 释放内部所占用的内存
CSTL_LIB void hmap_free(HMAP *hmap);

// 插入输出
CSTL_LIB void hmap_insert(HMAP *hmap, void *key, void *value);

// 查找数据
CSTL_LIB void *hmap_get(const HMAP *hmap, void *key);
CSTL_LIB void *hmap_find(const HMAP *hmap, void *key);

CSTL_LIB bool hmap_has_key(const HMAP *hmap, void *key);

// 修改数据
CSTL_LIB void hmap_set(const HMAP *hmap, void *key, void *new_value);

// 删除指定的元素
CSTL_LIB void hmap_erase(HMAP *hmap, void *key);

// 删除所有的元素
CSTL_LIB void hmap_clear(HMAP *hmap);

// 获取内部元素的数目
CSTL_LIB int hmap_size(const HMAP *hmap);
CSTL_LIB bool hmap_empty(const HMAP *hmap);

// for_echo用来支撑遍历
CSTL_LIB void hmap_for_each(HMAP *hmap, HMAP_FOR_EACH for_each_func, void *user_data);

#endif //HMAP_H_H
