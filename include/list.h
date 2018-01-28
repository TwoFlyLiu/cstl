/*!
 * \file list.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于list的所有的api。
 */

#ifndef INCLUDE_LIST_H_H
#define INCLUDE_LIST_H_H

#include "cstl_stddef.h"

/*!
 * \brief 一个list中的一个节点
 */
typedef struct CSTLListNode {
    struct CSTLListNode *prev; //!< 前一个节点
    struct CSTLListNode *next; //!< 下一个节点

    void *data;
} list_node_t;

/*!
 * \brief 一个list容器，类似于c++中的list
 * 
 * 内部使用的是链表来存储数据，所以他的插入，删除复杂度都是O(1),但是他的查询复杂度是O(n),
 * 他用来处理非常大量的数据，并且需要稳定的性能时候，比较好。
 */
typedef struct CSTLList{
    list_node_t *head;              //!< 头结点
    list_node_t *tail;              //!< 尾部节点
    size_t len;                     //!< 用来记录
    size_t unit_size;               //!< 每个元素所占内存的尺寸
    destroy_func_t destroy_func;    //!< 元素对应的销毁函数
} LIST, list_t;

/*!
 *  \brief list_foreach的回调函数
 *  \param [in,out] value 当前元素值的地址
 *  \param [in,out] user_data 用户传递的数据，可以用来输出处理结果
 */
typedef void (*LIST_FOREACH_FUNC)(void *value, void *user_data);
typedef LIST_FOREACH_FUNC list_foreach_func_t; //!< 一个LIST_FOREACH_FUNC的别名，用来统一类型命名规则

// 工厂方法

/*!
 *  \brief list_new 新建一个list类型的容器。
 *  \param [in] unit_size 每个元素占用内存的尺寸，如果保存的元素类型是int, 则这个值就是sizeof(int), 如果是double,
 *  那么这个值就是sizeof(double), 一次类推.
 *  \param [in] destroy 值的销毁函数，当每次要删除一个值的时候，如果这个值被设置的话，那么每次在删除之前就会
 *  将元素的指针为参数来调用一次回调函数
 *  \retval 返回新的list容器的指针。
 *  \note 当不再使用的时候，需要调用list_free来释放内部锁占用的资源。
 */
CSTL_LIB LIST *list_new(size_t unit_size, destroy_func_t destroy);

/*!
 *  \brief list_free 销毁有list_new创建的list内部所占用的所有的资源。
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。 
 */
CSTL_LIB void list_free(LIST *list);

// 访问修改容量信息

/*!
 *  \brief list_capacity 获取list容器内部总共可以存放的元素数目
 *  \param [in] list list容器指针。
 *  \retval 返回list容器内，总共可以存放的元素数目
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t list_capacity(const LIST *list);

/*!
 *  \brief list_empty 检测list容器内部是否没有包含任何数据。
 *  \param [in] list list容器指针。
 *  \retval 如果list容器内部没有任何元素，返回true, 否则返回false。
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB bool list_empty(const LIST *list);

/*!
 *  \brief list_size 用来返回list容器内部现有的元素的数目。
 *  \param [in] list list容器指针。
 *  \retval  返回list容器内部现有的元素的数目。
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t list_size(const LIST *list);

// 增加

/*!
 *  \brief list_push_front 在头部插入一个一个元素
 *  \param [in,out] list list容器指针。
 *  \param [in] elem 要插入的值的指针。
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_push_front(LIST *list, const void *elem);

/*!
 *  \brief list_pop_back 在尾部插入一个元素
 *  \param [in,out] list list容器指针
 *  \param [in] elem 要插入栓么值的指针
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_push_back(LIST *list, const void *elem);

/*!
 *  \brief list_insert 在某个索引位置上插入一个元素
 *  \param [in,out] list list容器指针
 *  \param [in] index 要插入的索引位置
 *  \param [in] elem 要插入的值的指针。
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_insert(LIST *list, int index, const void *elem);

/*!
 *  \brief list_extend 在某个索引位置插入insert_list中的所有的元素 
 *  \param [in,out] list list容器指针
 *  \param [in] index要插入的索引位置
 *  \param [in] insert_list 该容器中所有元素会被插入
 *  \retval none.
 *  \note list, insert_list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_extend(LIST *list, int index, const LIST *insert_list);

/*!
 *  \brief list_extend_front 在list容器头部插入insert_list容器中所有的元素。
 *  \param [in,out] list list容器指针
 *  \param [in] insert_list 该容器中所有元素都会被插入
 *  \retval none.
 *  \note list, insert_list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_extend_front(LIST *list, const LIST *insert_list);

/*!
 *  \brief list_extend_front 在list容器尾部插入insert_list容器中所有的元素。
 *  \param [in,out] list list容器指针
 *  \param [in] insert_list 该容器中所有元素都会被插入
 *  \retval none.
 *  \note list, insert_list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_extend_back(LIST *list, const LIST *insert_list);

//  访问/修改

/*!
 *  \brief list_get 获取list容器指定索引的元素的指针
 *  \param [in,out] list list容器指针
 *  \param [in] index 要获取的元素索引位置。
 *  \retval 如果list容器有指定索引的话，则返回元素值的地址，否则返回NULL。
 *  \note list不能为NULL, 否则会断言失败
 */
