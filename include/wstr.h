/*!
 * \file wstr.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于字符串的常用操作
 * 
 * 它的作用就是c++中std::wstring所扮演的角色。内部使用的算法和vec容器使用的是相同的算法，所以对于字符串的
 * 连接性能是非常高的，对于在指定位置插入，性能不是很高
 */

#ifndef INCLUDE_WSTR_H_H
#define INCLUDE_WSTR_H_H

#include "cstl_stddef.h"
#include "vec.h"

struct CSTLWString;                 //!< 表示宽字符串
typedef struct CSTLWString wstr_t;  //!< CSTLWString的别名，用来统一命名规则

/*!
 *  \brief wstr_new 创建一个长度为0的宽字符串
 *  \retval 返回一个长度为0的宽字符串 
 *  \note 当返回的wstr_t指针不在的使用的时候，要调用wstr_free来进行销毁否则会造成内存泄露
 */
CSTL_LIB wstr_t * wstr_new();

/*!
 *  \brief wstr_new_from 以一个L'\0'结尾的宽字符来新建一个宽字符串
 *  \param [in] wstr 以L'\0'结尾的宽字符串
 *  \retval 返回一个内容是wstr的字符串 
 *  \note 当返回的wstr_t指针不在的使用的时候，要调用wstr_free来进行销毁否则会造成内存泄露
 */
CSTL_LIB wstr_t * wstr_new_from(const wchar_t *wstr);

/*!
 *  \brief wstr_new_from_limit 以一个指定长度的宽字符串来新建一个宽字符串
 *  \param [in] wstr 一个宽字符串
 *  \retval 返回一个内容是wstr[0] .. wstr[len-1]的宽字符串
 *  \note 当返回的wstr_t指针不在的使用的时候，要调用wstr_free来进行销毁否则会造成内存泄露
 */
CSTL_LIB wstr_t * wstr_new_from_limit(const wchar_t *wstr, int len);

/*!
 *  \brief wstr_format 以一个格式化参数来新建一个宽字符串 
 *  \param [in] fmt 格式化字符串，格式样式和wsprintf样式完全一样
 *  \param [in] ... fmt的可变参数
 *  \retval 返回一个新的款字符串
 *  \note 当返回的wstr_t指针不在的使用的时候，要调用wstr_free来进行销毁否则会造成内存泄露
 */
CSTL_LIB wstr_t * wstr_format(const wchar_t *fmt, ...);

/*!
 *  \brief wstr_free 销毁wstr_t内部所有的资源
 *  \param [in,out] wstr wstr宽字符串, 它是使用wstr_new系列函数返回的指针
 *  \retval none.
 *  \note 当此函数调用后，那么wstr指针就不再有效，以后任何对于wstr的存取操作都会造成程序奔溃。
 */
void wstr_free(wstr_t *wstr);

// 访问内部信息

/*!
 *  \brief wstr_size 获取wstr宽字符串内部字符的数目
 *  \param [in] wstr wstr_t宽字符
 *  \retval 返回wstr宽字符串内部字符的数目 
 *  \note wstr不能为NULL，否则会造成断言失败
 */
CSTL_LIB size_t wstr_size(const wstr_t *wstr);

/*!
 *  \brief wstr_capacity 获取wstr内部当前内存总共可以存放的宽字符的数目
 *  \param [in] wstr wstr_t宽字符串
 *  \retval 返回wstr内部当前内存总共可以存放的宽字符的数目, 包含L'\0'   
 *  \note wstr不能为NULL，否则会造成断言失败
 */
CSTL_LIB size_t wstr_capacity(const wstr_t *wstr);

/*!
 *  \brief wstr_append 添加新的子串到宽字符尾部
 *  \param [in,out] wstr wstr_t宽字符串
 *  \param [in] raw 原始的以L'\0'结尾的宽字符串 
 *  \retval 返回wstr
 *  \note wstr不能为NULL，否则会造成断言失败
 */
CSTL_LIB wstr_t* wstr_append(wstr_t *wstr, const wchar_t *raw);

/*!
 *  \brief wstr_insert 在宽字符串的指定位置插入一个子串
 *  
 *  如果pos < 0则会在头部进行插入，如果pos比宽字符串的长度还要大，则会在尾部进行插入。
 *  
 *  \param [in,out] wstr wstr_t款字符串
 *  \param [in] pos 要插入的位置
 *  \param [in] new_wstr 要插入的子串
 *  \retval 返回wstr
 *  \note wstr不能为NULL，否则会断言失败。  
 */
