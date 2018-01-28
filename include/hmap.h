/*!
 * \file hmap.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了hmap_t的所有api函数。
 */

#ifndef HMAP_H_H
#define HMAP_H_H

#define BUCKET_SIZE 255

#include "cstl_stddef.h"
#include "vec.h"

// 计算hash值
typedef hash_func_t HMAP_HASH; //!< hash函数指针类型别名

// 比较两个Key是否相等，0表示相等，非0表示不等
typedef cmp_func_t HMAP_KEY_CMP; //!< 比较函数指针类型别名

/*!
 * \brief hmap_for_each参数类型
 * \param [in] key 表示元素的key, 这个值不能修改
 * \param [in,out] value 表示元素的value
 * \param [in,out] user_data 用户可以指定的额外参数
 * \retval none.
 */
typedef void (*HMAP_FOR_EACH)(const void *key, void *value, void *user_data);

/*! 
 * \brief 一个关联容器，类似于java中的HashMap
 * 
 * 内部使用的是hash表来维护所有的数据，相比较于使用rb_tree来管理数据，他的插入性能还是非常高的，
 * 查找性能在数据冲突非常严重的情况下，性能没有rb_tree高，但是在冲突不严重的，查找性能应该比
 * 使用rb_tree性能还要高。所以说使用它，是一种非常非常高效的选择。
 */
typedef struct {
    VEC* buckets[BUCKET_SIZE];      //!< 一个hash表
    int key_size;                   //!< key的占用内存的尺寸
    int value_size;                 //!< value占用内存的尺寸
    int len;                        //!< 元素的数目
    hash_func_t hash_func;          //!< key的hash函数指针
    cmp_func_t key_cmp_func;        //!< key的比较函数指针
    destroy_func_t key_destroy;     //!< key的销毁回调函数指针
    destroy_func_t val_destroy;     //!< value的销毁回调函数指针
}hmap_t, HMAP;


// 工厂函数

/*!
 * \brief hmap_t的工厂函数
 * 
 * 所有的hmap函数的第一个参数都要使用此函数或者hmap_new_with_destroy_func返回的值来进行调用
 * 
 * \param [in] key_size key所占用内存的尺寸，如果key是int类型，那么这个值就是sizeof(int), 如果key类型是char类型，那么
 * 这个值就是sizeof(char)
 * \param [in] value_size value所占用内存的尺寸, 如果value是int类型，那么这个值就是sizeof(int), 如果value类型是char, 那么
 * 这个值就是sizeof(char), 以此类推。
 * \param [in] hash_func key的hash函数
 * \param [in] key_cmp_func key的比较函数
 * \retval 如果成功的话返回新的hmap_t实例 ，否则返回NULL
 * \note hash_func, key_cmp_func必须要设置，否则会断言失败, 会返回NULL。如果返回的指针不在使用的话，那么要调用hmap_free
 * 函数来释放掉内部所占用的资源, 否则会出现内存泄露。
 */
CSTL_LIB HMAP *hmap_new(int key_size, int value_size, 
        hash_func_t hash_func,
        cmp_func_t key_cmp_func);

/*!
 * \brief hmap_t的工厂函数
 * 
 * 所有的hmap函数的第一个参数都要使用此函数或者hmap_new返回的值来进行调用
 * 
 * \param [in] key_size key所占用内存的尺寸，如果key是int类型，那么这个值就是sizeof(int), 如果key类型是char类型，那么
 * 这个值就是sizeof(char)
 * \param [in] value_size value所占用内存的尺寸, 如果value是int类型，那么这个值就是sizeof(int), 如果value类型是char, 那么
 * 这个值就是sizeof(char), 以此类推。
 * \param [in] hash_func key的hash函数
 * \param [in] key_cmp_func key的比较函数
 * \param [in] key_destroy key销毁函数, 当此函数不为空，那么当销毁某个元素的时候，那么这个函数就会被调用
 * \param [in] val_destroy value销毁函数, 当此函数不为空，那么当销毁某个元素的时候，那么这个函数就会被调用
 * \retval 如果成功的话返回新的hmap_t实例 ，否则返回NULL
 * \note hash_func, key_cmp_func必须要设置，否则会断言失败, 会返回NULL。如果返回的指针不在使用的话，那么要调用hmap_free
 * 函数来释放掉内部所占用的资源, 否则会出现内存泄露。
 */
CSTL_LIB HMAP *hmap_new_with_destroy_func(int key_size, int value_size, 
        hash_func_t hash_func,
        HMAP_KEY_CMP key_cmp_func,
        destroy_func_t key_destroy,
        destroy_func_t val_destroy);
/*!
 *  \brief 销毁所有的元素释放掉内部所占用的内存
 *  
 *  如果key_destroy函数或者val_destroy函数被设置的话，那么就会让每个元素挨个调用这两个非空的函数。
 *  
 *  \param [in,out] hmap hmap_t实例
 *  \retval none.
 *  \note hmap不能为NULL，否则会断言失败。当调用此函数后hmap就不在有效，以后所有对于hmap的操作行为都是未知的。
 */