CSTL_LIB void *list_get(LIST *list, int index);

/*!
 *  \brief list_get_node 获取list容器指定索引的元素在list容器的节点指针
 *  \param [in,out] list list 容器指针
 *  \param [in] index 要获取的元素索引位置。
 *  \retval 如果list容器有指定索引的话，则返回该元素所在节点的地址，否则返回NULL。
 */
CSTL_LIB list_node_t *list_get_node(LIST *list, int index);

/*!
 *  \brief list_get 获取list容器指定索引的元素的指针
 *  \param [in,out] list list容器指针
 *  \param [in] index 要获取的元素索引位置。
 *  \retval 如果list容器有指定索引的话，则返回元素值的地址，否则返回NULL。
 *  \note list不能为NULL, 否则会断言失败
 */
CSTL_LIB void *list_at(LIST *list, int index);

/*!
 *  \brief list_set 设置list容器中指定索引的元素的值
 *  \param [in,out] list list容器指针
 *  \param [in] index 要设置的元素的位置
 *  \param [in] value 要设置的值的地址。
 *  \retval none.
 *  \note list, value不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_set(LIST *list, int index, const void *value);

/*!
 *  \brief list_front 获取list容器头部的元素的值的地址
 *  \param [in,out] list list容器指针
 *  \retval 如果list容器不为空，则返回头部值的地址，否则返回NULL.
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB void *list_front(LIST *list);

/*!
 *  \brief list_back 获取list容器尾部的元素的值的地址
 *  \param [in,out] list list容器指针
 *  \retval 如果list容器不为空，则返回尾部元素值的地址，否则返回NULL.
 *  \note list不能为NULL, 否则会断言失败
 */
CSTL_LIB void *list_back(LIST *list);

// 移除

/*!
 *  \brief list_pop_back 删除list容器最后一个元素
 *  \param [in,out] list list容器指针
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。 
 */
CSTL_LIB void list_pop_back(LIST *list);

/*!
 *  \brief list_pop_back 删除list容器最第一个元素
 *  \param [in, out] list list容器指针
 *  \retval none.
 *  \note list 不能为NULL，否则会断言失败。 
 */
CSTL_LIB void list_pop_front(LIST *list);

/*!
 *  \brief list_remove 删除list容器中第一个compare(elem, value)==0的元素
 *  \param [in, out] list list容器指针
 *  \param [in] value 要删除的值
 *  \param [in] compare 比较函数，等于0则表示要删除
 *  \retval none.
 *  \note list value compare不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_remove(LIST *list, void *value, cmp_func_t compare); //移除第一个出现的值

/*!
 *  \brief list_remove_node 删除list容器中的指定节点。它是所有移除函数中执行速度最块的函数。
 *  \param [in,out] list list容器指针
 *  \param [in] node 要删除的节点
 *  \retval none.
 *  \note list, node不能为NULL，否则会断言失败。当这个函数执行后，node就不再有效了，千万不要使用，否则会
 *  造成程序奔溃的。
 */
