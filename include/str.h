#ifndef INCLUDE_STR_H_H
#define INCLUDE_STR_H_H

#include "cstl_stddef.h"
#include "vec.h"

///////////////////////////////////////////////////////////////////////////
// 本str api，除了str_sub, str_char_size， 使用的单位是utf8字符的单位
// 比如 '中', 会被当成一个字符，对于除了这两个方法以外的方法，使用的
// 单位都是字节单位，即使1, 和原始string.h返回的单位一直。
// 
// TODO: 可以设计一个utf8_str来全面支持utf8, 即将本api的单位全部
// 转换为utf8的单位
// 思路：可以利用str_t的api, 将utf8 api中的单位转化为byte的单位，然后调用对应的str_api，再将返回值转化为utf8单位
//       要么重新完全实现，或许更加简单
//       折中，具体api函数 哪种方便就使用哪种实现

// 只是声明(至少c99是支持的)
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
CSTL_LIB size_t str_byte_size(str_t *str);

// 返回字符数目（一个字符可能有多个字节）
CSTL_LIB size_t str_char_size(str_t *str);

// 内部还可以使用空闲缓存的字节数目
CSTL_LIB size_t str_capacity(str_t *str);

// 增加 添加字符串
CSTL_LIB str_t* str_append(str_t *str, const char *raw);

// 在某个位置上进行插入
CSTL_LIB str_t* str_insert(str_t *str, int pos, const char *new_str);

// 获取某个字符(因为一个字符可能有多个字节，所以返回str_t)
CSTL_LIB str_t* str_at(str_t *str, int index);

// 相对于str_at，使用栈来存储性能比较好
CSTL_LIB int str_at_buf(str_t *str, int index, char *buf, int buflen,
        int *err);

// 获取单个字节（有可能数据不完整）
CSTL_LIB char str_at_byte(str_t *str, int index);

// 返回子字符集
CSTL_LIB str_t* str_sub(str_t *str, int offset, int length);

CSTL_LIB int str_sub_buf(str_t *str, int offset, int length,
        char *buf, int buflen, int *err);

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
CSTL_LIB unsigned int    hash_code(str_t *str);

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

// 便利方法，方便销毁在容器中的str_t*
CSTL_LIB void str_ptr_destroy(str_t **str);

#define str_vec_get(vec, index) (*(str_t**)vec_get(vec, index))

#endif //INCLUDE/STR_H_H
