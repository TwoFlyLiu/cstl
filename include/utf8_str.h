/*!
 * \file utf8_str.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于utf8字符串的常见操作
 * 
 * u8_str_t内部的字符串编码格式是utf8编码，并且它提供的函数必须要提供的字符串，必须要指定具体的编码类型，
 * 内部都会现将这个指定编码转换为utf8编码，然后然后才会执行真正的内部方法。
 */

#ifndef INCLUDE_UTF8_STR_H_H
#define INCLUDE_UTF8_STR_H_H

#include "cstl_stddef.h"
#include "vec.h"

struct CSTLUtf8String;                      //!< 表示cstl中的utf8字符串
typedef struct CSTLUtf8String u8_str_t;     //!< CSTLUtf8String的一个别名

typedef unsigned char byte_t;               //!< 标一个一个字节类型

#define CSTL_ASCII "ASCII"                  //!< ASCII字符集名称
#define CSTL_UTF8 "UTF-8"                   //!< UTF8字符集名称
#define CSTL_UTF16 "UTF-16LE"               //!< UTF16字符集名称
#define CSTL_UTF32 "UTF-32LE"               //!< UTF32字符集名称
#define CSTL_GBK "GB18030"                  //!< GBK字符集名称

// 在windows上，wchar_t表示的是utf16编码，在linux则是utf32编码
#if defined(LINUX)
#define CSTL_UNICODE CSTL_UTF32             //!< UNICODE字符集名称
#else
#define CSTL_UNICODE CSTL_UTF16             //!< UNICODE字符集名称
#endif

/*!
 *  \brief 表示一个utf8字符
 */
typedef struct {
    byte_t *beg;  // 一个utf8字符的起始地址   
    byte_t *end;  // 一个utf8字符的终止地址
} u8_char_t;

/*!
 *  \brief u8_char_byte_size 返回一个utf8字符串的字符数目
 *  \param [in] u8_char 一个utf8字符串
 *  \retval 返回一个utf8字符串字符的数目
 *  \note u8_char 不能为NULL，否则断言失败。
 */
CSTL_LIB size_t u8_char_byte_size(const u8_char_t *u8_char);

/*!
 *  \brief u8_char_cmp 比较两个utf8字符
 *  \param u8_char 一个utf8字符
 *  \param beg 另一个utf8字符的起始地址
 *  \param beg 另一个utf8字符的终止地址（不包含）
 *  \retval 小于，返回-1, 等于返回0,大于返回1
 *  \note u8_char, beg, end不能为NULL，否则会断言失败
 */
CSTL_LIB int u8_char_cmp(const u8_char_t *u8_char, byte_t *beg, byte_t *end);

/*!
 *  \brief u8_char_to_u8_str 将一个utf8字符返回成一个以'\0'结尾的字符串。
 *  \param [in] u8_char utf8字符
 *  \param [out] buf 用来接收保存的返回的内容
 *  \param [in] buflen buf的长度
 *  \retval 返回转换后的字符串，buf
 *  \note u8_char, buf必须不为NULL，否则断言会失败。如果buflen不够保存一个utf8字符，那么就会返回一个空字符串
 */
CSTL_LIB char* u8_char_to_u8_str(const u8_char_t *u8_char, char *buf, size_t buflen);

// 工厂方法(内部支持utf-8字符集)

/*!
 *  \brief u8_str_new 新建一个空的utf8字符串
 *  \retval 一个新的空的utf8字符串。
 *  \note 当返回的指针不在使用的使用，需要使用u8_str_free来进行销毁。
 */
CSTL_LIB u8_str_t * u8_str_new();

/*!
 *  \brief u8_str_new_from 使用一个已有的指定编码的字符串来新建一个utf8字符串
 *  \param [in] bytes 一个已有的指定编码的字符串
 *  \param [in] length 这个字符串在内存中占用的尺寸
 *  \param [in] charset 编码的名称, 可以是如下值：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 返回新的utf8字符串
 *  \note bytes, charset必须不能为NULL，否则会断言失败。对于常用的编码，推荐使用此函数对应的辅助函数：u8_str_new_from_ascii,
 *  u8_str_new_from_u8, u8_str_new_from_gbk, u8_str_new_from_uni
 */
CSTL_LIB u8_str_t * 
    u8_str_new_from(const byte_t *bytes, int length, const char *charset);

// 对于下面的几个函数，都会检测字符的有效性，并且主动舍弃掉最后不完整的字符

/*!
 *  \brief u8_str_new_from_ascii 使用一个ascii字符串来创建一个utf8字符串
 *  \param [in] bytes 一个ascii字符串
 *  \retval 返回一个utf8编码的字符串
 */
CSTL_LIB u8_str_t * u8_str_new_from_ascii(const char *bytes);

/*!
 *  \brief u8_str_new_from_gbk 使用一个gbk字符串来创建一个utf8字符串
 *  \param [in] bytes 一个gbk字符串
 *  \retval 返回一个utf8编码的字符串
 */
CSTL_LIB u8_str_t * u8_str_new_from_gbk(const char *bytes);

/*!
 *  \brief u8_str_new_from_u8 使用一个utf8字符串来创建一个utf8字符串
 *  \param [in] bytes 一个utf8字符串
 *  \retval 返回一个utf8编码的字符串
 */