CSTL_LIB void list_remove_node(LIST *list, list_node_t *node);

/*!
 *  \brief list_erase 删除list容器中指定索引的元素。
 *  \param [in,out] list list容器指针
 *  \param [in] index 指定的索引
 *  \retval none.
 *  \note list不能为NULL，否则会断言失败。
 */
CSTL_LIB void list_erase(LIST *list, int index); //移除指定位置上的元素

/*!
 *  \brief list_clear 删除list容器中所有的元素
 *  \param [in,out] list list容器指针
 *  \note list不能为NULL，否则会断言失败
 */
CSTL_LIB void list_clear(LIST *list); //移除内部所有元素

// 查找
/*!
 *  \brief list_find_node 使用指定的比较函数来查找list容器中指定的元素节点 
 *  \param [in,out] list list容器指针
 *  \param [in] val 要查找的值
 *  \compare [in] 指定的比较函数
 *  \retval 如果找到就将指定的节点地址返回，否则返回NULL。
 *  \note list, val, compare不能为NULL，否则会断言失败
 */
CSTL_LIB list_node_t *list_find_node(LIST *list, void *val, cmp_func_t compare);

/*!
 *  \brief list_find_node 使用指定的比较函数来查找list容器中指定的值
 *  \param [in,out] list list容器指针
 *  \param [in] val 要查找的值的指针
 *  \compare [in] 指定的比较函数
 *  \retval 如果找到就将容器中指定的值地址返回，否则返回NULL。
 *  \note list, val, compare不能为NULL，否则会断言失败
 */
CSTL_LIB void *list_find(LIST *list, void *val, cmp_func_t compare);

// 二分法查找，对于list效率非常低
 
/*! 
*   \brief list_bin_find 使用二分法查找来查找list容器中指定的值
* 
*   这个算法因为使用的是折半查找，每次对索引来进行折半，而对于list来讲，获取指定索引的元素
* 只有从头向后挨着走，性能非常低。或许还没有直接从头向后一个元素一个元素来查找来的快。
* 
*   \param [in,out] list list容器指针
*   \param [in] val 要查找的值的指针
*   \param [in] compare 比较函数
*   \retval 如果找到就将容器中指定的值地址返回，否则返回NULL。
*   \note list, val, compare不能为NULL，否则会断言失败
*/ 
CSTL_LIB void *list_bin_find(LIST *list, void *val, cmp_func_t compare);

// 排序（使用快速排序算法）

/*!
 *  \brief list_sort 使用指定的比较函数来排序list容器内部所有的元素
 *  \param [in,out] list list容器指针
 *  \param [in] compare 比较函数
 *  \retval none.
 *  \note list, compare不能为NULL，否则会断言失败。 
 */
CSTL_LIB void list_sort(LIST *list, cmp_func_t compare);

// 交换两个元素的值
/*!
 *  \brief list_swap 交换list容器中指定索引位置上的值
 *  \param [in,out] list list容器指针
 *  \param [in] idx1 索引1的位置
 *  \param [in] idx2 索引2的位置
 *  \retval none.
 *  \note list不能为NULL, idx1, idx2必须为有效的索引位置，否则会断言失败。
 */
CSTL_LIB void list_swap(LIST *list, int idx1, int idx2);

/*!
 *  \brief list_foreach 便利list容器中所有的元素
 *  \param [in,out] list list容器指针
 *  \param [in] list_foreach_func 回调函数
 *  \param [in,out] user_data 传入回调函数的附加参数，可以用来保存处理结果。
 *  \retval none.
 *  \note list, list_foreach_func, user_data不能为NULL, 否则会断言失败。
 */
CSTL_LIB void list_foreach(LIST *list, list_foreach_func_t list_foreach_func, void *user_data);

/*!
 *  \brief list_resize 重新调整list容器的尺寸
 *  \param [in,out] list list容器指针
 *  \param [in] new_size 新的尺寸
 *  \param [in] val 对于多出来的元素要设置的默认值
 *  \retval none.
 *  \note list和val不能为NULl，否则会断言失败。  
 */
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
