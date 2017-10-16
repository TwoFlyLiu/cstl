#ifndef INCLUDE_UTF8_STR_H_H
#define INCLUDE_UTF8_STR_H_H

#include "cstl_stddef.h"
#include "vec.h"

// u8 字符串最后末尾本api也会自动追加'\0'
struct CSTLUtf8String;
typedef struct CSTLUtf8String u8_str_t;

typedef unsigned char byte_t;

#define CSTL_ASCII "ASCII"
#define CSTL_UTF8 "UTF-8"
#define CSTL_UTF16 "UTF-16LE"
#define CSTL_GBK "GB18030"

typedef struct {
    byte_t *beg;
    byte_t *end; //不包含
} u8_char_t;

CSTL_LIB size_t u8_char_byte_size(const u8_char_t *u8_char);
CSTL_LIB int u8_char_cmp(const u8_char_t *u8_char, byte_t *beg, byte_t *end);
CSTL_LIB char* u8_char_to_u8_str(const u8_char_t *u8_char, char *buf, size_t buflen);

// 工厂方法(内部支持utf-8字符集)
CSTL_LIB u8_str_t * u8_str_new();
CSTL_LIB u8_str_t * 
    u8_str_new_from(byte_t *bytes, int length, const char *charset);

// 对于下面的几个函数，都会检测字符的有效性，并且主动舍弃掉最后不完整的字符
CSTL_LIB u8_str_t * u8_str_new_from_ascii(const char *bytes);
CSTL_LIB u8_str_t * u8_str_new_from_gbk(const char *bytes);
CSTL_LIB u8_str_t * u8_str_new_from_u8(const char *bytes);
CSTL_LIB u8_str_t * u8_str_new_from_u16(const wchar_t *bytes);

// 释放内部所有的资源
CSTL_LIB void u8_str_free(u8_str_t *u8_str);

// 访问内部信息

// 获取u8字符数目
CSTL_LIB size_t u8_str_size(const u8_str_t *u8_str);

// 返回u8字节数目
CSTL_LIB size_t u8_str_byte_count(const u8_str_t *u8_str);

// 内部还可以使用空闲缓存的字节数目
CSTL_LIB size_t u8_str_capacity(const u8_str_t *u8_str);


// 增加 添加字符串
CSTL_LIB u8_str_t* u8_str_append(u8_str_t *u8_str, byte_t *bytes,
        int len, const char *const charset);

CSTL_LIB u8_str_t* u8_str_append_ascii(u8_str_t *u8_str, const char *ascii_str);
CSTL_LIB u8_str_t* u8_str_append_gbk(u8_str_t *u8_str, const char *gbk_str);
CSTL_LIB u8_str_t* u8_str_append_u8(u8_str_t *u8_str, const char *u8str);
CSTL_LIB u8_str_t* u8_str_append_u16(u8_str_t *u8_str, const wchar_t *u16str);

// 在某个位置上进行插入
CSTL_LIB u8_str_t* u8_str_insert(u8_str_t *u8_str, int pos, const byte_t *bytes,
        int len, const char *const charset);

CSTL_LIB u8_str_t* u8_str_insert_ascii(u8_str_t *u8_str, int pos, const char* bytes);
CSTL_LIB u8_str_t* u8_str_insert_gbk(u8_str_t *u8_str, int pos, const char* bytes);
CSTL_LIB u8_str_t* u8_str_insert_u8(u8_str_t *u8_str, int pos, const char* bytes);
CSTL_LIB u8_str_t* u8_str_insert_u16(u8_str_t *u8_str, int pos, const wchar_t* bytes);


// 获取某个字符(因为一个字符可能有多个字节，所以返回u8_str_t)
CSTL_LIB void u8_str_at(u8_str_t *u8_str, size_t index, u8_char_t *u8c);

// 返回子字符集
CSTL_LIB u8_str_t* u8_str_sub(u8_str_t *u8_str, size_t offset, size_t length);


// 去除收尾空白字符
CSTL_LIB u8_str_t* u8_str_trim(u8_str_t *u8_str);

// 只去除头部空白字符
CSTL_LIB u8_str_t* u8_str_trim_left(u8_str_t *u8_str);

