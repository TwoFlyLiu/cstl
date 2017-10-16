#ifndef INCLUDE_STR_H_H
#define INCLUDE_STR_H_H

#include "cstl_stddef.h"
#include "vec.h"

// str_t 表示编译系统默认支持的字符串类型，对于windows, 一般是GBK类型，对于linux或者msys一般是utf8类型，
// 所以他的类型是不固定的，原来的版本因为是在msys平台上，以前假设他是utf8字符串，但是这个版本这个假设
// 不存在了，真正的utf8字符集在u8_str_t中，所以里面关于求字符长度的相关api，也会进行相应调整。
// 
// 关于字符长度的api，为了和上一个版本兼容，仍然保留char_size, 移除了sub, sub_buf函数,推荐使用wstr_t来统一字符集处理，
// 避免乱码问题。
struct CSTLString;
typedef struct CSTLString str_t;

// 工厂方法(内部支持utf-8字符集)
CSTL_LIB str_t * str_new();
CSTL_LIB str_t * str_new_from(const char *str);
CSTL_LIB str_t * str_new_from_limit(const char *str, int len);
CSTL_LIB str_t * str_format(const char *fmt, ...);

// 释放内部所有的资源
void str_free(str_t *str);

// 访问内部信息

// 返回字节数目(byte)
CSTL_LIB size_t str_byte_size(const str_t *str);

// 返回字符数目（一个字符可能有多个字节）
CSTL_LIB size_t str_char_size(const str_t *str);

// 内部还可以使用空闲缓存的字节数目
CSTL_LIB size_t str_capacity(const str_t *str);

// 增加 添加字符串
CSTL_LIB str_t* str_append(str_t *str, const char *raw);

// 在某个位置上进行插入
CSTL_LIB str_t* str_insert(str_t *str, int pos, const char *new_str);

// 获取单个字节（有可能数据不完整）
CSTL_LIB char str_at_byte(str_t *str, int index);

// 返回子字节符, offset单位是字节， length单位也是字节
CSTL_LIB str_t* str_sub_str(str_t *str, int offset, int length);
CSTL_LIB int str_sub_str_buf(str_t *str, int offset, int length,
        char *buf, int buflen, int *err);

// 去除收尾空白字符
CSTL_LIB str_t* str_trim(str_t *str);

// 只去除头部空白字符
CSTL_LIB str_t* str_trim_left(str_t *str);

// 只去除尾部空白字符
CSTL_LIB str_t* str_trim_right(str_t *str);

// 分割字符集
CSTL_LIB VEC*   str_split(str_t *str, const char *regex);

// 切割行
CSTL_LIB VEC*   str_split_lines(str_t *str);

// 比较字符集
CSTL_LIB int    str_cmp(str_t *str, const char *rhv);

////////////////////////////////////////测试函数//////////////////////////////

// 用来测试
CSTL_LIB bool   str_contains(const str_t *str, const char *sub);

// 比较两者内容释放相等
CSTL_LIB bool   str_equals(const str_t *str, const char *rhv);

// 忽略大小写比较
CSTL_LIB bool   str_equals_ignore_case(const str_t *str, const char *rhv);

// 检测是否为空
CSTL_LIB bool   str_empty(const str_t *str);

// 支持正则表达式匹配
CSTL_LIB bool   str_matches(str_t *str, const char *regex);

// 测试是否以value开头
CSTL_LIB bool   str_starts_with(const str_t *str, const char *value);

// 返回内部的内部的内容
CSTL_LIB char*  str_c_str(str_t *str);

// 计算hash值
CSTL_LIB unsigned int    str_hash_code(str_t *str);
CSTL_LIB unsigned int    str_ptr_hash_code(str_t **str);

// 方将直接将原始字符串赋值给str_t*类型
CSTL_LIB str_t* str_assign(str_t *str, const char *rhv);

// 查找
CSTL_LIB int    str_index_of(const str_t *str, int from_index, const char *val);
CSTL_LIB int    str_index_of_char(const str_t *str, int from_index, char val);
CSTL_LIB int    str_rindex_of(const str_t *str, int from_index, const char *val);
CSTL_LIB int    str_rindex_of_char(const str_t *str, int from_index, char val);

// 替换str中的内容
CSTL_LIB str_t*  str_replace(str_t *str, const char *old_str, const char *new_str);
CSTL_LIB str_t*  str_replace_byte(str_t *str, const char old_ch, char new_ch);
CSTL_LIB str_t*  str_replace_first(str_t *str, int offset,  const char *old_str, const char *new_str);

CSTL_LIB str_t* str_to_lowercase(str_t *str);
CSTL_LIB str_t* str_to_uppercase(str_t *str);

// 减少或者扩大容量
CSTL_LIB str_t* str_resize(str_t *str, size_t new_size);

// 便利方法，方便销毁在容器中的str_t*
CSTL_LIB void str_ptr_destroy(str_t **str);

#define str_vec_get(vec, index) (*(str_t**)vec_get(vec, index))

#endif //INCLUDE/STR_H_H
