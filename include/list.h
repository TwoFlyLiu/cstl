#ifndef INCLUDE_LIST_H_H
#define INCLUDE_LIST_H_H

#include "cstl_stddef.h"

typedef struct CSTLListNode {
    struct CSTLListNode *prev;
    struct CSTLListNode *next;

    void *data;
} list_node_t;

typedef struct CSTLList{
    list_node_t *head;
    list_node_t *tail;

    size_t len;
    size_t unit_size;
    destroy_func_t destroy_func;
} LIST, list_t;

typedef void (*LIST_FOREACH_FUNC)(void *value, void *user_data);
typedef LIST_FOREACH_FUNC list_foreach_func_t;

// 工厂方法
CSTL_LIB LIST *list_new(size_t unit_size, destroy_func_t destroy);

CSTL_LIB void list_free(LIST *list);

// 访问修改容量信息
CSTL_LIB size_t list_capacity(LIST *list);
CSTL_LIB bool list_empty(LIST *list);
CSTL_LIB size_t list_size(LIST *list);

// 增加
CSTL_LIB void list_push_front(LIST *list, void *elem);
CSTL_LIB void list_push_back(LIST *list, void *elem);
CSTL_LIB void list_insert(LIST *list, int index, void *elem);
CSTL_LIB void list_extend(LIST *list, int index, LIST *insert_list);
CSTL_LIB void list_extend_front(LIST *list, LIST *insert_list);
CSTL_LIB void list_extend_back(LIST *list, LIST *insert_list);

//  访问/修改
CSTL_LIB void *list_get(LIST *list, int index);
CSTL_LIB list_node_t *list_get_node(LIST *list, int index);

CSTL_LIB void *list_at(LIST *list, int index);
CSTL_LIB void list_set(LIST *list, int index, void *value);

CSTL_LIB void *list_front(LIST *list);
CSTL_LIB void *list_back(LIST *list);

// 移除
CSTL_LIB void list_pop_back(LIST *list);
CSTL_LIB void list_pop_front(LIST *list);

CSTL_LIB void list_remove(LIST *list, void *value, cmp_func_t compare); //移除第一个出现的值
CSTL_LIB void list_remove_node(LIST *list, list_node_t *node);
CSTL_LIB void list_erase(LIST *list, int index); //移除指定位置上的元素
CSTL_LIB void list_clear(LIST *list); //移除内部所有元素

// 查找
CSTL_LIB list_node_t *list_find_node(LIST *list, void *val, cmp_func_t compare);

CSTL_LIB void *list_find(LIST *list, void *val, cmp_func_t compare);

// 二分法查找，对于list效率非常低
CSTL_LIB void *list_bin_find(LIST *list, void *val, cmp_func_t compare);

// 排序（使用快速排序算法）
CSTL_LIB void list_sort(LIST *list, cmp_func_t compare);

// 交换两个元素的值
CSTL_LIB void list_swap(LIST *list, int idx1, int idx2);

// 遍历函数
CSTL_LIB void list_foreach(LIST *list, list_foreach_func_t list_foreach_func, void *user_data);

CSTL_LIB void list_resize(LIST *list, size_t new_size, void *val);