CSTL_LIB u8_str_t * u8_str_new_from_u8(const char *bytes);

/*!
 *  \brief u8_str_new_from_uni 使用一个unicode字符串来创建一个utf8字符串
 *  \param [in] bytes 一个unicode字符串
 *  \retval 返回一个utf8编码的字符串
 */
CSTL_LIB u8_str_t * u8_str_new_from_uni(const wchar_t *bytes);

/*!
 *  \brief u8_str_free 释放u8字符串内部所占用的所有的资源
 *  \param [in,out] u8_str utf8字符串
 *  \retval none.
 *  \note u8_str不能为NULL，否则会断言失败。当此函数执行后，u8_str就不再有用了，不能再使用了
 */
CSTL_LIB void u8_str_free(u8_str_t *u8_str);

// 访问内部信息

/*!
 *  \brief u8_str_size  获取utf8字符串的字符数目
 *  \param [in] u8_str utf8字符串
 *  \retval 返回utf8字符串字符的数目
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_str_size(const u8_str_t *u8_str);

/*!
 *  \brief u8_str_byte_count 获取utf8字符串的字节数目
 *  \param [in] u8_str utf8字符串
 *  \retval 返回utf8字符串的字节数目 
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_str_byte_count(const u8_str_t *u8_str);

/*!
 *  \brief u8_str_capacity 获取utf8字符串内部当前已经分配好的内存的字节数目
 *  \param [in] u8_str utf8字符串
 *  \retval 返回utf8字符串当前内部已经分配好的字节数目 
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_str_capacity(const u8_str_t *u8_str);


/*!
 *  \brief u8_str_append 在一个utf8字符串尾部添加一个指定编码的字符串
 *  \param [in,out] u8_str 现有的一个utf8字符串
 *  \param [in] bytes 一个已有的指定编码的字符串
 *  \param [in] len 这个字符串在内存中占用的尺寸
 *  \param [in] charset 编码的名称, 可以是如下值：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 返回u8_str, 支持链式编程
 *  \note u8_str, bytes, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_append(u8_str_t *u8_str, byte_t *bytes,
        int len, const char *const charset);

/*!
 *  \brief u8_str_append_ascii 在一个utf8字符串添加一个ascii字符串, 他是u8_str_append的一个辅助函数
 *  \param [in, out] u8_str 现有的一个utf8字符串
 *  \param [in] ascii_str 一个ascii字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_append_ascii(u8_str_t *u8_str, const char *ascii_str);

/*!
 *  \brief u8_str_append_gbk 在一个utf8字符串添加一个gbk字符串, 他是u8_str_append的一个辅助函数
 *  \param [in, out] u8_str 现有的一个utf8字符串
 *  \param [in] ascii_str 一个gbk字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_append_gbk(u8_str_t *u8_str, const char *gbk_str);

/*!
 *  \brief u8_str_append_u8 在一个utf8字符串添加一个utf8字符串, 他是u8_str_append的一个辅助函数
 *  \param [in, out] u8_str 现有的一个utf8字符串
 *  \param [in] ascii_str 一个utf8字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_append_u8(u8_str_t *u8_str, const char *u8str);

/*!
 *  \brief u8_str_append_uni 在一个utf8字符串添加一个unicode字符串, 他是u8_str_append的一个辅助函数
 *  \param [in, out] u8_str 现有的一个utf8字符串
 *  \param [in] ascii_str 一个unicode字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_append_uni(u8_str_t *u8_str, const wchar_t *unistr);

/*!
 *  \brief u8_str_insert 在一个utf8字符串的指定位置插入一个指定编码的子串
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] pos 如果pos小于0，则会在utf8字符串头部进行插入，如果pos大于utf8字符数目，则会在尾部进行插入。
 *  \param [in] bytes 一个指定编码的字符串
 *  \param [in] len bytes的长度
 *  \param [in] charset 编码的名称, 可以是如下值：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 返回u8_str
 *  \note u8_str, bytes, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_insert(u8_str_t *u8_str, int pos, const byte_t *bytes,
        int len, const char *const charset);

/*!
 *  \brief u8_str_insert_ascii 在一个utf8字符串的指定位置插入一个ascii字符串, 本质上是一个u8_str_insert的一个辅助函数
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] pos 要插入的位置 
 *  \param [in] bytes 要插入的ascii字符集
 *  \retval u8_str
 */
CSTL_LIB u8_str_t* u8_str_insert_ascii(u8_str_t *u8_str, int pos, const char* bytes);

/*!
 *  \brief u8_str_insert_gbk 在一个utf8字符串的指定位置插入一个gbk字符串, 本质上是一个u8_str_insert的一个辅助函数
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] pos 要插入的位置 
 *  \param [in] bytes 要插入的gbk字符集
 *  \retval u8_str
 */
CSTL_LIB u8_str_t* u8_str_insert_gbk(u8_str_t *u8_str, int pos, const char* bytes);

/*!
 *  \brief u8_str_insert_u8 在一个utf8字符串的指定位置插入一个u8字符串, 本质上是一个u8_str_insert的一个辅助函数
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] pos 要插入的位置 
 *  \param [in] bytes 要插入的u8字符集
 *  \retval u8_str
 */