// 只去除尾部空白字符
CSTL_LIB u8_str_t* u8_str_trim_right(u8_str_t *u8_str);

// 分割字符集
CSTL_LIB VEC*   u8_str_split(u8_str_t *u8_str, const byte_t *regex, int len, const char *const charset);
CSTL_LIB VEC*   u8_str_split_ascii(u8_str_t *u8_str, const char *regex);
CSTL_LIB VEC*   u8_str_split_gbk(u8_str_t *u8_str, const char *regex);
CSTL_LIB VEC*   u8_str_split_u8(u8_str_t *u8_str, const char *regex);
CSTL_LIB VEC*   u8_str_split_u16(u8_str_t *u8_str, const wchar_t *regex);

// 切割行
CSTL_LIB VEC*   u8_str_split_lines(u8_str_t *u8_str);

// 比较字符集
CSTL_LIB int    u8_str_cmp(u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset);
CSTL_LIB int    u8_str_cmp_ascii(u8_str_t *u8_str, const char *rhv);
CSTL_LIB int    u8_str_cmp_gbk(u8_str_t *u8_str, const char *rhv);
CSTL_LIB int    u8_str_cmp_u8(u8_str_t *u8_str, const char *rhv);
CSTL_LIB int    u8_str_cmp_u16(u8_str_t *u8_str, const wchar_t *rhv);

////////////////////////////////////////测试函数//////////////////////////////

// 用来测试
CSTL_LIB bool   u8_str_contains(const u8_str_t *u8_str, const byte_t *sub, size_t len,
        const char *const charset);
CSTL_LIB bool   u8_str_contains_ascii(const u8_str_t *u8_str, const char *sub);
CSTL_LIB bool   u8_str_contains_gbk(const u8_str_t *u8_str, const char *sub);
CSTL_LIB bool   u8_str_contains_u8(const u8_str_t *u8_str, const char *sub);
CSTL_LIB bool   u8_str_contains_u16(const u8_str_t *u8_str, const wchar_t *sub);

