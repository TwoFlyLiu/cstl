/*!
 * \file vec.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了vec_t的所有api函数。
 */

#ifndef VEC_H_H
#define VEC_H_H

#include "cstl_stddef.h"

/*!
 * \brief 一个顺序容器类型，如果熟悉c++的话，相当于c++中的vector
 * 
 * vec_t是一个线性存储的容器，使用它可以执行一系列对于元素的增删改和遍历操作。
 * 
 * 因为使用线性存储所以有一下好处：
 * - 可以没有性能损耗使用索引来访问指定的元素，
 * - 对于有序的数列，可以无损耗地使用二分法进行查找
 * - 执行尾插几乎就相当于对于数组元素的赋值操作，效率非常高，时间复杂度是O(1)
 *   
 * 同样使用线性存储有以下缺点：
 * - 对于在非尾部位置进行插入删除操作，性能影响非常大
 *   
 * 所以如果经常执行在非尾部位置进行插入删除，那么就不要使用线性存储容器，应该使用链式存储容器list_t.
 */
typedef struct {
    void *beg;                      //!< 内部数据第一个元素的地址
    int len;                        //!< 内部容器元素的数目
    int unit_size;                  //!< 单个元素的内存尺寸
    int capacity;                   //!< 当前容器的内存最多可以存储元素的数目
    destroy_func_t destroy_func;    //!< 当元素销毁时候，调用的函数
} VEC, vec_t;

/*!
 *  \brief vec_foreach的毁掉函数参数
 *  
 *  \param [in,out] value vec_t容器中，某个元素的地址，如果你存放的元素类型是int类型，那么value的具体类型就是
 *  int*, 如果你存放的元素类型是double, 那么value的具体类型就是double*, 如果你存放的元素类型是int*，那么value
 *  的具体类型就是int**, 以此类推。
 *  \param [in,out] user_data 用户传入过来的数据，可以是任何类型。
 */
typedef void (*VEC_FOREACH_FUNC)(void *value, void *user_data);
typedef VEC_FOREACH_FUNC vec_foreach_func_t; //!< VEC_FOREACH_FUNC别名，主要用来统一的类型命名

/*!
 * \brief vec_new 用来构造一个带有默认容量的vec_t, 默认容量是10。
 * \param [in] unit_size 单个元素的尺寸（即单个元素锁占用的内存大小）, 如果你存放的元素类型是int，那么这个值就应该是
 * sizeof(int), 如果你存放的元素类型是double, 那么这个值就应该是sizeof(double)。
 * \param [in] destroy 销毁元素时候调用的函数
 * \retval 新的vec_t容器实例，以后所有对于vec_t容器的操作，都要通过这个指针来进行。
 * 
 * \note 当不在使用时候，需要调用vec_free来销毁此容器，否则会造成内存泄露
 */
CSTL_LIB VEC *vec_new(int unit_size, destroy_func_t destroy);

/*!
 * \brief vec_new_with_capacity 用来构造一个带有指定初始容量的vec_t
 * \param [in] unit_size 单个元素的尺寸（即单个元素锁占用的内存大小）, 如果你存放的元素类型是int，那么这个值就应该是
 * sizeof(int), 如果你存放的元素类型是double, 那么这个值就应该是sizeof(double)。
 * \param [in] destroy 销毁元素时候调用的函数
 * \param [in] capacity 容器的初始化容量
 * \retval 新的vec_t容器实例，以后所有对于vec_t容器的操作，都要通过这个指针来进行。
 * 
 * \note 当不在使用时候，需要调用vec_free来销毁此容器，否则会造成内存泄露
 */
CSTL_LIB VEC *vec_new_with_capacity(int unit_size, destroy_func_t destroy, int capacity);

