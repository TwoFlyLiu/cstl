/*!
 * \file str.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于字符串的常用操作
 * 
 * 它的作用就是c++中std::string所扮演的角色。内部使用的算法和vec容器使用的是相同的算法，所以对于字符串的
 * 连接性能是非常高的，对于在指定位置插入，性能不是很高
 */

#ifndef INCLUDE_STR_H_H
#define INCLUDE_STR_H_H

#include "cstl_stddef.h"
#include "vec.h"

struct CSTLString; //!< 表示的是本地平台字符串类型
typedef struct CSTLString str_t; //!< CSTLString的一个别名

// 工厂方法(内部支持utf-8字符集)

/*!
 *  \brief str_new 新建一个空的字符串
 *  \retval 返回新的字符串
 *  \note 返回的指针在不再需要的时候，要调用str_free函数来进行删除，否则会造成内存泄露。
 */
CSTL_LIB str_t * str_new();

/*!
 *  \brief str_new_from 从一个现有的字符串来新建一个字符串
 *  \param [in] str 一个以'\0'终止的字符串
 *  \retval 返回新的字符串
 *  \note 返回的指针在不再需要的时候，要调用str_free函数来进行删除，否则会造成内存泄露。
 */
CSTL_LIB str_t * str_new_from(const char *str);

/*!
 *  \brief str_new_from_limit 从一个现有字符串来创建一个字符串
 *  \param [in] str 一个字符串
 *  \param [in] len str的长度
 *  \retval 返回新的字符串，字符串的内容是str[0] ... str[len-1] 
 *  \note 返回的指针在不再需要的时候，要调用str_free函数来进行删除，否则会造成内存泄露。
 */
CSTL_LIB str_t * str_new_from_limit(const char *str, int len);

/*!
 *  \brief str_format 使用一个格式化字符串来生成指定的字符串 
 *  
 *  类似于sprintf, 只是它最终是将字符串保存到str_t内部
 *  
 *  \param [in] fmt 格式化参数，具体的格式字符串和printf, sprintf的格式化字符串完全一致。
 *  \param [in] ... 可变参数，来填充前面指定的格式化内容。
 *  \retval 返回新的字符串
 *  \note 返回的指针在不再需要的时候，要调用str_free函数来进行删除，否则会造成内存泄露。
 */
CSTL_LIB str_t * str_format(const char *fmt, ...);

/*!
 *  \brief str_free 释放掉有str_new相关函数返回的str_t类型内部所占用的所有资源。 
 *  \param [in] str str_t字符串
 *  \note str不能为NULL，否则会断言失败，当次函数执行后，str就不再有效了，再次对于str的任何存取操作，
 *  可能会造成程序奔溃。
 */
void str_free(str_t *str);

// 访问内部信息

/*!
 * \brief str_byte_size 获取字符串内部所占用的字节数目 
 * \param [in] str str_t字符串
 * \retval 返回字符串内部所占用的字节数目 
 * \note str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t str_byte_size(const str_t *str);

/*!
 *  \brief str_char_size 获取字符串内所有的字符数目
 *  \param [in] str str_t字符串
 *  \retval 返回字符串内部所有的字符串数目 
 *  \note str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t str_char_size(const str_t *str);

/*!
 *  \brief str_capacity 获取字符串内部现有内存总共可以存放的字节数目
 *  \param [in] str str_t字符串
 *  \retval 返回字符串内部现有内存总共可以存放的字节数目 
 *  \note str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t str_capacity(const str_t *str);

/*!
 *  \brief str_append 尾部添加一个'\0'结尾的字符串到str中
 *  \param [in,out] str str_t字符串
 *  \param [in] raw 以'\0'结尾的字符串
 *  \retval 返回str
 *  \note str不能为NULL, 否则会断言失败
 */
CSTL_LIB str_t* str_append(str_t *str, const char *raw);

CSTL_LIB str_t* str_push_back(str_t *str, char ch);

