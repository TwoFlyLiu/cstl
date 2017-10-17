#ifndef VEC_H_H
#define VEC_H_H

#include "cstl_stddef.h"

typedef struct {
    void *beg;
    int len;
    int unit_size;
    int capacity;
    destroy_func_t destroy_func;
} VEC, vec_t;

typedef void (*VEC_FOREACH_FUNC)(void *value, void *user_data);
typedef VEC_FOREACH_FUNC vec_foreach_func_t;

// 工厂方法
CSTL_LIB VEC *vec_new(int unit_size, destroy_func_t destroy);
CSTL_LIB VEC *vec_new_with_capacity(int unit_size, destroy_func_t destroy, int capacity);

CSTL_LIB void vec_free(VEC *vec);

// 访问修改容量信息
CSTL_LIB int vec_capacity(VEC *vec);
CSTL_LIB bool vec_empty(VEC *vec);
CSTL_LIB int vec_size(VEC *vec);

CSTL_LIB void vec_resize(VEC *vec, int new_size, void *value);

// 增加
CSTL_LIB void vec_push_front(VEC *vec, void *elem);
CSTL_LIB void vec_push_back(VEC *vec, void *elem);
CSTL_LIB void vec_insert(VEC *vec, int index, void *elem);
CSTL_LIB void vec_extend(VEC *vec, int index, VEC *insert_vec);
CSTL_LIB void vec_extend_front(VEC *vec, VEC *insert_vec);
CSTL_LIB void vec_extend_back(VEC *vec, VEC *insert_vec);

//  访问/修改
CSTL_LIB void *vec_get(VEC *vec, int index);

void *vec_at(VEC *vec, int index);
void vec_set(VEC *vec, int index, void *value);

CSTL_LIB void *vec_front(VEC *vec);
CSTL_LIB void *vec_back(VEC *vec);

// 移除
CSTL_LIB void vec_pop_back(VEC *vec);
CSTL_LIB void vec_pop_front(VEC *vec);

CSTL_LIB void vec_remove(VEC *vec, void *value, cmp_func_t compare); //移除第一个出现的值
CSTL_LIB void vec_erase(VEC *vec, int index); //移除指定位置上的元素
CSTL_LIB void vec_clear(VEC *vec); //移除内部所有元素

// 查找
CSTL_LIB void *vec_find(VEC *vec, void *val, cmp_func_t compare);
CSTL_LIB void *vec_bin_find(VEC *vec, void *val, cmp_func_t compare);

// 排序（使用快速排序算法）
CSTL_LIB void vec_sort(VEC *vec, cmp_func_t compare);

// 交换两个元素的值
CSTL_LIB void vec_swap(VEC *vec, int idx1, int idx2);

// 遍历函数
CSTL_LIB void vec_foreach(VEC *vec, vec_foreach_func_t vec_foreach_func, void *user_data);