/*!
 * \brief 销毁vec_t实例内存锁占用的所有资源
 * 
 * 在销毁所有资源前，如果指定了destroy_func_t类型的函数的话，那么会以每个元素的指针为参数来调用这个函数，
 * 然后会释放掉，内部所分配的资源。
 * 
 * \param [in] vec 要释放的vec_t容器实例
 * \retval none.
 * \note 当调用过此函数后，vec这个指针就不在有效，不能在使用此指针，否则行为是未知。vec绝对不能为空，否则会
 * 断言失败。
 */
CSTL_LIB void vec_free(VEC *vec);

// 访问修改容量信息

/*!
 * \brief 获取vec_t实例的当前容量，因为容量是根据需求变化的，所以多次调用，它的返回值未必相同。
 * \param [in] vec vec_t实例
 * \retval 当前容器的最多可以存储的元素数目，当超过这个数目的时候，容器内部就会重新分配内存，
 * 这个容量值就会变大。 
 * \note vec 绝对不能为空，否则内部会断言失败的
 */
CSTL_LIB int vec_capacity(const VEC *vec);

/*!
 * \brief 检测vec_t容器是否为空（就是内部使用有元素）
 * \param [in] vec vec_t实例
 * \retval 如果vec_t容器内部没有元素，则返回true, 否则返回false
 * \note vec 绝对不能为空，否则内部会断言失败的
 */
CSTL_LIB bool vec_empty(const VEC *vec);

/*!
 * \brief 获取vec_t容器当前元素的数目
 * \param [in] vec vec_t实例
 * \retval 返回vec_t容器当前元素的数目
 * \note vec 绝对不能为空，否则内部会断言失败的
 */
CSTL_LIB int vec_size(const VEC *vec);

/*!
 * \brief 重新改变vec_t容器元素的数目
 * 
 * 如果new_size比原来的元素数目多的话，那么多出来的元素值会被赋值成value, 如果new_size比原来元素数目少的话，
 * 那么容器中索引为new_size以后的元素就会被删除掉（如果指定destroy_func_t，那么元素在删除之前，会被作为参数来
 * 调用这个函数的）。
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] new_size 新的元素数目
 * \param [in] value 多出来的元素的默认值
 * \retval none.
 * 
 * \note vec 绝对不能为空，否则内部会断言失败的
 */
CSTL_LIB void vec_resize(VEC *vec, int new_size, const void *value);

// 增加

/*!
 * \brief 在容器头部插入一个元素
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] elem 要插入的元素值
 * \retval none.
 * 
 * \note vec 绝对不能为空，否则内部会断言失败的。这个操作对于性能影响非常大。
 */
CSTL_LIB void vec_push_front(VEC *vec, const void *elem);

/*!
 * \brief 在容器尾部插入一个元素
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] elem 要插入的元素值
 * \retval none.
 * 
 * \note vec 绝对不能为空，否则内部会断言失败的, 这个操作对于性能几乎没有影响。
 */
CSTL_LIB void vec_push_back(VEC *vec, const void *elem);

/*!
 * \brief 在容器任意一个位置进行插入
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] index 要插入的索引位置
 * \param [in] elem 要插入的元素值
 * \retval none.
 * 
 * \note vec 绝对不能为空（NULL），否则会断言失败。如果index < 0, 那么会在头部进行插入，如果index >= vec_size(vec),
 * 那么会在尾部进行插入。这个操作在非尾部插入，对于性能影响比较大。
 */
CSTL_LIB void vec_insert(VEC *vec, int index, const void *elem);