/*!
 *  \brief str_insert 在str的指定位置来插入一个'\0'结尾的字符串
 *  
 *  如果pos小于0，那么就会在头部进行插入，如果pos大于最大字节长度，那么就会在尾部进行插入
 *  否则就会在指定位置来进行插入。
 *  
 *  \param [in,out] str str_t字符串
 *  \param [in] pos 指定位置
 *  \param [in] new_str 以'\0'结尾的字符串
 *  \retval 返回str
 *  \note str不能为NULL, 否则会断言失败
 */
CSTL_LIB str_t* str_insert(str_t *str, int pos, const char *new_str);

/*!
 *  \brief str_erase 移除字符串指定位置上的字符
 *  \param [in] str str_t字符串
 *  \param [in] pos 要移除的位置, 如果pos<0，则会在头部进行移除，如果pos大于或者等于字符串的长度，则会移除尾部的元素
 *  \retval 返回str
 *  \note str不能为NULL，否则断言会失败 
 */
CSTL_LIB str_t* str_erase(str_t *str, int pos);

/*!
 *  \brief str_at_byte 获取指定位置的字节字符
 *  \param [in] str str_t字符串
 *  \param [in] index 指定的位置
 *  \return 如果index是一个有效的位置，那么返回指定位置上的字符串，否则返回-1  
 */
CSTL_LIB char str_at_byte(str_t *str, int index);

/*!
 *  \brief str_sub_str 返回str中的子串
 *  \param [in] str str_t字符串
 *  \param [in] offset 偏移的位置
 *  \param [in] length子串的长度 
 *  \retval 返回指定范围的str的子字符串，并且会自动调整offset为正确的位置。
 */
CSTL_LIB str_t* str_sub_str(str_t *str, int offset, int length);

/*!
 *  \brief str_sub_str_buf 返回str中的子串
 *  \param [in] str str_t字符串
 *  \param [in] offset 偏移的位置
 *  \param [in] length 子串的长度
 *  \param [out] buf 用来保存最后的子字符串
 *  \param [out] buflen buf的长度
 *  \param [out] err 用来接收错误信息
 *  \retval 返回已经写出到buf中的字节数目。 
 *  \note str,buf 不能为NULl，否则会断言失败。当buf长度不够存储str_t的值的时候，*err == ENOMEM, 如果
 *  足够的话，那么*err == 0
 */
CSTL_LIB int str_sub_str_buf(str_t *str, int offset, int length,
        char *buf, int buflen, int *err);

/*!
 *  \brief str_trim 去除str首尾的所有空白字符
 *  \param [in,out] str str_t字符串
 *  \retval 返回str
 *  \note str不能为NULL，否则会断言失败。 
 */
CSTL_LIB str_t* str_trim(str_t *str);

/*!
 *  \brief str_trim 去除str头部的所有空白字符
 *  \param [in,out] str str_t字符串
 *  \retval 返回str
 *  \note str不能为NULL，否则会断言失败。 
 */
CSTL_LIB str_t* str_trim_left(str_t *str);

/*!
 *  \brief str_trim 去除str尾部的所有空白字符
 *  \param [in,out] str str_t字符串
 *  \retval 返回str
 *  \note str不能为NULL，否则会断言失败。 
 */
CSTL_LIB str_t* str_trim_right(str_t *str);

/*!
 *  \brief str_split 用来以指定的正则表达式来分割str中的内容到VEC容器中
 *  \param [in] str str_t字符串
 *  \param [in] regex 正则字符串
 *  \retval 返回分割后的内容到vec_t中，返回的值当不在使用的时候要调用vec_free来进行删除
 *  \note str, regex不能为NULL，否则会断言失败。 
 */
CSTL_LIB VEC*   str_split(str_t *str, const char *regex);

/*!
 *  \brief str_split_lines 按行来切割str中的所有内容
 *  \param [in] str str_t字符串
 *  \retval 返回切割后的内容， 返回的值当不在使用的时候要调用vec_free来进行删除
 *  \note str不能为NULL，否则会断言失败。 
 */
CSTL_LIB VEC*   str_split_lines(str_t *str);

/*!
 *  \brief str_cmp 比较str和rhv是否相同
 *  \param [in] str str_t字符串
 *  \param [in] rhv 以'\0'结尾的字符串
 *  \retval =0 表示str=rhv，<0表示str < rhv, >0表示str > rhv 
 *  \note str, rhv不能为NULL，否则会断言失败。 
 */