// 比较两者内容释放相等
CSTL_LIB bool   u8_str_equals(const u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset);
CSTL_LIB bool   u8_str_equals_ascii(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_gbk(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_u8(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_u16(const u8_str_t *u8_str, const wchar_t *rhv);

// 忽略大小写比较
CSTL_LIB bool   u8_str_equals_ignore_case(const u8_str_t *u8_str, const byte_t *rhv,
        size_t len, const char *const charset);
CSTL_LIB bool   u8_str_equals_ignore_case_ascii(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_ignore_case_gbk(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_ignore_case_u8(const u8_str_t *u8_str, const char *rhv);
CSTL_LIB bool   u8_str_equals_ignore_case_u16(const u8_str_t *u8_str, const wchar_t *rhv);

// 检测是否为空
CSTL_LIB bool   u8_str_empty(const u8_str_t *u8_str);

// 支持正则表达式匹配
CSTL_LIB bool   u8_str_matches(u8_str_t *u8_str, const byte_t *regex, size_t len, 
        const char *const charset);
CSTL_LIB bool   u8_str_matches_ascii(u8_str_t *u8_str, const char *regex);
CSTL_LIB bool   u8_str_matches_gbk(u8_str_t *u8_str, const char *regex);
CSTL_LIB bool   u8_str_matches_u8(u8_str_t *u8_str, const char *regex);
CSTL_LIB bool   u8_str_matches_u16(u8_str_t *u8_str, const wchar_t *regex);

// 测试是否以value开头
CSTL_LIB bool   u8_str_starts_with(const u8_str_t *u8_str, const byte_t *value, size_t len,
        const char *const charset);
CSTL_LIB bool   u8_str_starts_with_ascii(const u8_str_t *u8_str, const char *value);
CSTL_LIB bool   u8_str_starts_with_gbk(const u8_str_t *u8_str, const char *value);
CSTL_LIB bool   u8_str_starts_with_u8(const u8_str_t *u8_str, const char *value);
CSTL_LIB bool   u8_str_starts_with_u16(const u8_str_t *u8_str, const wchar_t *value);

// 返回内部的内部的内容
CSTL_LIB char*  u8_str_c_u8_str(u8_str_t *u8_str);

// 计算hash值
CSTL_LIB unsigned int    u8_str_hash_code(u8_str_t *u8_str);
CSTL_LIB unsigned int    u8_str_ptr_hash_code(u8_str_t **u8_str);

// 方将直接将原始字符串赋值给u8_str_t*类型
CSTL_LIB u8_str_t* u8_str_assign(u8_str_t *u8_str, const byte_t *rhv, int len,
        const char *const charset);
CSTL_LIB u8_str_t* u8_str_assign_ascii(u8_str_t *u8_str, const char *rhv);
CSTL_LIB u8_str_t* u8_str_assign_gbk(u8_str_t *u8_str, const char *rhv);
CSTL_LIB u8_str_t* u8_str_assign_u8(u8_str_t *u8_str, const char *rhv);
CSTL_LIB u8_str_t* u8_str_assign_u16(u8_str_t *u8_str, const wchar_t *rhv);

// 查找
CSTL_LIB int    u8_str_index_of(const u8_str_t *u8_str, int from_index, const byte_t *val, int len,
        const char *const charset);
CSTL_LIB int    u8_str_index_of_ascii(u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_index_of_gbk(u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_index_of_u8(u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_index_of_u16(u8_str_t *u8_str, int from_index, const wchar_t *val);


CSTL_LIB int    u8_str_rindex_of(const u8_str_t *u8_str, int from_index, const byte_t *val,
        int len, const char *const charset);
CSTL_LIB int    u8_str_rindex_of_ascii(const u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_rindex_of_gbk(const u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_rindex_of_u8(const u8_str_t *u8_str, int from_index, const char *val);
CSTL_LIB int    u8_str_rindex_of_u16(const u8_str_t *u8_str, int from_index, const wchar_t *val);

// 替换u8_str中的内容
CSTL_LIB u8_str_t*  u8_str_replace(u8_str_t *u8_str, const byte_t *old_bytes, int old_len, 
        const char *const old_charset, const byte_t *new_bytes, int new_len,
        const char *const new_charset);
CSTL_LIB u8_str_t*  u8_str_replace_ascii(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_gbk(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_u8(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_u16(u8_str_t *u8_str, const wchar_t *old_u8_str, const wchar_t *new_u8_str);


CSTL_LIB u8_str_t*  u8_str_replace_first(u8_str_t *u8_str, int offset,  const byte_t *old_u8_str, int old_len,
       const char *const old_charset, const byte_t *new_u8_str, int new_len, 
       const char *const new_charset);
CSTL_LIB u8_str_t*  u8_str_replace_first_ascii(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_first_gbk(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_first_u8(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str);
CSTL_LIB u8_str_t*  u8_str_replace_first_u16(u8_str_t *u8_str, int offset,  const wchar_t *old_u8_str, const wchar_t *new_u8_str);

CSTL_LIB u8_str_t* u8_str_to_lowercase(u8_str_t *u8_str);
CSTL_LIB u8_str_t* u8_str_to_uppercase(u8_str_t *u8_str);


// 一个直接访问原始u8数据，进行偏移的方法，对于普通的u8 char*也是有效的
CSTL_LIB const char* u8_raw_offset(const char* u8_raw, size_t offset);

// 返回u8_raw中字符的数目，相当于原来版本中的str_size
CSTL_LIB size_t u8_raw_size(const char *u8_raw);
CSTL_LIB char * raw_to_u8_raw(const byte_t *raw, size_t len, const char *charset);

// 处理u8字符串，字节位置和字符位置之间的转换
CSTL_LIB size_t u8_raw_raw_pos_to_u8_pos(const char *u8_raw, size_t raw_pos);
CSTL_LIB size_t u8_raw_u8_pos_to_raw_pos(const char *u8_raw, size_t u8_pos);
CSTL_LIB size_t u8_str_raw_pos_to_u8_pos(const u8_str_t *u8, size_t raw_pos);
CSTL_LIB size_t u8_str_u8_pos_to_raw_pos(const u8_str_t *u8, size_t u8_pos);

// 便利方法，方便销毁在容器中的u8_str_t*
CSTL_LIB void u8_str_ptr_destroy(u8_str_t **u8);
#define u8_str_vec_get(vec, index) (*(u8_str_t**)vec_get(vec, index))

#endif //INCLUDE/STR_H_H