/*!
 * \brief 将一个vec_t容器中的所有元素插入到当前vec_t容器中的指定位置。
 * 
 * 比如：
 * ```cpp
 * vec_t *vec = vec_new(sizeof(int), NULL);
 * vec_t *vec2 = vec_new(sizeof(int), NULL);
 * 
 * for (int i = 0; i < 5; i++) {
 *     vec_push_back(vec, &i); 
 * }
 * 
 * for (int i = 8; i < 10; i++) {
 *     vec_push_back(vec2, &i); 
 * }
 * 
 * // vec值是[0, 1, 2, 3, 4]
 * // vec2值是[8, 9]
 * 
 * vec_extend(vec, 1, vec2); //vec值是：[0, 8, 9, 1, 2, 3, 4]
 * 
 * vec_free(vec2); 
 * vec_free(vec); 
 * ```
 * 
 * \param [in,out] vec vec_t实例 
 * \param [in] index 要插入的索引位置
 * \param [in] insert_vec 内部所有元素将会被插入到vec中
 * \retval none.
 * 
 * \note vec 绝对不能为空（NULL）， insert_vec 也不能为空，否则会断言失败。如果index < 0, 那么会在头部进行插入，如果index >= vec_size(vec),
 * 那么会在尾部进行插入。这个操作在非尾部插入，对于性能影响比较大。
 */
CSTL_LIB void vec_extend(VEC *vec, int index, const VEC *insert_vec);

/*!
 * \brief 将一个vec_t容器中的所有元素头部插入到vec容器中
 * 
 * 比如：
 * ```cpp
 * vec_t *vec = vec_new(sizeof(int), NULL);
 * vec_t *vec2 = vec_new(sizeof(int), NULL);
 * 
 * for (int i = 0; i < 5; i++) {
 *     vec_push_back(vec, &i); 
 * }
 * 
 * for (int i = 8; i < 10; i++) {
 *     vec_push_back(vec2, &i); 
 * }
 * 
 * // vec值是[0, 1, 2, 3, 4]
 * // vec2值是[8, 9]
 * 
 * vec_extend_front(vec, vec2); //vec值是：[8, 9, 0, 1, 2, 3, 4]
 * 
 * vec_free(vec2); 
 * vec_free(vec); 
 * ```
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] insert_vec 内部所有元素将会被插入到vec中
 * \note vec, insert_vec都不能为空，否则会被断言失败
 */
CSTL_LIB void vec_extend_front(VEC *vec, const VEC *insert_vec);

/*!
 * 将一个vec_t中的所有元素尾部插入到vec_t中
 * 
 * 比如：
 * ```cpp
 * vec_t *vec = vec_new(sizeof(int), NULL);
 * vec_t *vec2 = vec_new(sizeof(int), NULL);
 * 
 * for (int i = 0; i < 5; i++) {
 *     vec_push_back(vec, &i); 
 * }
 * 
 * for (int i = 8; i < 10; i++) {
 *     vec_push_back(vec2, &i); 
 * }
 * 
 * // vec值是[0, 1, 2, 3, 4]
 * // vec2值是[8, 9]
 * 
 * vec_extend_back(vec, vec2); //vec值是：[0, 1, 2, 3, 4, 8, 9]
 * 
 * vec_free(vec2); 
 * vec_free(vec); 
 * ```
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] insert_vec 内部所有元素将会被插入到vec中
 * \note vec, insert_vec都不能为空，否则会被断言失败
 */
CSTL_LIB void vec_extend_back(VEC *vec, const VEC *insert_vec);


//  访问/修改

/*!
 * \bref 获取vec容器中指定的索引位置上的元素的地址
 * \param [in] vec vec_t实例
 * \param [in] index 索引位置
 * \retval 如果index是有效索引位置，则返回此索引位置上的元素的地址，否则返回NULL。
 * \note vec不能为空，否则会断言失败
 */
CSTL_LIB void *vec_get(VEC *vec, int index);

/*!
 * \bref 获取vec容器中指定的索引位置上的元素的地址
 * 
 * 他和vec_get具有同样的功能，这个函数存在主要是为了模拟c++ std::vector::at方法 
 * 
 * \param [in] vec vec_t实例
 * \param [in] index 索引位置
 * \retval 如果index是有效索引位置，则返回此索引位置上的元素的地址，否则返回NULL。
 * \note vec不能为空，否则会断言失败
 */
void *vec_at(VEC *vec, int index);