CSTL_LIB void hmap_free(HMAP *hmap);

/*!
 *  \brief 插入一个新的键值对，如果键已经存在的话，则会替换掉原来的值，并且在调用之前，会调用原来值的销毁函数 
 *  \param [in,out] hmap hmap_t实例
 *  \param [in] key 新键的地址
 *  \param [in] value 新值的地址
 *  \retval none.
 *  \note hmap不能为NULL，否则会断言失败。
 */
CSTL_LIB void hmap_insert(HMAP *hmap, const void *key, const void *value);

// 查找数据

/*!
 *  \brief 获取一个key对应的值
 *  \param [in] hmap hmap_t实例
 *  \param [in] key 键的地址
 *  \retval 返回键对应的值的指针，可以通过这个指针来修改值,如果键在hmap中不存的话，则返回NULL。
 *  \note hmap不能为NULL，key不能为NULL, 否则会断言失败。
 */
CSTL_LIB void *hmap_get(const HMAP *hmap, const void *key);

/*!
 *  \brief 获取一个key对应的值
 *  \param [in] hmap hmap_t实例
 *  \param [in] key 键的地址
 *  \retval 返回键对应的值的指针，可以通过这个指针来修改值,如果键在hmap中不存的话，则返回NULL。
 *  \note hmap和key都不能为NULL, 否则张断言失败。这个方法的功能和hmap_get功能完全一致，只是为了兼容c++的hmap的find api
 */
CSTL_LIB void *hmap_find(const HMAP *hmap, const void *key);

/*!
 *  \brief 查找容器中是否有指定的key
 *  \param [in] hmap hmap_t实例
 *  \param [in] key 键的地址
 *  \retval 如果存在，则返回true, 否则返回false
 *  \note hmap和Key都不能为NULL, 否则会断言失败。  
 */
CSTL_LIB bool hmap_has_key(const HMAP *hmap, const void *key);

// 修改数据

/*!
 *  \brief 设置容器中key所对应的value
 *  \param [in,out] hmap hmap_t实例
 *  \param [in] key 键的地址
 *  \param [in] new_value 新的值的地址
 *  \retval none.
 *  \note hmap, key, value都不能为NULL, 否则会断言失败。 如果原来的值不存在，内部则不会进行任何操作，
 *  并不会添加新的键值对, 如果存在，并且设置了val_destroy, 则会调用此函数，然后才会设置新的值。
 */
CSTL_LIB void hmap_set(HMAP *hmap, const void *key, const void *new_value);

/*!
 * \brief 删除容器中指定的key所对应的键值队。
 * \param [in,out] hmap hmap_t实例
 * \param [in] key 键的地址
 * \retval none.
 * \note hmap, key都不能为NULL, 否则会断言失败。执行此函数，如果key存在的话，那么它所对应的entry就会被删除掉。
 * 在删除之前，如果相应的销毁函数(key_destroy, val_destroy)设置的话，就会调用相应的销毁函数。
 */
CSTL_LIB void hmap_erase(HMAP *hmap, const void *key);

/*!
 *  \brief 删除容器中所有的元素
 *  \param [in,out] hmap hmap_t实例
 *  \retval none.
 *  \note hmap不能为NULL，否则会断言失败。执行此函数，那么hmap的所有元素都会被删除掉，如果设置了销毁函数的，那么
 *  元素在被销毁之前，就会调用相应的销毁函数。
 */
CSTL_LIB void hmap_clear(HMAP *hmap);

/*!
 *  \brief 获取容器中元素的数目
 *  \param [in] hmap hmap_t实例
 *  \retval none.
 *  \note hmap不能为NULL，否则会断言失败。
 */
CSTL_LIB int hmap_size(const HMAP *hmap);

/*!
 *  \brief 检测hmap中是否有没有元素
 *  \param [in] hmap hmap_t实例
 *  \retval 如果容器中没有任何元素，那么返回true, 否则返回false。
 *  \note hmap不能为NULL，否则会断言失败。
 */
CSTL_LIB bool hmap_empty(const HMAP *hmap);

/*!
 *  \brief 用来遍历hmap中所有的元素
 *  ```.c
 *      // 死循环
 *      // 1. 获取下一个key, value
 *      // 2. 如果key == NULL，那么就会跳出循环
 *      // 3. 调用for_each_func(key, value, user_data)
 *      // 4. 执行1
 *  ```
 *  \param [in] hmap hmap_t实例
 *  \retval none. 
 *  \note hmap, for_each_func不能为NULL，否则会断言失败
 */
CSTL_LIB void hmap_for_each(HMAP *hmap, HMAP_FOR_EACH for_each_func, void *user_data);

#endif //HMAP_H_H