CSTL_LIB u8_str_t* u8_str_insert_u8(u8_str_t *u8_str, int pos, const char* bytes);

/*!
 *  \brief u8_str_insert_u8 在一个utf8字符串的指定位置插入一个unicode字符串, 本质上是一个u8_str_insert的一个辅助函数
 *  \param [in,out] u8_str 一个unicode字符串
 *  \param [in] pos 要插入的位置 
 *  \param [in] bytes 要插入的u8字符集
 *  \retval u8_str
 */
CSTL_LIB u8_str_t* u8_str_insert_uni(u8_str_t *u8_str, int pos, const wchar_t* bytes);


/*!
 *  \brief u8_str_at 返回一个utf8字符串指定位置上的字符
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] index 指定字符的索引位置
 *  \param [out] u8c 原来接收返回的utf8字节
 *  \retval none. 
 *  \note u8_str, u8c不能为NULL，否则会断言失败。
 */
CSTL_LIB void u8_str_at(u8_str_t *u8_str, size_t index, u8_char_t *u8c);

/*!
 *  \brief u8_str_sub 返回一个utf8字符中的指定子串
 *  \param [in] u8_str 一个utf8编码的字符串
 *  \param [in] offset 起始偏移的字符位置
 *  \param [in] length 指定字符的长度
 *  \retval  返回的一个子utf8字符串
 *  \note u8_str不能为NULL，否则会断言失败。   
 */
CSTL_LIB u8_str_t* u8_str_sub(u8_str_t *u8_str, size_t offset, size_t length);

/*!
 *  \brief u8_str_trim 移除掉一个utf8字符串收尾两侧的所有的空白字符
 *  \param [in,out] u8_str 一个utf8字符串
 *  \retval u8_str
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_trim(u8_str_t *u8_str);

/*!
 *  \brief u8_str_trim_left 移除掉一个utf8字符串头部的所有的空白字符
 *  \param [in,out] u8_str 一个utf8字符串
 *  \retval u8_str
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_trim_left(u8_str_t *u8_str);

/*!
 *  \brief u8_str_trim_left 移除掉一个utf8字符串尾部的所有的空白字符
 *  \param [in,out] u8_str 一个utf8字符串
 *  \retval u8_str
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_trim_right(u8_str_t *u8_str);

/*!
 *  \brief u8_str_split 一个指定编码的字符串来分割一个utf8字符串
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个指定编码的正则表达式字符串
 *  \param [in] len regex的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 返回所有分割后的utf8字符串列表
 *  \note u8_str, regex, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB VEC*   u8_str_split(u8_str_t *u8_str, const byte_t *regex, int len, const char *const charset);

/*!
 *  \brief u8_str_split_ascii 使用一个ascii字符串来分割一个utf8字符串, 本质上是u8_str_split的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个ascii编码的正则表达式
 *  \retval 返回所有分割后的utf8字符串列表
 */