//// 下面定义的宏， 主要忽略类型转换问题
#define DEFINE_NUM_TYPE_LIST(type) \
    static inline LIST * type##_list_new() {\
        return list_new(sizeof(type), NULL);\
    }\
    static inline type * type##_list_get(LIST *list, int index) {\
        void *ret = list_get(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_at(LIST *list, int index) {\
        void *ret = list_at(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_front(LIST *list) {\
        void *ret = list_front(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_back(LIST *list) {\
        void *ret = list_back(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_find(LIST *list, type value) {\
        void * ret = list_find(list, &value, CSTL_NUM_CMP_FUNC(type));\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_list_push_front(LIST *list, type value) {\
        list_push_front(list, &value);\
    }\
    static inline void type##_list_push_back(LIST *list, type value) {\
        list_push_back(list, &value);\
    }\
    static inline void type##_list_insert(LIST *list, int index, type value) {\
        list_insert(list, index, &value);\
    }\
    static inline void type##_list_set(LIST *list, int index, type value) {\
        list_set(list, index, &value);\
    }\
    static inline void type##_list_remove(LIST *list, type value) {\
        list_remove(list, &value, CSTL_NUM_CMP_FUNC(type));\
    }\
    static inline void type##_list_sort(LIST *list) {\
        list_sort(list, CSTL_NUM_CMP_FUNC(type)); \
    }\
    static inline void type##_list_resize(LIST *list, int new_size, type value) {\
        list_resize(list, new_size, &value);\
    }

#define DEFINE_FLOAT_NUM_TYPE_LIST(type) \
    static inline LIST * type##_list_new() {\
        return list_new(sizeof(type), NULL);\
    }\
    static inline type * type##_list_get(LIST *list, int index) {\
        void *ret = list_get(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_at(LIST *list, int index) {\
        void *ret = list_at(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_front(LIST *list) {\
        void *ret = list_front(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_back(LIST *list) {\
        void *ret = list_back(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_find(LIST *list, type value) {\
        void * ret = list_find(list, &value, CSTL_NUM_CMP_FUNC(type));\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_list_push_front(LIST *list, type value) {\
        list_push_front(list, &value);\
    }\
    static inline void type##_list_push_back(LIST *list, type value) {\
        list_push_back(list, &value);\
    }\
    static inline void type##_list_insert(LIST *list, int index, type value) {\
        list_insert(list, index, &value);\
    }\
    static inline void type##_list_set(LIST *list, int index, type value) {\
        list_set(list, index, &value);\
    }\
    static inline void type##_list_remove(LIST *list, type value) {\
        list_remove(list, &value, CSTL_NUM_CMP_FUNC(type));\
    }\
    static inline void type##_list_sort(LIST *list) {\
        list_sort(list, CSTL_NUM_CMP_FUNC(type)); \
    }\
    static inline void type##_list_resize(LIST *list, int new_size, type value) {\
        list_resize(list, new_size, &value);\
    }

#define DEFINE_UNSIGNED_NUM_TYPE_LIST(type) \
    static inline LIST * unsigned_##type##_list_new() {\
        return list_new(sizeof(unsigned type), NULL);\
    }\
    static inline unsigned type * unsigned_##type##_list_get(LIST *list, int index) {\
        void *ret = list_get(list, index);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_list_at(LIST *list, int index) {\
        void *ret = list_at(list, index);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_list_front(LIST *list) {\
        void *ret = list_front(list);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_list_back(LIST *list) {\
        void *ret = list_back(list);\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline unsigned type * unsigned_##type##_list_find(LIST *list, unsigned type value) {\
        void * ret = list_find(list, &value, CSTL_UNSIGNED_CMP_FUNC(type));\
        return (ret == NULL ? (unsigned type *)NULL : (unsigned type *)ret);\
    }\
    static inline void unsigned_##type##_list_push_front(LIST *list, unsigned type value) {\
        list_push_front(list, &value);\
    }\
    static inline void unsigned_##type##_list_push_back(LIST *list, unsigned type value) {\
        list_push_back(list, &value);\
    }\
    static inline void unsigned_##type##_list_insert(LIST *list, int index, unsigned type value) {\
        list_insert(list, index, &value);\
    }\
    static inline void unsigned_##type##_list_set(LIST *list, int index, unsigned type value) {\
        list_set(list, index, &value);\
    }\
    static inline void unsigned_##type##_list_remove(LIST *list, unsigned type value) {\
        list_remove(list, &value, CSTL_UNSIGNED_CMP_FUNC(type));\
    }\
    static inline void unsigned_##type##_list_sort(LIST *list) {\
        list_sort(list, CSTL_UNSIGNED_CMP_FUNC(type)); \
    }\
    static inline void unsigned_##type##_list_resize(LIST *list, int new_size, unsigned type value) {\
        list_resize(list, new_size, &value);\
    }


//// 定义常用类型list的遍历包装函数
DEFINE_NUM_TYPE_LIST(char)

DEFINE_NUM_TYPE_LIST(short)
DEFINE_NUM_TYPE_LIST(int)
DEFINE_NUM_TYPE_LIST(long)

DEFINE_FLOAT_NUM_TYPE_LIST(float)
DEFINE_FLOAT_NUM_TYPE_LIST(double)

DEFINE_UNSIGNED_NUM_TYPE_LIST(char)
DEFINE_UNSIGNED_NUM_TYPE_LIST(short)
DEFINE_UNSIGNED_NUM_TYPE_LIST(int)
DEFINE_UNSIGNED_NUM_TYPE_LIST(long)


//// type - 表示具体IDE结构体类型
//// cmp  - 与type相匹配的LIST_COMPARE类型的函数指针
#define DEFINE_STRUCT_TYPE_LIST(type, cmp_func) \
    static inline LIST * type##_list_new() {\
        return list_new(sizeof(type), NULL);\
    }\
    static inline type * type##_list_get(LIST *list, int index) {\
        void *ret = list_get(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_at(LIST *list, int index) {\
        void *ret = list_at(list, index);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_front(LIST *list) {\
        void *ret = list_front(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_back(LIST *list) {\
        void *ret = list_back(list);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_find(LIST *list, type *value) {\
        void * ret = list_find(list, value, &cmp_func);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline type * type##_list_find_func(LIST *list, type *value, cmp_func_t compare) {\
        void * ret = list_find(list, value, compare);\
        return (ret == NULL ? (type *)NULL : (type *)ret);\
    }\
    static inline void type##_list_push_front(LIST *list, type *value) {\
        list_push_front(list, value);\
    }\
    static inline void type##_list_push_back(LIST *list, type *value) {\
        list_push_back(list, value);\
    }\
    static inline void type##_list_insert(LIST *list, int index, type *value) {\
        list_insert(list, index, value);\
    }\
    static inline void type##_list_set(LIST *list, int index, type *value) {\
        list_set(list, index, value);\
    }\
    static inline void type##_list_remove(LIST *list, type *value) {\
        list_remove(list, value, &cmp_func);\
    }\
    static inline void type##_list_remove_func(LIST *list, type *value, cmp_func_t compare) {\
        list_remove(list, value, compare);\
    }\
    static inline void type##_list_sort(LIST *list) {\
        list_sort(list, &cmp_func); \
    }\
    static inline void type##_list_sort_func(LIST *list, cmp_func_t compare) {\
        list_sort(list, compare); \
    }

#endif //LIST_H_H