CSTL_LIB int    str_cmp(str_t *str, const char *rhv);

////////////////////////////////////////测试函数//////////////////////////////

/*!
 *  \brief str_contains 检测字符串中是否包含某个子字符串
 *  \param [in] str str_t字符串
 *  \param [in] sub 子字符串
 *  \retval 如果包含返回true, 否则返回false
 *  \note str, sub不能为NULL，否则会断言失败。 
 */
CSTL_LIB bool   str_contains(const str_t *str, const char *sub);

/*!
 *  \brief str_equals 检测两个字符串是否相等
 *  \param [in] str str_t字符串 
 *  \param [in] rhv 另一个以'\0'结尾的字符串
 *  \retval 相等返回true，否则返回false
 *  \note str, rhv不能为NULL，否则会断言失败。 
 */
CSTL_LIB bool   str_equals(const str_t *str, const char *rhv);

/*!
 *  \brief str_equals_ignore_case 忽略大小写来比较两个字符串是否相等
 *  \param [in] str str_t字符串
 *  \param [in] rhv另一个以'\0'结尾的字符串
 *  \retval 如果两者在忽略大小写的情况下，相等，则返回true, 否则返回false
 *  \note str, rhv不能为NULL，否则断言会失败 
 */
CSTL_LIB bool   str_equals_ignore_case(const str_t *str, const char *rhv);

/*!
 *  \brief str_empty 检测字符串是否不包含任何字符
 *  \param [in] str str_t字符串
 *  \retval 如果str不包含任何字符串则返回true, 否则返回false. 
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB bool   str_empty(const str_t *str);

/*!
 *  \brief str_matches 一个正则表达式校验函数
 *  \param [in] str str_t字符串
 *  \param regex 正则表达式字符串
 *  \retval 如果str匹配regex，则返回true，否则返回false。 
 *  \note str, regex不能为NULL，否则断言失败。
 */
CSTL_LIB bool   str_matches(str_t *str, const char *regex);

/*!
 *  \brief str_starts_with 测试字符串是否以某个子串开头
 *  \param [in] str str_t字符串
 *  \param [in] value 以'\0'结尾的子串
 *  \retval 如果str是一value开头，那么返回true，否则返回false
 *  \note str, value不能为NULL，否则断言失败。 
 */
CSTL_LIB bool   str_starts_with(const str_t *str, const char *value);

/*!
 *  \brief str_c_str 返回str内部保存的字符串
 *  \param [in] str str_t字符串
 *  \retval 返回str内部保存的字符串
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB char*  str_c_str(str_t *str);

/*!
 *  \brief str_hash_code 返回str字符串的hash值
 *  \param [in] str str_t字符串
 *  \retval 返回str字符串的hash值
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB unsigned int    str_hash_code(str_t *str);

/*!
 *  \brief str_ptr_hash_code 计算字符串的hash值
 *  \param [in] str str_t**类型, 也表示一种字符串
 *  \retval 返回真正内部字符串的hash值
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB unsigned int    str_ptr_hash_code(str_t **str);

/*!
 *  \brief str_assign 赋值函数
 *  
 *  直接将str设置为新的值，可以重新利用str中的内存，是个非常方法的函数。
 *  
 *  \param [in,out] str str_t字符串
 *  \param [in] rhv 另一个以'\0'结尾的字符串
 *  \retval 返回str
 *  \note str, rhv不能为NULL，否则会断言失败。 
 */
CSTL_LIB str_t* str_assign(str_t *str, const char *rhv);

// 查找

/*!
 *  \brief str_index_of 查找子串在str中的位置
 *  \param [in] str str_t字符串
 *  \param [in] from_index 查找的起始位置
 *  \param [in] 要查找的字符串
 *  \retval 如果找到则返回索引位置，否则返回-1
 *  \note str, val不能为NULL，否则会断言失败。 
 */
CSTL_LIB int    str_index_of(const str_t *str, int from_index, const char *val);

/*!
 *  \brief str_index_of_char 查找字符在str中的位置
 *  \param [in] str str_t字符串
 *  \param [in] from_index 查找的起始位置
 *  \param [in] val 要查找的字符
 *  \retval 如果找到返回返回字符在字符串中的索引位置，否则返回-1
 *  \note str不能为NULL，否则会断言失败。 
 */