//// 下面定义的宏， 主要忽略类型转换问题
#define DEFINE_NUM_TYPE_VEC(type) \
    static inline VEC * type##_vec_new() {\
        return vec_new(sizeof(type), NULL);\
    }\
    static inline VEC * type##_vec_new_with_capacity(int capacity) {\
        return vec_new_with_capacity(sizeof(type), NULL, capacity);\
    }\
    static inline type * type##_vec_get(VEC *vec, int index) {\
        void *ret = vec_get(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_at(VEC *vec, int index) {\
        void *ret = vec_at(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_front(VEC *vec) {\
        void *ret = vec_front(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_back(VEC *vec) {\
        void *ret = vec_back(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_find(VEC *vec, type value) {\
        void * ret = vec_find(vec, &value, CSTL_NUM_CMP_FUNC(type));\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_vec_push_front(VEC *vec, type value) {\
        vec_push_front(vec, &value);\
    }\
    static inline void type##_vec_push_back(VEC *vec, type value) {\
        vec_push_back(vec, &value);\
    }\
    static inline void type##_vec_insert(VEC *vec, int index, type value) {\
        vec_insert(vec, index, &value);\
    }\
    static inline void type##_vec_set(VEC *vec, int index, type value) {\
        vec_set(vec, index, &value);\
    }\
    static inline void type##_vec_remove(VEC *vec, type value) {\
        vec_remove(vec, &value, CSTL_NUM_CMP_FUNC(type));\
    }\
    static inline void type##_vec_sort(VEC *vec) {\
        vec_sort(vec, CSTL_NUM_CMP_FUNC(type)); \
    }\
    static inline void type##_vec_resize(VEC *vec, int new_size, type value) {\
        vec_resize(vec, new_size, &value);\
    }

#define DEFINE_FLOAT_NUM_TYPE_VEC(type) \
    static inline VEC * type##_vec_new() {\
        return vec_new(sizeof(type), NULL);\
    }\
    static inline VEC * type##_vec_new_with_capacity(int capacity) {\
        return vec_new_with_capacity(sizeof(type), NULL, capacity);\
    }\
    static inline type * type##_vec_get(VEC *vec, int index) {\
        void *ret = vec_get(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_at(VEC *vec, int index) {\
        void *ret = vec_at(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_front(VEC *vec) {\
        void *ret = vec_front(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_back(VEC *vec) {\
        void *ret = vec_back(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_find(VEC *vec, type value) {\
        void * ret = vec_find(vec, &value, CSTL_NUM_CMP_FUNC(type));\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_vec_push_front(VEC *vec, type value) {\
        vec_push_front(vec, &value);\
    }\
    static inline void type##_vec_push_back(VEC *vec, type value) {\
        vec_push_back(vec, &value);\
    }\
    static inline void type##_vec_insert(VEC *vec, int index, type value) {\
        vec_insert(vec, index, &value);\
    }\
    static inline void type##_vec_set(VEC *vec, int index, type value) {\
        vec_set(vec, index, &value);\
    }\
    static inline void type##_vec_remove(VEC *vec, type value) {\
        vec_remove(vec, &value, CSTL_NUM_CMP_FUNC(type));\
    }\
    static inline void type##_vec_sort(VEC *vec) {\
        vec_sort(vec, CSTL_NUM_CMP_FUNC(type)); \
    }\
    static inline void type##_vec_resize(VEC *vec, int new_size, type value) {\
        vec_resize(vec, new_size, &value);\
    }

#define DEFINE_UNSIGNED_NUM_TYPE_VEC(type) \
    static inline VEC * unsigned_##type##_vec_new() {\
        return vec_new(sizeof(unsigned type), NULL);\
    }\
    static inline VEC * unsigned_##type##_vec_new_with_capacity(int capacity) {\
        return vec_new_with_capacity(sizeof(unsigned type), NULL, capacity);\
    }\
    static inline unsigned type * unsigned_##type##_vec_get(VEC *vec, int index) {\
        void *ret = vec_get(vec, index);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_vec_at(VEC *vec, int index) {\
        void *ret = vec_at(vec, index);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_vec_front(VEC *vec) {\
        void *ret = vec_front(vec);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_vec_back(VEC *vec) {\
        void *ret = vec_back(vec);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_vec_find(VEC *vec, unsigned type value) {\
        void * ret = vec_find(vec, &value, CSTL_UNSIGNED_CMP_FUNC(type));\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline void unsigned_##type##_vec_push_front(VEC *vec, unsigned type value) {\
        vec_push_front(vec, &value);\
    }\
    static inline void unsigned_##type##_vec_push_back(VEC *vec, unsigned type value) {\
        vec_push_back(vec, &value);\
    }\
    static inline void unsigned_##type##_vec_insert(VEC *vec, int index, unsigned type value) {\
        vec_insert(vec, index, &value);\
    }\
    static inline void unsigned_##type##_vec_set(VEC *vec, int index, unsigned type value) {\
        vec_set(vec, index, &value);\
    }\
    static inline void unsigned_##type##_vec_remove(VEC *vec, unsigned type value) {\
        vec_remove(vec, &value, CSTL_UNSIGNED_CMP_FUNC(type));\
    }\
    static inline void unsigned_##type##_vec_sort(VEC *vec) {\
        vec_sort(vec, CSTL_UNSIGNED_CMP_FUNC(type)); \
    }\
    static inline void unsigned_##type##_vec_resize(VEC *vec, int new_size, unsigned type value) {\
        vec_resize(vec, new_size, &value);\
    }


//// 定义常用类型vec的遍历包装函数
DEFINE_NUM_TYPE_VEC(char)

DEFINE_NUM_TYPE_VEC(short)
DEFINE_NUM_TYPE_VEC(int)
DEFINE_NUM_TYPE_VEC(long)

DEFINE_FLOAT_NUM_TYPE_VEC(float)
DEFINE_FLOAT_NUM_TYPE_VEC(double)

DEFINE_UNSIGNED_NUM_TYPE_VEC(char)
DEFINE_UNSIGNED_NUM_TYPE_VEC(short)
DEFINE_UNSIGNED_NUM_TYPE_VEC(int)
DEFINE_UNSIGNED_NUM_TYPE_VEC(long)


//// type - 表示具体IDE结构体类型
//// cmp  - 与type相匹配的VEC_COMPARE类型的函数指针
#define DEFINE_STRUCT_TYPE_VEC(type, cmp_func) \
    static inline VEC * type##_vec_new() {\
        return vec_new(sizeof(type), NULL);\
    }\
    static inline VEC * type##_vec_new_with_capacity(int capacity) {\
        return vec_new_with_capacity(sizeof(type), NULL, capacity);\
    }\
    static inline type * type##_vec_get(VEC *vec, int index) {\
        void *ret = vec_get(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_at(VEC *vec, int index) {\
        void *ret = vec_at(vec, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_front(VEC *vec) {\
        void *ret = vec_front(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_back(VEC *vec) {\
        void *ret = vec_back(vec);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_find(VEC *vec, type *value) {\
        void * ret = vec_find(vec, value, &cmp_func);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_vec_find_func(VEC *vec, type *value, cmp_func_t compare) {\
        void * ret = vec_find(vec, value, compare);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_vec_push_front(VEC *vec, type *value) {\
        vec_push_front(vec, value);\
    }\
    static inline void type##_vec_push_back(VEC *vec, type *value) {\
        vec_push_back(vec, value);\
    }\
    static inline void type##_vec_insert(VEC *vec, int index, type *value) {\
        vec_insert(vec, index, value);\
    }\
    static inline void type##_vec_set(VEC *vec, int index, type *value) {\
        vec_set(vec, index, value);\
    }\
    static inline void type##_vec_remove(VEC *vec, type *value) {\
        vec_remove(vec, value, &cmp_func);\
    }\
    static inline void type##_vec_remove_func(VEC *vec, type *value, cmp_func_t compare) {\
        vec_remove(vec, value, compare);\
    }\
    static inline void type##_vec_sort(VEC *vec) {\
        vec_sort(vec, &cmp_func); \
    }\
    static inline void type##_vec_sort_func(VEC *vec, cmp_func_t compare) {\
        vec_sort(vec, compare); \
    }

#endif //VEC_H_H