/*!
 * \brief 设置vec容器中指定元素的值
 * \param [in,out] vec vec_t实例
 * \param [in] index 索引位置
 * \retval none.
 * \note vec不能为NULL，否则会断言失败。index必须是有效的索引位置，否则也会断言失败。
 */
void vec_set(VEC *vec, int index, const void *value);

/*!
 * \brief 获取vec容器中第一个元素的地址
 * \param [in] vec vec_t实例
 * \retval 如果容器内没有任何元素，那么返回NULL, 否则返回第一个元素的地址。
 * \note vec不能为NULL，否则会断言失败
 */
CSTL_LIB void *vec_front(VEC *vec);

/*!
 * \brief 获取vec容器中最后一个元素的地址
 * \param [in] vec vec_t实例
 * \retval 如果容器内部没有任何元素，否则NULL, 否则返回最后一个元素的地址。
 * \note vec不能为NULL，否则会断言失败
 */
CSTL_LIB void *vec_back(VEC *vec);

// 移除

/*!
 * \brief 移除vec容器中的尾部元素
 * 
 * 如果容器非空并且destroy_func_t设置的话，则会将尾部元素指针作为参数来调用此函数
 * 
 * \param [in,out] vec vec_t实例
 * \retval none.
 * \note vec不能为NULL，否则会断言失败
 */
CSTL_LIB void vec_pop_back(VEC *vec);

/*!
 * \brief 移除vec容器中的头部元素，如果destroy_func_t函数设置的话，那么会调用此函数
 * 
 * 如果容器非空并且destroy_func_t设置的话，则会将头部元素指针作为参数来调用次函数
 * 
 * \param [in,out] vec vec_t实例
 * \retval none.
 * \note vec不能为NULL，否则会断言失败
 */
CSTL_LIB void vec_pop_front(VEC *vec);

/*!
 * \brief 移除vec容器中第一个和value等值的元素。
 * 
 * 如果值找到的话，并且destroy_func_t设置的话，则会将找到的元素的地址作为参数调用此函数
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] value 要移除的值
 * \param [in] compare 比较函数，如果(*compare)(elem, value) == 0，则说明相等，否则说明不相等
 * \retval none.
 * \note vec, compare不能为NULL, 否则会断言失败。
 */
CSTL_LIB void vec_remove(VEC *vec, const void *value, cmp_func_t compare); //移除第一个出现的值

/*!
 * \brief 移除vec容器中所有和value等值的元素。
 * 
 * 如果值找到的话，并且destroy_func_t设置的话，则会将找到的元素的地址作为参数调用此函数
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] value 要移除的值
 * \param [in] compare比较函数, 如果(*compare)(elem, value) == 0，则说明相等，否则说明不相等
 * \retval none.
 * \note vec, compare不能为NULL, 否则会断言失败。
 */
CSTL_LIB void vec_remove_all(VEC *vec, const void *value, cmp_func_t compare);

/*!
 * \brief 移除vec容器中指定索引位置上的元素。
 * 
 * 如果指定索引存在，并且destroy_func_t设置的话，那么会将此索引位置的元素地址作为参数调用次函数
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] index 要删除的索引位置
 * \retval none.
 * \note vec 不能为NULL, index必须是有效的位置，否则会断言失败
 */
CSTL_LIB void vec_erase(VEC *vec, int index);

/*!
 * \brief 移除vec容器中所有的元素
 * 
 * 如果vec非空的话，并且destroy_func_t设置的话，那么会挨个将所有元素地址作为参数调用此函数。
 * 
 * \param [in,out] vec vec_t实例
 * \retval none.
 * \note vec不能为NULL，否则会断言失败
 */
CSTL_LIB void vec_clear(VEC *vec); //移除内部所有元素

// 查找

/*!
 * \brief 查找vec容器中第一个和val等值的元素
 * \param [in] vec vec_t实例
 * \param [in] val 要查找的值
 * \param [in] compare 比较函数, 如果(*compare)(&elem, value) == 0，则说明相等，否则说明不相等
 * \retval 如果找到的话，则返回元素的地址，否则返回NULL.
 * \note vec和compare必须不能为NULL, 否则会断言失败。
 */