CSTL_LIB int    str_index_of_char(const str_t *str, int from_index, char val);

/*!
 *  \brief str_rindex_of 反向查找子串在字符串中的位置
 *  \param [in] str str_t字符串
 *  \param [in] from_index 查找的起始位置
 *  \param [in] val 要查找的子串
 *  \retval 如果找到则返回子串在字符串中的位置，否则返回-1
 *  \note str, val不能为NULL，否则会断言失败。
 */
CSTL_LIB int    str_rindex_of(const str_t *str, int from_index, const char *val);

/*!
 *  \brief str_rindex_of 反向查找字符在字符串中的位置
 *  \param [in] str str_t字符串
 *  \param [in] from_index 查找的起始位置
 *  \param [in] val 要查找的字符
 *  \retval 如果找到则返回子串在字符串中的位置，否则返回-1
 *  \note str, val不能为NULL，否则会断言失败。
 */
CSTL_LIB int    str_rindex_of_char(const str_t *str, int from_index, char val);

/*!
 *  \brief str_replace 替换字符串中的内容
 *  \param [in,out] str str_t字符串
 *  \param [in] old_str 老的子串
 *  \param [in] new_str 新的子串
 *  \retval str
 *  \note str, old_str, new_str不能为NULL，否则会断言失败。 
 */
CSTL_LIB str_t*  str_replace(str_t *str, const char *old_str, const char *new_str);

/*!
 *  \brief str_replace_byte 替换字符串中的指定字符
 *  \param [in,out] str str_t字符串
 *  \param [in] old_ch 原来的字符
 *  \param [in] new_ch 新的字符 
 *  \retval str
 *  \note str不能为NULL，否则会断言失败。
 */
CSTL_LIB str_t*  str_replace_byte(str_t *str, const char old_ch, char new_ch);

/*!
 *  \brief str_replace_first替换字符串第一次出现的子串为新的子串
 *  
 *  offset如果小于，那么此函数会将offset设置为0。
 *  
 *  \param [in,out] str str_t字符串
 *  \param [in] offset 偏移的位置, 或者说起始替换的位置
 *  \param [in] old_str 要替换的子串
 *  \param [in] new_str 替换成的新的子串
 *  \retval str
 *  \note str, old_str, new_str不能为NULL，否则会断言失败。
 */
CSTL_LIB str_t*  str_replace_first(str_t *str, int offset,  const char *old_str, const char *new_str);

/*!
 *  \brief str_to_lowercase 将str字符串中的大写字母都替换为小写字符串  
 *  \param [in,out] str str_t字符串
 *  \retval str
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB str_t* str_to_lowercase(str_t *str);

/*!
 *  \brief str_to_uppercase 将字符串中的小写字母都替换为大写字母 
 *  \param [in,out] str str_t字符串
 *  \retval str
 *  \note str不能为NULL，否则会断言失败
 */
CSTL_LIB str_t* str_to_uppercase(str_t *str);

/*!
 *  \brief str_resize 重新改变字符串的尺寸
 *  
 *  如果new_size如果小于原来的长度，那么字符串多出来的内容会被截掉，如果new_size大于原来的长度，
 *  那么多出来的内容中的每个字符会被填充为字符ch。
 *  
 *  \param [in,out] str str_t字符串
 *  \param new_size 新字符串的尺寸
 *  \retval str
 *  \note str不能为NULL，否则会断言失败。   
 */
CSTL_LIB str_t* str_resize(str_t *str, size_t new_size, char ch);

/*!
 *  \brief str_ptr_destroy 销毁*str锁占用的资源
 *  
 *  是个辅助函数，用来方便讲str_t类型存储在其他容器中，并且很方便地进行销毁
 *  \param str str_t**字符串
 *  \retval none.
 */
CSTL_LIB void str_ptr_destroy(str_t **str);

#define str_vec_get(vec, index) (*(str_t**)vec_get(vec, index)) //!< 获取str_t*的vec容器中，指定索引位置的str_t*值

#endif //INCLUDE/STR_H_H