CSTL_LIB wstr_t* wstr_insert(wstr_t *wstr, int pos, const wchar_t *new_wstr);

/*!
 *  \brief wstr_at 获取宽字符串指定位置上的宽字符
 *  \param [in] wstr wstr_t宽字符
 *  \param [in] index 要获取字符的索引位置
 *  \retval 如果index是个有效的索引位置([0,len)), 则返回对应位置上的宽字符，否则返回(wchar_t)-1  
 *  \note wstr不能为NULL，否则会断言失败。  
 */
CSTL_LIB wchar_t wstr_at(const wstr_t *wstr, int index);

/*!
 *  \brief wstr_sub_wstr 获取宽字符串指定范围内的子串
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] offset 起始位置
 *  \param [in] length 要获取的子串的长度
 *  \retval 返回具体的子串。index如果是小于0，则会自动从头部获取子串，如果大于长度，则会从尾部来获取子串  
 *  \note wstr不能为NULL，否则会断言失败。  
 */
CSTL_LIB wstr_t* wstr_sub_wstr(const wstr_t *wstr, int offset, int length);

/*!
 *  \brief wstr_sub_wstr 获取宽字符串指定范围内的子串, 将获取到的子串保存到buf中
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] offset 起始位置
 *  \param [in] length 要获取的子串的长度
 *  \param [in,out] buf 用来接收最终的子串
 *  \param [in] buflen buf的最大长度
 *  \param [out] 如果buf长度不够，*err == ENOMEM，否则*err = 0 
 *  \retval 返回已经写出到buf中的长度。
 *  \note wstr, buf不能为NULL，否则会断言失败。  
 */
CSTL_LIB int wstr_sub_wstr_buf(const wstr_t *wstr, int offset, int length,
        wchar_t *buf, int buflen, int *err);

/*!
 *  \brief wstr_trim 去除宽字符串头部所有的空白字符
 *  \param [in,out] wstr wstr_t宽字符串
 *  \retval 返回wstr
 *  \note wstr不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t* wstr_trim(wstr_t *wstr);

/*!
 *  \brief wstr_trim_left 去除宽字符串头部所有的空白字符
 *  \param [in,out] wstr wstr_t宽字符串
 *  \retval 返回wstr
 *  \note wstr不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t* wstr_trim_left(wstr_t *wstr);

/*!
 *  \brief wstr_trim_left 去除宽字符串尾部所有的空白字符
 *  \param [in,out] wstr wstr_t宽字符串
 *  \retval 返回wstr
 *  \note wstr不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t* wstr_trim_right(wstr_t *wstr);

/*!
 *  \brief wstr_split 以指定正则表达式来切割宽字符串到VEC容器中
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] regex 正则表达式字符串
 *  \retval 返回切割后的多个子串
 *  \note wstr, regex不能为NULL，否则会断言失败。
 */
CSTL_LIB VEC*   wstr_split(const wstr_t *wstr, const wchar_t *regex);

/*!
 *  \brief wstr_split_lines 将一个宽字符串按照多行来进行分割到VEC容器中
 *  \param [in] wstr wstr_t宽字符串 
 *  \retval 返回切割后的多个子串
 *  \note wstr不能为NULL，否则会断言失败
 */
CSTL_LIB VEC*   wstr_split_lines(const wstr_t *wstr);

/*!
 *  \brief wstr_cmp 比较两个宽字符串的之间的大小关系
 *  \param [in] wstr wstr_t宽字符串 
 *  \param [in] rhv 另一个宽字符串
 *  \retval 如果wstr == rhv, 则返回0，如果wstr > rhv, 则返回>0, 否则返回<0
 *  \note wstr, rhv不能为NULL，否则会断言失败
 */
CSTL_LIB int    wstr_cmp(const wstr_t *wstr, const wchar_t *rhv);

////////////////////////////////////////测试函数//////////////////////////////

/*!
 *  \brief wstr_contains 检测宽字符串中是否含有指定的子串
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] sub 要检测的子串
 *  \retval 如果wstr中含有sub，则返回true，否则返回false。
 *  \note wstr, sub不能为NULL，否则会断言失败。
 */
CSTL_LIB bool   wstr_contains(const wstr_t *wstr, const wchar_t *sub);

/*!
 *  \brief wstr_equals 比较两个宽字符串内容是否相等
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] rhv 另一个宽字符串
 *  \retval 如果wstr == rhv，则返回true，否则返回false。 
 *  \note wstr, rhv不能为NULL, 否则会断言失败。
 */
