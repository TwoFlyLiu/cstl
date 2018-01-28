/*!
 * \file hmap.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了hset_t的所有api函数。
 */

#ifndef HSET_H_H
#define HSET_H_H

#include "hmap.h"

/*!
 * \brief 一个集合容器
 * 
 * 内部使用hash map来存储操作所有数据，性能非常高。
 */
typedef struct {
    hmap_t *data; //!< 内部保存所有数据
} hset_t, HSET;

/*!
 *  \brief 新建一个hset_t实例
 *  \param [in] unit_size 表示元素值的所占内存的尺寸，如果值是int类型，那么该值就是sizeof(int), 如果元素类型是int*类型，
 *  那么该值就是sizeof(int*), 以此类推。
 *  \param [in] hash_func 元素值所关联的hash函数。
 *  \param [in] key_cmp 用来比较两个元素的值是否相等，否则0表示相等，非0表示不等。
 *  \retval 返回的hset实例。
 *  \note 当返回的hset实例，不在使用的时候，要记得调用hset_free函数，否则会造成内存泄露。
 */
hset_t *hset_new(int unit_size, hash_func_t hash_func, cmp_func_t key_cmp);

/*!
 *  \brief 销毁有hset_new创建的hset_t实例
 *  \param [in,out] hset hset_t实例
 *  \retval none.
 *  \note hset不能为NULL，否则会断言失败。当调用此函数后，hset就不能够在使用的，否则会造成程序奔溃。
 */
void hset_free(hset_t *hset);

/*!
 *  \brief 插入一个值到hset_t实例中
 *  \param [in,out] hset hset_t实例
 *  \param [in] value 一个值的指针
 *  \retval none.
 *  \note hset, value不能为NULL，否则会断言失败。
 */
void hset_insert(hset_t *hset, const void* value);

/*!
 *  \brief 删除hset容器中所有的元素
 *  \param [in,out] hset hset_t实例
 *  \param [in] value 要删除值的指针
 *  \retval none.
 *  \note hset, value不能为NULL，否则会断言失败。 
 */
void hset_erase(hset_t *hset, const void *value);

/*!
 *  \brief 查找hset容器中一个元素值在容器中的地址。
 *  \param [in,out] hset hset_t实例
 *  \param [in] value 一个值的指针, 注意，他只是一个普通值的指针，是用来进行在hset中进行搜索查找的值
 *  \retval 如果找到，则返回所对应值的地址，否则返回NULL。
 *  \note hset, value不能为NULL，否则会断言失败。
 */
void *hset_find(hset_t *hset, const void *value);

/*!
 *  \brief 检测hset容器中是否包含指定的值
 *  \param [in,out] hset hset_t实例
 *  \param [in] value 要查找的值。
 *  \retval 如果找到返回true, 否则返回false。
 *  \note hset, value不能为NULL，否则会断言失败。
 */
bool hset_contains(hset_t *hset, const void *value);

/*!
 *  \brief 删除hset容器中所有的值
 *  \param [in,out] hset hset_t实例
 *  \retval none.
 *  \note hset不能为NULL，否则会断言失败。
 */
void hset_clear(hset_t *hset);

/*!
 *  \brief 检测hset中是否不包含任何元素
 *  \param [in,out] hset hset_t实例
 *  \note hset不能为NULL，否则会断言失败。
 */
bool hset_empty(hset_t *hset);

/*!
 *  \brief 获取hset容器中元素的数目。
 *  \param [in,out] hset hset_t实例
 *  \retval 返回hset容器中元素的数目。
 *  \note hset不能为NULL，否则会断言失败。
 */
int hset_size(hset_t *hset);

#endif //HSET_H_H