CSTL_LIB void *vec_find(VEC *vec, const void *val, cmp_func_t compare);

/*!
 * \brief 使用二分法查找vec容器中和val等值的元素。
 * \param [in] vec vec_t实例
 * \param [in] val要查找的值
 * \param [in] compare 比较函数, 如果(*compare)(&elem, value) == 0，则说明相等，否则说明不相等
 * \retval 如果找到的话，则返回元素的地址，否则返回NULL.
 * \note vec和compare必须不能为NULL, 否则会断言失败。在调用这个函数前，必须要保证vec容器内部元素是有序的，
 * 所以请在调用这个之前，如果不确定vec容器内部是否有序，那么请调用vec_sort函数。
 */
CSTL_LIB void *vec_bin_find(VEC *vec, const void *val, cmp_func_t compare);

// 排序（使用快速排序算法）

/*!
 * \brief 使用快速排序算法来排序vec内部所有的元素
 * \param [in,out] vec vec_t实例
 * \param [in] compare 比较函数, 如果(*compare)(&elem1, &elem2) == 0，则说明相等，否则说明不相等
 * \retval none.
 * \note vec, compare都不能为NULL，否则断言失败。
 */
CSTL_LIB void vec_sort(VEC *vec, cmp_func_t compare);

/*!
 * \brief 交换vec容器中两个索引位置上的值
 * \param [in,out] vec vec_t实例
 * \param [in] idx1 第一个索引位置
 * \param [in] idx2 第二个索引位置
 * \retval none.
 * \note VEC必须不能为空，idx1, idx2必须是有效的索引范围否则会断言失败
 */
CSTL_LIB void vec_swap(VEC *vec, int idx1, int idx2);

/*!
 * \brief 遍历vec容器中的所有元素
 * 
 * 内部算法是：
 * ```cpp
 * for (int i = 0; i < vec_size(vec); i++) {
 *     (*vec_foreach_func)(vec_get(i), user_data);
 * }
 * ```
 * 
 * \param [in,out] vec vec_t实例
 * \param [in] vec_foreach_func 遍历回调函数
 * \param [in,out] user_data 额外参数
 * \retval none.
 * \note vec和vec_foreach_func都不能为NULL, 否则会断言失败。
 */
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
    static inline void type##_vec_remove_all(VEC *vec, type value) {\
        vec_remove_all(vec, &value, CSTL_NUM_CMP_FUNC(type));\
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
    static inline void type##_vec_remove_all(VEC *vec, type value) {\
        vec_remove_all(vec, &value, CSTL_NUM_CMP_FUNC(type));\
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
    static inline void unsigned_##type##_vec_remove_all(VEC *vec, unsigned type value) {\
        vec_remove_all(vec, &value, CSTL_UNSIGNED_CMP_FUNC(type));\
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
    static inline void type##_vec_remove_all(VEC *vec, type *value) {\
        vec_remove_all(vec, value, &cmp_func);\
    }\
    static inline void type##_vec_remove_func(VEC *vec, type *value, cmp_func_t compare) {\
        vec_remove(vec, value, compare);\
    }\
    static inline void type##_vec_remove_all_func(VEC *vec, type *value, cmp_func_t compare) {\
        vec_remove_all(vec, value, compare);\
    }\
    static inline void type##_vec_sort(VEC *vec) {\
        vec_sort(vec, &cmp_func); \
    }\
    static inline void type##_vec_sort_func(VEC *vec, cmp_func_t compare) {\
        vec_sort(vec, compare); \
    }

#undef DEFINE_NUM_TYPE_VEC
#undef DEFINE_FLOAT_NUM_TYPE_VEC
#undef DEFINE_UNSIGNED_NUM_TYPE_VEC

#endif //VEC_H_H
