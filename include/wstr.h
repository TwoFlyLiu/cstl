#ifndef INCLUDE_WSTR_H_H
#define INCLUDE_WSTR_H_H

#include "cstl_stddef.h"
#include "vec.h"

///////////////////////////////////////////////////////////////////////////
// 本wstr api，除了wstr_sub, wstr_wchar_t_size， 使用的单位是utf8字符的单位
// 比如 '中', 会被当成一个字符，对于除了这两个方法以外的方法，使用的
// 单位都是字节单位，即使1, 和原始wstring.h返回的单位一直。
// 
// TODO: 可以设计一个utf8_wstr来全面支持utf8, 即将本api的单位全部
// 转换为utf8的单位
// 思路：可以利用wstr_t的api, 将utf8 api中的单位转化为byte的单位，然后调用对应的wstr_api，再将返回值转化为utf8单位
//       要么重新完全实现，或许更加简单
//       折中，具体api函数 哪种方便就使用哪种实现

// 只是声明(至少c99是支持的)
struct CSTLWString;
typedef struct CSTLWString wstr_t;

// 工厂方法(内部支持utf-8字符集)
CSTL_LIB wstr_t * wstr_new();
CSTL_LIB wstr_t * wstr_new_from(const wchar_t *wstr);
CSTL_LIB wstr_t * wstr_new_from_limit(const wchar_t *wstr, int len);
CSTL_LIB wstr_t * wstr_format(const wchar_t *fmt, ...);

// 释放内部所有的资源
void wstr_free(wstr_t *wstr);

// 访问内部信息

CSTL_LIB size_t wstr_size(const wstr_t *wstr);

// 内部还可以使用空闲缓存的字节数目
CSTL_LIB size_t wstr_capacity(const wstr_t *wstr);

// 增加 添加字符串
CSTL_LIB wstr_t* wstr_append(wstr_t *wstr, const wchar_t *raw);

// 在某个位置上进行插入
CSTL_LIB wstr_t* wstr_insert(wstr_t *wstr, int pos, const wchar_t *new_wstr);

// 获取某个字符(因为一个字符可能有多个字节，所以返回wstr_t)
CSTL_LIB wchar_t wstr_at(wstr_t *wstr, int index);

// 返回子字节符, offset单位是字节， length单位也是字节
CSTL_LIB wstr_t* wstr_sub_wstr(wstr_t *wstr, int offset, int length);
CSTL_LIB int wstr_sub_wstr_buf(wstr_t *wstr, int offset, int length,
        wchar_t *buf, int buflen, int *err);

// 去除收尾空白字符
CSTL_LIB wstr_t* wstr_trim(wstr_t *wstr);

// 只去除头部空白字符
CSTL_LIB wstr_t* wstr_trim_left(wstr_t *wstr);

// 只去除尾部空白字符
CSTL_LIB wstr_t* wstr_trim_right(wstr_t *wstr);

// 分割字符集
CSTL_LIB VEC*   wstr_split(wstr_t *wstr, const wchar_t *regex);

// 切割行
CSTL_LIB VEC*   wstr_split_lines(wstr_t *wstr);

// 比较字符集
CSTL_LIB int    wstr_cmp(wstr_t *wstr, const wchar_t *rhv);

////////////////////////////////////////测试函数//////////////////////////////

// 用来测试
CSTL_LIB bool   wstr_contains(const wstr_t *wstr, const wchar_t *sub);

// 比较两者内容释放相等
CSTL_LIB bool   wstr_equals(const wstr_t *wstr, const wchar_t *rhv);

// 忽略大小写比较
CSTL_LIB bool   wstr_equals_ignore_case(const wstr_t *wstr, const wchar_t *rhv);

// 检测是否为空
CSTL_LIB bool   wstr_empty(const wstr_t *wstr);

// 支持正则表达式匹配
CSTL_LIB bool   wstr_matches(wstr_t *wstr, const wchar_t *regex);

// 测试是否以value开头
CSTL_LIB bool   wstr_starts_with(const wstr_t *wstr, const wchar_t *value);

// 返回内部的内部的内容
CSTL_LIB wchar_t*  wstr_c_wstr(wstr_t *wstr);

// 计算hash值
CSTL_LIB unsigned int    wstr_hash_code(wstr_t *wstr);
CSTL_LIB unsigned int    wstr_ptr_hash_code(wstr_t **wstr);

// 方将直接将原始字符串赋值给wstr_t*类型
CSTL_LIB wstr_t* wstr_assign(wstr_t *wstr, const wchar_t *rhv);

// 查找
CSTL_LIB int    wstr_index_of(const wstr_t *wstr, int from_index, const wchar_t *val);
CSTL_LIB int    wstr_index_of_wchar(const wstr_t *wstr, int from_index, wchar_t val);
CSTL_LIB int    wstr_rindex_of(const wstr_t *wstr, int from_index, const wchar_t *val);
CSTL_LIB int    wstr_rindex_of_wchar(const wstr_t *wstr, int from_index, wchar_t val);

// 替换wstr中的内容
CSTL_LIB wstr_t*  wstr_replace(wstr_t *wstr, const wchar_t *old_wstr, const wchar_t *new_wstr);
CSTL_LIB wstr_t*  wstr_replace_wchar(wstr_t *wstr, const wchar_t old_ch, wchar_t new_ch);
CSTL_LIB wstr_t*  wstr_replace_first(wstr_t *wstr, int offset,  const wchar_t *old_wstr, const wchar_t *new_wstr);

CSTL_LIB wstr_t* wstr_to_lowercase(wstr_t *wstr);
CSTL_LIB wstr_t* wstr_to_uppercase(wstr_t *wstr);

// 便利方法，方便销毁在容器中的wstr_t*
CSTL_LIB void wstr_ptr_dewstroy(wstr_t **wstr);

// 减少或者扩大容量
CSTL_LIB wstr_t* wstr_resize(wstr_t *str, size_t new_size);

#define wstr_vec_get(vec, index) (*(wstr_t**)vec_get(vec, index))

#endif //INCLUDE/WSTR_H_H