CSTL_LIB VEC*   u8_str_split_ascii(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_split_gbk 使用一个gbk字符串来分割一个utf8字符串, 本质上是u8_str_split的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个gbk编码的正则表达式
 *  \retval 返回所有分割后的utf8字符串列表
 */
CSTL_LIB VEC*   u8_str_split_gbk(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_split_u8 使用一个utf8字符串来分割一个utf8字符串, 本质上是u8_str_split的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个utf8编码的正则表达式
 *  \retval 返回所有分割后的utf8字符串列表
 */
CSTL_LIB VEC*   u8_str_split_u8(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_split_u8 使用一个unicode字符串来分割一个utf8字符串, 本质上是u8_str_split的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个unicode编码的正则表达式
 *  \retval 返回所有分割后的utf8字符串列表
 */
CSTL_LIB VEC*   u8_str_split_uni(u8_str_t *u8_str, const wchar_t *regex);

/*!
 *  \brief u8_str_split_lines 以一个换行符来切割一个utf8字符串
 *  \param [in] u8_str 一个utf8字符串。 
 *  \retval 返回所有分割后的utf8字符串列表
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB VEC*   u8_str_split_lines(u8_str_t *u8_str);

/*!
 *  \brief u8_str_cmp 比较utf8字符串和一个指定编码的字符串之间的大小关系。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个指定编码的字符串
 *  \param [in] len rhv的长度
 *  \param [in] charset 一个字符集的名称。可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果小于, 返回值<0，如果等于，返回值等于0，如果大于，返回值大于0 
 *  \note u8_str, rhv, charset不能为NULL，否则断言会失败。
 */
CSTL_LIB int    u8_str_cmp(u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset);

/*!
 *  \brief u8_str_cmp_ascii 比较一个utf8字符串和另一个ascii字符串之间的大小关系, 本质上是u8_str_cmp的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个ascii字符串
 *  \retval 如果小于, 返回值<0，如果等于，返回值等于0，如果大于，返回值大于0 
 */
CSTL_LIB int    u8_str_cmp_ascii(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_cmp_gbk 比较一个utf8字符串和另一个gbk字符串之间的大小关系, 本质上是u8_str_cmp的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个gbk字符串
 *  \retval 如果小于, 返回值<0，如果等于，返回值等于0，如果大于，返回值大于0 
 */
CSTL_LIB int    u8_str_cmp_gbk(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_cmp_u8 比较一个utf8字符串和另一个utf8字符串之间的大小关系, 本质上是u8_str_cmp的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个utf8字符串
 *  \retval 如果小于, 返回值<0，如果等于，返回值等于0，如果大于，返回值大于0 
 */
CSTL_LIB int    u8_str_cmp_u8(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_cmp_u8 比较一个utf8字符串和另一个unicode字符串之间的大小关系, 本质上是u8_str_cmp的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个unicode字符串
 *  \retval 如果小于, 返回值<0，如果等于，返回值等于0，如果大于，返回值大于0 
 */
CSTL_LIB int    u8_str_cmp_uni(u8_str_t *u8_str, const wchar_t *rhv);

////////////////////////////////////////测试函数//////////////////////////////

/*!
 *  \brief u8_str_contains 检测一个utf8字符串是否含有一个指定编码的子字符串
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] sub 一个指定编码格式的字符串
 *  \param [in] len sub的长度
 *  \param [in] charset 编码的名称。可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果含有的返回true, 否则返回false。
 *  \note u8_str, sub, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB bool   u8_str_contains(const u8_str_t *u8_str, const byte_t *sub, size_t len,
        const char *const charset);

/*!
 *  \brief u8_str_contains_ascii 检测一个utf8字符串是否含有一个ascii编码的字符串, 本质上是u8_str_contains的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] sub 一个ascii字符串
 *  \retval 如果含有的话，则反掌true,否则返回false。
 */
CSTL_LIB bool   u8_str_contains_ascii(const u8_str_t *u8_str, const char *sub);

/*!
 *  \brief u8_str_contains_gbk 检测一个utf8字符串是否含有一个gbk编码的字符串, 本质上是u8_str_contains的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] sub 一个gbk字符串
 *  \retval 如果含有的话，则反掌true,否则返回false。
 */
CSTL_LIB bool   u8_str_contains_gbk(const u8_str_t *u8_str, const char *sub);

/*!
 *  \brief u8_str_contains_u8 检测一个utf8字符串是否含有一个utf8编码的字符串, 本质上是u8_str_contains的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] sub 一个utf8字符串
 *  \retval 如果含有的话，则反掌true,否则返回false。
 */
CSTL_LIB bool   u8_str_contains_u8(const u8_str_t *u8_str, const char *sub);

/*!
 *  \brief u8_str_contains_u8 检测一个utf8字符串是否含有一个unicode编码的字符串, 本质上是u8_str_contains的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] sub 一个unicode字符串
 *  \retval 如果含有的话，则反掌true,否则返回false。
 */
CSTL_LIB bool   u8_str_contains_uni(const u8_str_t *u8_str, const wchar_t *sub);

/*!
 *  \brief u8_str_equals 检测一个utf8字符串是否和另一个指定编码的字符串具有完全相同的内容
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 一个指定编码的字符串
 *  \param [in] len rhv的长度
 *  \param [in] charset 一个字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果完全相同的话，返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_equals(const u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset);

/*!
 *  \brief u8_str_equals_ascii 检测一个utf8字符串是否等于另一个ascii字符串。本质上是u8_str_equals的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个ascii字符串
 *  \retval 相等返回true, 否则返回false 
 */
CSTL_LIB bool   u8_str_equals_ascii(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_gbk 检测一个utf8字符串是否等于另一个gbk字符串。本质上是u8_str_equals的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个gbk字符串
 *  \retval 相等返回true, 否则返回false 
 */
CSTL_LIB bool   u8_str_equals_gbk(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_gbk 检测一个utf8字符串是否等于另一个utf8字符串。本质上是u8_str_equals的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个utf8字符串
 *  \retval 相等返回true, 否则返回false 
 */
CSTL_LIB bool   u8_str_equals_u8(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_gbk 检测一个utf8字符串是否等于另一个unicode字符串。本质上是u8_str_equals的一个辅助函数
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个unicode字符串
 *  \retval 相等返回true, 否则返回false 
 */
CSTL_LIB bool   u8_str_equals_uni(const u8_str_t *u8_str, const wchar_t *rhv);

/*!
 *  \brief u8_str_equals_ignore_case 忽略大小写来比较一个utf8字符串和另一个指定编码的字符串是否相等。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个指定编码的字符串
 *  \param [in] len rhv的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果相等返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_equals_ignore_case(const u8_str_t *u8_str, const byte_t *rhv,
        size_t len, const char *const charset);

/*!
 *  \brief u8_str_equals_ignore_case_ascii 忽略大小写来比较一个utf8字符串和另一个ascii字符串是否相等。本质上是
 *  u8_str_equals_ignore_case的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个ascii字符串
 *  \retval 相等返回true，不等返回false。
 */
CSTL_LIB bool   u8_str_equals_ignore_case_ascii(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_ignore_case_gbk 忽略大小写来比较一个utf8字符串和另一个gbk字符串是否相等。本质上是
 *  u8_str_equals_ignore_case的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个gbk字符串
 *  \retval 相等返回true，不等返回false。
 */
CSTL_LIB bool   u8_str_equals_ignore_case_gbk(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_ignore_case_u8 忽略大小写来比较一个utf8字符串和另一个utf8字符串是否相等。本质上是
 *  u8_str_equals_ignore_case的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个utf8字符串
 *  \retval 相等返回true，不等返回false。
 */
CSTL_LIB bool   u8_str_equals_ignore_case_u8(const u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_equals_ignore_case_u8 忽略大小写来比较一个utf8字符串和另一个unicode字符串是否相等。本质上是
 *  u8_str_equals_ignore_case的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个unicode字符串
 *  \retval 相等返回true，不等返回false。
 */
CSTL_LIB bool   u8_str_equals_ignore_case_uni(const u8_str_t *u8_str, const wchar_t *rhv);

/*!
 *  \brief u8_str_empty 检测一个utf8字符串是否不含有任何字符
 *  \param [in] u8_str 一个utf8字符串
 *  \retval 如果含有字符返回false，否则返回true。
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB bool   u8_str_empty(const u8_str_t *u8_str);

/*!
 *  \brief u8_str_matches 检测一个utf8字符串是否符合一个指定编码格式的正则表达式。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个指定编码的正则表达式字符串
 *  \param [in] len regex的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 符合返回true, 否则返回false。
 *  \note u8_str, regex, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB bool   u8_str_matches(u8_str_t *u8_str, const byte_t *regex, size_t len, 
        const char *const charset);

/*!
 *  \brief u8_str_matches_ascii 检测一个utf8字符串是否符合一个ascii编码的正则表达式字符串。本质上是：u8_str_matches的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个ascii编码的正则表达式
 *  \retval 符合返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_matches_ascii(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_matches_gbk 检测一个utf8字符串是否符合一个gbk编码的正则表达式字符串。本质上是：u8_str_matches的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个gbk编码的正则表达式
 *  \retval 符合返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_matches_gbk(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_matches_gbk 检测一个utf8字符串是否符合一个utf8编码的正则表达式字符串。本质上是：u8_str_matches的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个utf8编码的正则表达式
 *  \retval 符合返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_matches_u8(u8_str_t *u8_str, const char *regex);

/*!
 *  \brief u8_str_matches_gbk 检测一个utf8字符串是否符合一个unicode编码的正则表达式字符串。本质上是：u8_str_matches的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] regex 一个unicode编码的正则表达式
 *  \retval 符合返回true, 否则返回false。
 */
CSTL_LIB bool   u8_str_matches_uni(u8_str_t *u8_str, const wchar_t *regex);

/*!
 *  \brief u8_str_starts_with 检测一个utf8字符串是否以一个指定编码的子串开头
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] value 一个指定编码的字符串
 *  \param [in] len value的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果是的则返回true，否则返回false。
 *  \note u8_str, value, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB bool   u8_str_starts_with(const u8_str_t *u8_str, const byte_t *value, size_t len,
        const char *const charset);

/*!
 *  \brief u8_str_starts_with_ascii 检测一个utf8字符串是否以一个ascii字符串开头。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] value 一个ascii字符串
 *  \retval 如果是的则返回true，否则返回false。
 */
CSTL_LIB bool   u8_str_starts_with_ascii(const u8_str_t *u8_str, const char *value);

/*!
 *  \brief u8_str_starts_with_gbk 检测一个utf8字符串是否以一个gbk字符串开头。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] value 一个gbk字符串
 *  \retval 如果是的则返回true，否则返回false。
 */
CSTL_LIB bool   u8_str_starts_with_gbk(const u8_str_t *u8_str, const char *value);

/*!
 *  \brief u8_str_starts_with_u8 检测一个utf8字符串是否以一个utf8字符串开头。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] value 一个utf8字符串
 *  \retval 如果是的则返回true，否则返回false。
 */
CSTL_LIB bool   u8_str_starts_with_u8(const u8_str_t *u8_str, const char *value);

/*!
 *  \brief u8_str_starts_with_uni 检测一个utf8字符串是否以一个unicode字符串开头。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] value 一个unicode字符串
 *  \retval 如果是的则返回true，否则返回false。
 */
CSTL_LIB bool   u8_str_starts_with_uni(const u8_str_t *u8_str, const wchar_t *value);

/*!
 *  \brief u8_str_c_u8_str 返回utf8字符串内部所有保存的数据。
 *  \param [in] u8_str 一个utf8字符串
 *  \retval 返回utf8字符串内部所有保存的数据。 
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB char*  u8_str_c_u8_str(u8_str_t *u8_str);

/*!
 *  \brief u8_str_hash_code 计算一个utf8字符串的hash值
 *  \param [in] u8_str 一个utf8字符串
 *  \retval 反掌计算好的hash值 
 *  \note u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB unsigned int    u8_str_hash_code(u8_str_t *u8_str);

/*!
 *  \brief u8_str_ptr_hash_code 计算一个utf字符串的hash值 
 *  \param [in] u8_str 一个utf8字符串指针
 *  \retval 返回的是*u8_str的hash值
 *  \note u8_str, *u8_str不能为NULL，否则会断言失败。
 */
CSTL_LIB unsigned int    u8_str_ptr_hash_code(u8_str_t **u8_str);

/*!
 *  \brief u8_str_assign 将一个指定编码的字符串赋值给一个utf8字符串
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] rhv 一个指定编码的字符串
 *  \param [in] len 一个字符串的长度
 *  \param [in] charset 一个字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 返回u8_str
 *  \note u8_str, rhv, charset不能为 NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_assign(u8_str_t *u8_str, const byte_t *rhv, int len,
        const char *const charset);

/*!
 *  \brief u8_str_assign_ascii 将一个ascii字符串赋值给utf8字符串 
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个ascii字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_assign_ascii(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_assign_gbk 将一个gbk字符串赋值给utf8字符串 
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个gbk字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_assign_gbk(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_assign_u8 将一个utf8字符串赋值给utf8字符串 
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个utf8字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_assign_u8(u8_str_t *u8_str, const char *rhv);

/*!
 *  \brief u8_str_assign_uni 将一个unicode字符串赋值给utf8字符串 
 *  \param [in,out] u8_str 一个utf8字符串
 *  \param [in] rhv 另一个unicode字符串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t* u8_str_assign_uni(u8_str_t *u8_str, const wchar_t *rhv);

// 查找

/*!
 *  \brief u8_str_index_of 查找一个指定编码的字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找的起始字符位置
 *  \param [in] val 指定编码的字符串
 *  \param [in] len val的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果找到则返回找到的索引位置，否则返回-1 
 *  \note 如果u8_str, val, charset等于NULL，那么就会断言失败。如果from_index < 0，那么就会从头开始查找。
 */
CSTL_LIB int    u8_str_index_of(const u8_str_t *u8_str, int from_index, const byte_t *val, int len,
        const char *const charset);

/*!
 *  \brief u8_str_index_of_ascii 查找一个ascii字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找的起始字符串位置
 *  \param [in] val ascii字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1
 */
CSTL_LIB int    u8_str_index_of_ascii(u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_index_of_gbk 查找一个gbk字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找的起始字符串位置
 *  \param [in] val gbk字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1
 */
CSTL_LIB int    u8_str_index_of_gbk(u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_index_of_gbk 查找一个utf8字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找的起始字符串位置
 *  \param [in] val utf8字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1
 */
CSTL_LIB int    u8_str_index_of_u8(u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_index_of_uni 查找一个unicode字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找的起始字符串位置
 *  \param [in] val unicode字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1
 */
CSTL_LIB int    u8_str_index_of_uni(u8_str_t *u8_str, int from_index, const wchar_t *val);

/*!
 *  \brief u8_str_rindex_of 反向查找一个指定编码的字符串在utf8字符串中的索引位置
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找时候的起始字符索引位置，如果值小于0，则会在utf8字符串的最尾部来进行查找
 *  \param [in] val 指定编码的字符串
 *  \param [in] len val的长度
 *  \param [in] charset 字符集的名称，可以是：CSTL_ASCII, CSTL_UTF8, CSTL_UTF16, CSTL_UTF32, CSTL_UNICODE, 还可以是其他的
 *  libiconv库支持的编码名称。
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1。  
 *  \note u8_str, val, charset不能为NULL，否则会断言失败。
 */
CSTL_LIB int    u8_str_rindex_of(const u8_str_t *u8_str, int from_index, const byte_t *val,
        int len, const char *const charset);

/*!
 *  \brief u8_str_rindex_of_ascii 反向查找一个ascii字符串在utf8字符串中的索引位置, 本质上是u8_str_rindex_of的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找时候的起始字符索引位置，如果值小于0，则会在utf8字符串的最尾部来开始进行查找
 *  \param [in] val 一个ascii字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1。
 */
CSTL_LIB int    u8_str_rindex_of_ascii(const u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_rindex_of_gbk 反向查找一个gbk字符串在utf8字符串中的索引位置, 本质上是u8_str_rindex_of的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找时候的起始字符索引位置，如果值小于0，则会在utf8字符串的最尾部来开始进行查找
 *  \param [in] val 一个gbk字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1。
 */
CSTL_LIB int    u8_str_rindex_of_gbk(const u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_rindex_of_gbk 反向查找一个utf8字符串在utf8字符串中的索引位置, 本质上是u8_str_rindex_of的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找时候的起始字符索引位置，如果值小于0，则会在utf8字符串的最尾部来开始进行查找
 *  \param [in] val 一个utf8字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1。
 */
CSTL_LIB int    u8_str_rindex_of_u8(const u8_str_t *u8_str, int from_index, const char *val);

/*!
 *  \brief u8_str_rindex_of_uni 反向查找一个unicode字符串在utf8字符串中的索引位置, 本质上是u8_str_rindex_of的一个辅助函数。
 *  \param [in] u8_str 一个utf8字符串
 *  \param [in] from_index 开始查找时候的起始字符索引位置，如果值小于0，则会在utf8字符串的最尾部来开始进行查找
 *  \param [in] val 一个unicode字符串
 *  \retval 如果找到，则返回找到的索引位置，否则返回-1。
 */
CSTL_LIB int    u8_str_rindex_of_uni(const u8_str_t *u8_str, int from_index, const wchar_t *val);

// 替换u8_str中的内容

/*!
 *  \brief u8_str_replace 将一个utf8字符串中老的指定字符编码的子串替换为新的指定编码类型的子串
 *  \param [in,out] u8_str utf8字符串
 *  \param [in] old_bytes 老的指定编码的字符串
 *  \param [in] old_len old_byte的长度
 *  \param [in] old_charset 老的字符串的编码名称
 *  \param [in] new_bytes 新的指定编码的字符串
 *  \param [in] new_len new_bytes的长度
 *  \param [in] new_charset 新的字符串的编码名称
 *  \retval u8_str
 *  \note u8_str, old_bytes, old_charset, new_bytes, new_charset不能为NULL，否则断言就会失败。  
 */
CSTL_LIB u8_str_t*  u8_str_replace(u8_str_t *u8_str, const byte_t *old_bytes, int old_len, 
        const char *const old_charset, const byte_t *new_bytes, int new_len,
        const char *const new_charset);

/*!
 *  \brief u8_str_replace_ascii 将utf8字符串中老的ascii子串替换为新的ascii子串内容 , 本质上是u8_str_replace的辅助函数。
 *  \param [in,out] u8_str utf8字符串
 *  \param [in] old_u8_str 老的ascii字符子串
 *  \param [in] new_ascii_str 要替换成的新的ascii字符子串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_ascii(u8_str_t *u8_str, const char *old_ascii_str, const char *new_ascii_str);

/*!
 *  \brief u8_str_replace_gbk 将utf8字符串中老的gbk子串替换为新的gbk子串内容 , 本质上是u8_str_replace的辅助函数。
 *  \param [in,out] u8_str utf8字符串
 *  \param [in] old_gbk_str 老的gbk字符子串
 *  \param [in] new_gbk_str 要替换成的新的gbk字符子串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_gbk(u8_str_t *u8_str, const char *old_gbk_str, const char *new_gbk_str);

/*!
 *  \brief u8_str_replace_u8 将utf8字符串中老的utf8子串替换为新的utf8子串内容 , 本质上是u8_str_replace的辅助函数。
 *  \param [in,out] u8_str utf8字符串
 *  \param [in] old_u8_str 老的u8字符子串
 *  \param [in] new_u8_str 要替换成的新的u8字符子串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_u8(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str);

/*!
 *  \brief u8_str_replace_uni 将utf8字符串中老的unicode子串替换为新的unicode子串内容 , 本质上是u8_str_replace的辅助函数。
 *  \param [in,out] u8_str utf8字符串
 *  \param [in] old_uni_str 老的unicode字符子串
 *  \param [in] new_uni_str 要替换成的新的unicode字符子串
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_uni(u8_str_t *u8_str, const wchar_t *old_uni_str, const wchar_t *new_uni_str);


/*!
 *  \brief u8_str_replace_first 替换utf8字符串中第一次出现的指定字符集的字符串为新的指定字符串的字符串
 *  \param [in,out] u8_str utf8编码的字符串
 *  \param [in] offset 起始查找的位置, 如果offset小于0，那么起始查找位置就是0
 *  \param [in] old_str 老的指定编码的字符串
 *  \param [in] old_len old_str的长度(单位是字节)
 *  \param [in] old_charset 老的所使用字符集的名称
 *  \param [in] new_str old_str使用的字符编码的名称
 *  \param [in] nwe_len new_str的长度
 *  \param [in] new_charset new_str使用的字符编码的名称
 *  \retval u8_str
 *  \note u8_str, old_str, old_charset, new_str, new_charset不能为NULL，否则会断言失败。
 */
CSTL_LIB u8_str_t*  u8_str_replace_first(u8_str_t *u8_str, int offset,  const byte_t *old_str, int old_len,
       const char *const old_charset, const byte_t *new_str, int new_len, 
       const char *const new_charset);

/*!
 *  \brief u8_str_replace_first_ascii将utf8字符串中第一次出现的指定的ascii字符串替换为另一个ascii字符串, 本质上是u8_str_replace_first的一种便捷函数
 *  \param [in,out] u8_str utf8编码的字符串
 *  \param [in] offset 起始查找位置, 如果offset小于0，那么起始查找位置就是0
 *  \param [in] old_ascii_str 原来的ascii字符串内容
 *  \param [in] new_ascii_str 新的ascii字符串内容
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_first_ascii(u8_str_t *u8_str, int offset,  const char *old_ascii_str, const char *new_ascii_str);

/*!
 *  \brief u8_str_replace_first_gbk 将utf8字符串中第一次出现的指定的gbk字符串替换为另一个gbk字符串, 本质上是u8_str_replace_first的一种便捷函数
 *  \param [in,out] u8_str utf8编码的字符串
 *  \param [in] offset 起始查找位置, 如果offset小于0，那么起始查找位置就是0
 *  \param [in] old_gbk_str 原来的gbk字符串内容
 *  \param [in] new_gbk_str 新的gbk字符串内容
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_first_gbk(u8_str_t *u8_str, int offset,  const char *old_gbk_str, const char *new_gbk_str);

/*!
 *  \brief u8_str_replace_first_u8 将utf8字符串中第一次出现的指定的utf8字符串替换为另一个utf8字符串, 本质上是u8_str_replace_first的一种便捷函数
 *  \param [in,out] u8_str utf8编码的字符串
 *  \param [in] offset 起始查找位置, 如果offset小于0，那么起始查找位置就是0
 *  \param [in] old_utf8_str 原来的utf8字符串内容
 *  \param [in] new_utf8_str 新的utf8字符串内容
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_first_u8(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str);

/*!
 *  \brief u8_str_replace_first_uni 将utf8字符串中第一次出现的指定的uni字符串替换为另一个uni字符串, 本质上是u8_str_replace_first的一种便捷函数
 *  \param [in,out] u8_str utf8编码的字符串
 *  \param [in] offset 起始查找位置, 如果offset小于0，那么起始查找位置就是0
 *  \param [in] old_uni_str 原来的uni字符串内容
 *  \param [in] new_uni_str 新的uni字符串内容
 *  \retval 返回u8_str
 */
CSTL_LIB u8_str_t*  u8_str_replace_first_uni(u8_str_t *u8_str, int offset,  const wchar_t *old_uni_str, const wchar_t *new_uni_str);

/*!
 *  \brief u8_str_to_lowercase 将utf8字符串中所有大写字母替换为小写字母
 *  \param [in,out] u8_str utf8字符串
 *  \retval u8_str
 *  \note 如果u8_str为NULL，则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_to_lowercase(u8_str_t *u8_str);

/*!
 *  \brief u8_str_to_uppercase 将utf8字符串中所有小写字母替换为大写字母
 *  \param [in,out] u8_str utf8字符串
 *  \retval u8_str
 *  \note 如果u8_str为NULL，则会断言失败。
 */
CSTL_LIB u8_str_t* u8_str_to_uppercase(u8_str_t *u8_str);


// 一个直接访问原始u8数据，进行偏移的方法，对于普通的u8 char*也是有效的

/*!
 *  \brief  u8_raw_offset 对于原始的utf8字节流偏移指定的字符位置，得到偏移后的首字节地址
 *  \param [in] u8_raw 原始的utf8字节流
 *  \param [in] offset 偏移的位置
 *  \retval 返回偏移后的首地址 
 *  \note u8_raw 不能为NULL，否则会断言失败。
 */
CSTL_LIB const char* u8_raw_offset(const char* u8_raw, size_t offset);

// 返回u8_raw中字符的数目，相当于原来版本中的str_size

/*!
 *  \brief  u8_raw_size 获取原始utf8字节流的字符数目
 *  \param [in] u8_raw 原始的utf8字节流
 *  \retval 返回字符数目
 *  \note u8_raw不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_raw_size(const char *u8_raw);

/*!
 *  \brief raw_to_u8_raw 将指定编码的字节流转换为utf8编码的字节流
 *  \param [in] raw 指定编码的字节流
 *  \param [in] len raw的长度
 *  \param [in] charset 编码的名称
 *  \retval 返回utf8编码的字节流
 *  \note raw, charset不能为NULL，否则会断言失败。返回的内存是动态分配的，所以在不再使用它的时候，需要
 *  将这个指针用cstl_free释放掉
 */
CSTL_LIB char * raw_to_u8_raw(const byte_t *raw, size_t len, const char *charset);

// 处理u8字符串，字节位置和字符位置之间的转换

/*!
 *  \brief  u8_raw_raw_pos_to_u8_pos 将utf8字节流的字节位置转换为字符位置
 *  \param [in] u8_raw utf8字节流
 *  \param [in] raw_pos utf8字节位置
 *  \retval 返回utf8字符位置 
 *  \note u8_raw不能为NULL，否则会断言失败
 */
CSTL_LIB size_t u8_raw_raw_pos_to_u8_pos(const char *u8_raw, size_t raw_pos);

/*!
 *  \brief  u8_raw_u8_pos_to_raw_pos 将utf8字节流的字符位置转换为字节位置
 *  \param [in] u8_raw utf8字节流
 *  \param [in] u8_pos utf8字符位置
 *  \retval 返回utf8字符位置对应的字节位置
 *  \note u8_raw不能为NULL，否则断言会失败 
 */
CSTL_LIB size_t u8_raw_u8_pos_to_raw_pos(const char *u8_raw, size_t u8_pos);

/*!
 *  \brief  u8_str_raw_pos_to_u8_pos 将utf8字符串中的字节位置转换为字符位置
 *  \param [in] u8 utf8字符串
 *  \param [in] raw_pos 字节位置
 *  \retval 返回utf8字符串字节位置对应的字符位置
 *  \note u8不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_str_raw_pos_to_u8_pos(const u8_str_t *u8, size_t raw_pos);

/*!
 *  \brief  u8_str_u8_pos_to_raw_pos 将utf8字符串中的字符位置转化为字节位置
 *  \param [in] u8 utf8字符串
 *  \param [in] u8_pos 字符位置
 *  \retval 返回u8_pos在utf8字符串对应的字节位置
 *  \note u8不能为NULL，否则会断言失败。
 */
CSTL_LIB size_t u8_str_u8_pos_to_raw_pos(const u8_str_t *u8, size_t u8_pos);

/*!
 *  \brief u8_str_ptr_destroy 一个便利函数，用来方便设置将str_t*存储到某个容器中的销毁函数
 *  \param [out] u8 释放掉*u8内部占用的资源
 *  \retval none.
 *  \note 他是一个容器的回调函数，所以在以str_t*作为参数的时候，需要将这个函数设置容器中对应的销毁函数位置上
 */
CSTL_LIB void u8_str_ptr_destroy(u8_str_t **u8);

#define u8_str_vec_get(vec, index) (*(u8_str_t**)vec_get(vec, index)) //!< 便利宏，方便来获取以str_t*为元素类型的元素值

#endif //INCLUDE/STR_H_H