CSTL_LIB bool   wstr_equals(const wstr_t *wstr, const wchar_t *rhv);

/*!
 *  \brief wstr_equals_ignore_case 忽略大小写比较两个宽字符串内容是否相等
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] rhv 另一个宽字符串
 *  \retval 如果wstr == rhv，则返回true，否则返回false。 
 *  \note wstr, rhv不能为NULL, 否则会断言失败。
 */
CSTL_LIB bool   wstr_equals_ignore_case(const wstr_t *wstr, const wchar_t *rhv);

/*!
 *  \brief wstr_empty 检测宽字符串内容是否为空
 *  \param [in] wstr  wstr_t宽字符串
 *  \retval 否则wstr不包含任何字符串，则返回true, 否则返回false
 *  \note wstr不能为NULL，否则会断言失败
 */
CSTL_LIB bool   wstr_empty(const wstr_t *wstr);

/*!
 *  \brief wstr_matches 使用一个正则表达式来检测宽字符串是否符合指定格式
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] regex 正则表达式字符串
 *  \retval 如果符合则返回true，否则返回false
 *  \note wstr, regex不能为NULL，否则会断言失败。 
 */
CSTL_LIB bool   wstr_matches(const wstr_t *wstr, const wchar_t *regex);

/*!
 *  \brief wstr_starts_with 检测一个宽字符串是否以指定子串开头
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] value 指定子串
 *  \retval 如果wstr是以value开头，则返回true，否则返回false
 *  \note wstr, value不能为NULL，否则会断言失败。 
 */
CSTL_LIB bool   wstr_starts_with(const wstr_t *wstr, const wchar_t *value);

/*!
 *  \brief wstr_c_wstr 获取wstr_t宽字符串内部的内容
 *  \param [in] wstr wstr_t宽字符串
 *  \retval返回wstr_t宽字符串内部的内容   
 *  \note wstr不能为NULL，否则会断言失败
 */
CSTL_LIB wchar_t*  wstr_c_wstr(wstr_t *wstr);

/*!
 *  \brief wstr_hash_code  计算wstr_t的hash值 
 *  \param [in] wstr wstr_t字符串
 *  \retval 返回计算好的hash值
 */
CSTL_LIB unsigned int    wstr_hash_code(const wstr_t *wstr);

/*!
 *  \brief wstr_ptr_hash_code 计算**wstr的hash值 
 *  \param [in] wstr wstr_t字符串
 *  \retval 返回计算好的hash值
 */
CSTL_LIB unsigned int    wstr_ptr_hash_code(const wstr_t **wstr);

/*!
 *  \brief wstr_assign 将一个原始的宽字符串赋值给一个wstr_t宽字符串类型
 *  \param [in,out] wstr wstr_t宽字符串
 *  \param [in] rhv 一个以L'\0'结尾的宽字符串
 *  \retval 返回wstr
 *  \note wstr, rhv不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t* wstr_assign(wstr_t *wstr, const wchar_t *rhv);

// 查找

/*!
 *  \brief wstr_assign 查找一个子宽字符串在一个宽字符串中的索引位置
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] from_index 开始查找的起始位置
 *  \param [in] val 要查找的子串
 *  \retval 如果找到则返回指定的索引位置，如果没有找到，则返回-1.
 *  \note wstr, val不能为NULL，否则会断言失败。如果from_index小于0，则会在头部进行查找
 */
CSTL_LIB int    wstr_index_of(const wstr_t *wstr, int from_index, const wchar_t *val);

/*!
 *  \brief wstr_index_of_wchar 查找一个宽字符在指定宽字符串中的索引位置
 *  \param [in] wstr wstr_t宽字符串 
 *  \param [in] from_index 开始查找的起始位置
 *  \param [in] val 要查找的字符串值
 *  \retval 如果找到则返回指定的索引位置，如果没有找到，则返回-1
 *  \note wstr不能为NULL，否则断言会失败。 如果from_index小于0，则会在头部进行查找。
 */
CSTL_LIB int    wstr_index_of_wchar(const wstr_t *wstr, int from_index, wchar_t val);

/*!
 *  \brief wstr_rindex_of 在一个宽字符串中反向查找一个子宽字符串的位置
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] from_index 开始查找的起始位置
 *  \param [in] val 要查找的子串
 *  \retval 如果找到则返回指定的索引位置，如果没有找到，返回-1
 *  \note wstr, val不能为NULL，否则会断言失败。如果from_index小于0，则会从末尾进行查找。
 */
CSTL_LIB int    wstr_rindex_of(const wstr_t *wstr, int from_index, const wchar_t *val);

/*!
 *  \brief wstr_rindex_of_wchar 在一个宽字符串中反向查找一个字符的位置
 *  \param [in] wstr wstr_t宽字符串
 *  \param [in] from_index 开始查找的起始位置
 *  \param [in] val 要查找的字符
 *  \retval 如果找到则返回指定的索引位置，如果没有找到，返回-1
 *  \note wstr不能为NULL，否则会断言失败。如果from_index < 0, 则会在尾部来进行查找
 */
CSTL_LIB int    wstr_rindex_of_wchar(const wstr_t *wstr, int from_index, wchar_t val);

// 替换wstr中的内容
 
/*!
 *  \brief wstr_replace 替换一个宽字符串中指定的子串为新的子串
 *  \param [in,out] wstr wstr_t宽字符串
 *  \param [in] old_wstr 老的子串
 *  \param [in] new_wstr 新的子串 
 *  \retval wstr
 *  \note wstr, old_wstr, new_wstr不能为NULL，否则会断言失败
 */ 
CSTL_LIB wstr_t*  wstr_replace(wstr_t *wstr, const wchar_t *old_wstr, const wchar_t *new_wstr);

/*!
 *  \brief wstr_replace_wchar 替换一个宽字符串中指定的宽字符为新的宽字符
 *  \param [in,out] wstr wstr_t宽字符串
 *  \param [in] old_ch 老的宽字符
 *  \param [in] new_ch 新的宽字符
 *  \retval wstr
 *  \note wstr不能为NULL，否则会断言失败。 
 */
CSTL_LIB wstr_t*  wstr_replace_wchar(wstr_t *wstr, const wchar_t old_ch, wchar_t new_ch);

/*!
 *  \brief wstr_replace_first 替换第一个宽字符串中第一个出现的指定子串为新的子串
 *  \param [in,out] wstr wstr_t宽字符串
 *  \param [in] offset 开始查找的起始索引位置
 *  \param [in] old_wstr 老的子串
 *  \param [in] new_wstr 新的子串
 *  \retval wstr
 *  \note wstr, offset, old_wstr, new_wstr不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t*  wstr_replace_first(wstr_t *wstr, int offset,  const wchar_t *old_wstr, const wchar_t *new_wstr);

/*!
 *  \brief wstr_to_lowercase 将一个宽字符串中的所有大写字母替换为小写字母
 *  \param [in,out] wstr wstr_t宽字符串
 *  \retval wstr 
 *  \note wstr不能为NULL，否则会断言失败。
 */
CSTL_LIB wstr_t* wstr_to_lowercase(wstr_t *wstr);

/*!
 *  \brief wstr_to_uppercase 将一个宽字符串中的所有小写字母替换为大写字母
 *  \param [in,out] wstr wstr_t字符串
 *  \retval wstr
 *  \note wstr不能为NULL，否则会断言失败。 
 */
CSTL_LIB wstr_t* wstr_to_uppercase(wstr_t *wstr);

/*!
 *  \brief wstr_ptr_destroy 辅助函数，用来销毁*wstr内部占用的资源 
 *  \param [in,out] wstr 要进行销毁的宽字符串
 *  \retval none.
 *  \note  此函数执行后，*wstr就不在有用了
 */
CSTL_LIB void wstr_ptr_destroy(wstr_t **wstr);

/*!
 *  \brief wstr_resize 改变宽字符串的尺寸
 *  
 *  如果new_size大于原来宽字符串的长度，则会将多出来的元素赋值value,如果new_size小于
 *  原来的宽字符串的长度，则会将多出来的字符解掉。
 *  
 *  \param [in,out] str wstr_t宽字符串
 *  \param [in] new_size 新的尺寸
 *  \param [in] value 新的元素的默认值。
 *  \retval wstr
 *  \note str 不能为NULL，否则会断言失败。  
 */
CSTL_LIB wstr_t* wstr_resize(wstr_t *str, size_t new_size, wchar_t value);

#define wstr_vec_get(vec, index) (*(wstr_t**)vec_get(vec, index)) //!< 辅助宏，用来获取wstr_vec指定位置上的宽字符指针

#endif //INCLUDE/WSTR_H_H
