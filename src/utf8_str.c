/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 14 19:39:02
*/
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <iconv.h>
#include <stdio.h>
#include <wchar.h>
#include <regex.h>

#include "cstl_stddef.h"
#include "leak.h"

#include "utf8_str.h"
#include "str_conv.h"
#include "str.h"

size_t wcslen(const wchar_t *s);
CSTL_EXPORT inline size_t u8_char_byte_size(const u8_char_t *u8_char)
{
    assert(u8_char);
    return u8_char->end - u8_char->beg;
}

CSTL_EXPORT int u8_char_cmp(const u8_char_t *u8_char, byte_t *beg, byte_t *end)
{
    int len = 0;

    assert(u8_char && beg && end);

    len = end - beg;
    if (len < 0 || u8_char_byte_size(u8_char) != (size_t)len) {
        return 0;
    }

    const byte_t *u8_beg = u8_char->beg;
    while (len > 0 && *u8_beg == *beg) {
        ++u8_beg;
        ++beg;
        --len;
    }

    return u8_beg == u8_char->end;
}

CSTL_EXPORT char* u8_char_to_u8_str(const u8_char_t *u8_char, char *buf, size_t buflen)
{
    assert(u8_char && buf);

    if (buflen > u8_char_byte_size(u8_char)) {
        strncpy(buf, (const char*)u8_char->beg, u8_char_byte_size(u8_char));
        *(u8_char->beg + u8_char_byte_size(u8_char)) = '\0';
    } else {
        *buf = '\0';
    }

    return buf;
}

// 内部数据就是用str_t来储存
struct CSTLUtf8String {
    str_t *data; 
};
typedef struct CSTLUtf8String u8_str_t;

#define STR_BUF_SIZE 1024

CSTL_EXPORT u8_str_t * u8_str_new();
CSTL_EXPORT u8_str_t * 
    u8_str_new_from(byte_t *bytes, int length, const char *charset)
{
    char *u8_raw = raw_to_u8_raw(bytes, length, charset);
    str_t *u8_data = str_new_from(u8_raw);
    cstl_free(u8_raw);
    
    u8_str_t *u8_str = (u8_str_t*)cstl_malloc(sizeof(u8_str_t));
    u8_str->data = u8_data;
    return u8_str;
}

CSTL_EXPORT u8_str_t * u8_str_new_from_ascii(const char *bytes)
{
    return u8_str_new_from((byte_t*)bytes, strlen(bytes), "ASCII");
}

CSTL_EXPORT u8_str_t * u8_str_new_from_gbk(const char *bytes)
{
    return u8_str_new_from((byte_t*)bytes, strlen(bytes), "GB18030");
}

CSTL_EXPORT u8_str_t * u8_str_new_from_u8(const char *bytes)
{
    return u8_str_new_from((byte_t*)bytes, strlen(bytes), "UTF-8"); //之所以要这样是为了利用iconv去除bytes的无效字符和不完整字符
}

CSTL_EXPORT u8_str_t * u8_str_new_from_u16(const wchar_t *bytes)
{
    return u8_str_new_from((byte_t*)bytes, wcslen(bytes) * sizeof(wchar_t), "UTF-16LE");
}

CSTL_EXPORT void u8_str_free(u8_str_t *u8_str)
{
    assert(u8_str);
    str_free(u8_str->data);
    cstl_free(u8_str);
}

CSTL_EXPORT size_t u8_str_size(const u8_str_t *u8_str)
{
    assert(u8_str);
    return u8_raw_size(u8_str_c_u8_str((u8_str_t*)u8_str));
}

CSTL_EXPORT size_t u8_raw_size(const char *u8_raw)
{
    assert(u8_raw);
    size_t len = 0;

    while (*u8_raw) {
        len++;
        u8_raw= u8_raw_offset(u8_raw, 1);
    }
    return len;
}

CSTL_EXPORT size_t u8_str_byte_count(const u8_str_t *u8_str)
{
    assert(u8_str);
    return str_byte_size(u8_str->data);
}

CSTL_EXPORT size_t u8_str_capacity(const u8_str_t *u8_str)
{
    assert(u8_str);
    return str_capacity(u8_str->data);
}

CSTL_EXPORT u8_str_t* u8_str_append(u8_str_t *u8_str, byte_t *bytes,
        int len, const char *const charset)
{
    assert(u8_str);

    char *u8_raw = raw_to_u8_raw(bytes, len, charset);
    str_append(u8_str->data, u8_raw);
    cstl_free(u8_raw);
    
    return u8_str;
}

CSTL_EXPORT u8_str_t* u8_str_append_ascii(u8_str_t *u8_str, const char *ascii_str)
{
    assert(u8_str);
    return u8_str_append(u8_str, (byte_t*)ascii_str, strlen(ascii_str), "ASCII");
}

CSTL_EXPORT u8_str_t* u8_str_append_gbk(u8_str_t *u8_str, const char *gbk_str)
{
    assert(u8_str);
    return u8_str_append(u8_str, (byte_t*)gbk_str, strlen(gbk_str), "GB18030");
}

CSTL_EXPORT u8_str_t* u8_str_append_u8(u8_str_t *u8_str, const char *u8str)
{
    assert(u8_str);
    return u8_str_append(u8_str, (byte_t*)u8str, strlen(u8str), "UTF-8");
}

CSTL_EXPORT u8_str_t* u8_str_append_u16(u8_str_t *u8_str, const wchar_t *u16str)
{
    assert(u8_str);
    return u8_str_append(u8_str, (byte_t*)u16str, wcslen(u16str) * sizeof(wchar_t), "UTF-16LE");
}

CSTL_EXPORT u8_str_t* u8_str_insert(u8_str_t *u8_str, int pos, const byte_t *bytes,
        int len, const char *const charset)
{
    assert(u8_str);

    pos = (pos < 0) ? 0 : pos;
    pos = ((size_t)pos >= u8_str_size(u8_str)) ? u8_str_size(u8_str) : pos;

    char *u8_raw = raw_to_u8_raw(bytes, len, charset);

    // 要将pos转换为raw的位置
    pos = (size_t)u8_str_u8_pos_to_raw_pos(u8_str, (size_t)pos);
    
    str_insert(u8_str->data, pos, u8_raw);
    cstl_free(u8_raw);

    return u8_str;
}

CSTL_EXPORT u8_str_t* u8_str_insert_ascii(u8_str_t *u8_str, int pos, const char* bytes)
{
    assert(u8_str);
    return u8_str_insert(u8_str, pos, (const byte_t*)bytes, strlen(bytes), "ASCII");
}

CSTL_EXPORT u8_str_t* u8_str_insert_gbk(u8_str_t *u8_str, int pos, const char* bytes)
{
    assert(u8_str);
    return u8_str_insert(u8_str, pos, (const byte_t*)bytes, strlen(bytes), "GB18030");
}

CSTL_EXPORT u8_str_t* u8_str_insert_u8(u8_str_t *u8_str, int pos, const char* bytes)
{
    assert(u8_str);
    return u8_str_insert(u8_str, pos, (const byte_t*)bytes, strlen(bytes), "UTF-8");
}

CSTL_EXPORT u8_str_t* u8_str_insert_u16(u8_str_t *u8_str, int pos, const wchar_t* bytes)
{
    assert(u8_str);
    return u8_str_insert(u8_str, pos, (const byte_t*)bytes, wcslen(bytes) * sizeof(wchar_t), "UTF-16LE");
}

// 获取某个字符(因为一个字符可能有多个字节，所以返回u8_str_t)
CSTL_EXPORT void u8_str_at(u8_str_t *u8_str, size_t index, u8_char_t *u8c)
{
    assert(u8_str && u8c);

    u8c->beg = (byte_t*)u8_raw_offset(u8_str_c_u8_str(u8_str), index);
    u8c->end  = (byte_t*)u8_raw_offset((const char*)u8c->beg, 1);
}

// 返回子字符集(不是线程安全的，u8_str的值，在某一时刻可能会发生波动变化)
CSTL_EXPORT u8_str_t* u8_str_sub(u8_str_t *u8_str, size_t offset, size_t length)
{
    assert(u8_str);

    char *data = u8_str_c_u8_str(u8_str);
    char *beg = (char*)u8_raw_offset(data, offset);
    char *end = (char*)u8_raw_offset(beg, length);

    char old_val = *end;

    *end = '\0';
    u8_str_t *sub_u8 = u8_str_new_from_u8(beg);
    *end = old_val;

    return sub_u8;
}

// 去除收尾空白字符
CSTL_EXPORT u8_str_t* u8_str_trim(u8_str_t *u8_str)
{
    assert(u8_str);
    str_trim(u8_str->data);
    return u8_str;
}

// 只去除头部空白字符
CSTL_EXPORT u8_str_t* u8_str_trim_left(u8_str_t *u8_str)
{
    assert(u8_str);
    str_trim_left(u8_str->data);
    return u8_str;
}

// 只去除尾部空白字符
CSTL_EXPORT u8_str_t* u8_str_trim_right(u8_str_t *u8_str)
{
    assert(u8_str);
    str_trim_right(u8_str->data);
    return u8_str;
}

// 分割字符集
CSTL_EXPORT VEC*   u8_str_split(u8_str_t *u8_str, const byte_t *regex, int len,
        const char *const charset)
{
    assert(u8_str && regex);

    VEC *vec = vec_new(sizeof(u8_str_t*), (destroy_func_t)u8_str_ptr_destroy);
    char *raw_regex = raw_to_u8_raw(regex, len, charset);

    int offset = 0;
    str_t *sub_str;

    regex_t trim_regex;
    regmatch_t trim_match;

    // 先编译
    if (0 != regcomp(&trim_regex, raw_regex, REG_EXTENDED)) {
        cstl_free(raw_regex);
        return NULL;
    }
    cstl_free(raw_regex);

    str_t *str = u8_str->data;
    u8_str_t *new_u8 = NULL;
    // 进行匹配
    while (0 == regexec(&trim_regex, str_c_str(str) + offset, 1, &trim_match, 0)) {
        sub_str = str_sub_str(str, offset, trim_match.rm_so);
        new_u8 = (u8_str_t*)cstl_malloc(sizeof(u8_str_t));
        new_u8->data = sub_str;
        vec_push_back(vec, &new_u8);
        offset += trim_match.rm_eo;
    }

    sub_str = str_sub_str(str, offset, str_byte_size(str));
    new_u8 = (u8_str_t*)cstl_malloc(sizeof(u8_str_t));
    new_u8->data = sub_str;
    vec_push_back(vec, &new_u8);
    return vec;
}

CSTL_EXPORT VEC*   u8_str_split_ascii(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_split(u8_str, (byte_t*)regex, strlen(regex), CSTL_ASCII);
}

CSTL_EXPORT VEC*   u8_str_split_gbk(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_split(u8_str, (byte_t*)regex, strlen(regex), CSTL_GBK);
}

CSTL_EXPORT VEC*   u8_str_split_u8(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_split(u8_str, (byte_t*)regex, strlen(regex), CSTL_UTF8);
}

CSTL_EXPORT VEC*   u8_str_split_u16(u8_str_t *u8_str, const wchar_t *regex)
{
    assert(u8_str && regex);
    return u8_str_split(u8_str, (byte_t*)regex, wcslen(regex) * sizeof(wchar_t), CSTL_UTF16);
}


// 切割行
CSTL_EXPORT VEC*   u8_str_split_lines(u8_str_t *u8_str)
{
    assert(u8_str);
    return u8_str_split_ascii(u8_str, "[\n]");
}

// 比较字符集
CSTL_EXPORT int    u8_str_cmp(u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset)
{
    assert(u8_str && rhv && charset);
    char *u8_raw = raw_to_u8_raw(rhv, len, charset);
    int res = strcmp(u8_str_c_u8_str(u8_str), u8_raw);
    cstl_free(u8_raw);
    return res;
}
CSTL_EXPORT int    u8_str_cmp_ascii(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_cmp(u8_str, (const byte_t*)rhv, strlen(rhv), CSTL_ASCII);
}

CSTL_EXPORT int    u8_str_cmp_gbk(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_cmp(u8_str, (const byte_t*)rhv, strlen(rhv), CSTL_GBK);
}

CSTL_EXPORT int    u8_str_cmp_u8(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_cmp(u8_str, (const byte_t*)rhv, strlen(rhv), CSTL_UTF8);
}

CSTL_EXPORT int    u8_str_cmp_u16(u8_str_t *u8_str, const wchar_t *rhv)
{
    assert(u8_str && rhv);
    return u8_str_cmp(u8_str, (const byte_t*)rhv, wcslen(rhv) * sizeof(wchar_t), CSTL_UTF16);
}

////////////////////////////////////////测试函数//////////////////////////////

// 用来测试
CSTL_EXPORT bool   u8_str_contains(const u8_str_t *u8_str, const byte_t *sub, size_t len,
        const char *const charset)
{
    assert(u8_str && sub && charset);
    char *u8_sub = (char*)raw_to_u8_raw(sub, len, charset);
    bool res = str_contains(u8_str->data, u8_sub);
    cstl_free(u8_sub);
    return res;
}

CSTL_EXPORT bool   u8_str_contains_ascii(const u8_str_t *u8_str, const char *sub)
{
    assert(u8_str && sub);
    return u8_str_contains(u8_str, (byte_t*)sub, strlen(sub), CSTL_ASCII);
}

CSTL_EXPORT bool   u8_str_contains_gbk(const u8_str_t *u8_str, const char *sub)
{
    assert(u8_str && sub);
    return u8_str_contains(u8_str, (byte_t*)sub, strlen(sub), CSTL_GBK);
}

CSTL_EXPORT bool   u8_str_contains_u8(const u8_str_t *u8_str, const char *sub)
{
    assert(u8_str && sub);
    return u8_str_contains(u8_str, (byte_t*)sub, strlen(sub), CSTL_UTF8);
}

CSTL_EXPORT bool   u8_str_contains_u16(const u8_str_t *u8_str, const wchar_t *sub)
{
    assert(u8_str && sub);
    return u8_str_contains(u8_str, (byte_t*)sub, wcslen(sub) * sizeof(wchar_t), CSTL_UTF16);
}

// 比较两者内容释放相等
CSTL_EXPORT bool   u8_str_equals(const u8_str_t *u8_str, const byte_t *rhv, size_t len,
        const char *const charset)
{
    assert(u8_str && rhv && charset);
    char *u8_rhv = raw_to_u8_raw(rhv, len, charset);    
    bool res = str_equals(u8_str->data, u8_rhv);
    cstl_free(u8_rhv);
    return res;
}

CSTL_EXPORT bool   u8_str_equals_ascii(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_ASCII);
}

CSTL_EXPORT bool   u8_str_equals_gbk(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_GBK);
}

CSTL_EXPORT bool   u8_str_equals_u8(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_UTF8);
}

CSTL_EXPORT bool   u8_str_equals_u16(const u8_str_t *u8_str, const wchar_t *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals(u8_str, (byte_t*)rhv, wcslen(rhv) * sizeof(wchar_t), CSTL_UTF16);
}

// 忽略大小写比较
CSTL_EXPORT bool   u8_str_equals_ignore_case(const u8_str_t *u8_str, const byte_t *rhv,
        size_t len, const char *const charset)
{
    assert(u8_str && rhv);
    char *u8_rhv = raw_to_u8_raw(rhv, len ,charset);
    bool res = str_equals_ignore_case(u8_str->data, u8_rhv);
    cstl_free(u8_rhv);
    return res;
}

CSTL_EXPORT bool   u8_str_equals_ignore_case_ascii(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals_ignore_case(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_ASCII);
}

CSTL_EXPORT bool   u8_str_equals_ignore_case_gbk(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals_ignore_case(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_GBK);
}

CSTL_EXPORT bool   u8_str_equals_ignore_case_u8(const u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals_ignore_case(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_UTF8);
}

CSTL_EXPORT bool   u8_str_equals_ignore_case_u16(const u8_str_t *u8_str, const wchar_t *rhv)
{
    assert(u8_str && rhv);
    return u8_str_equals_ignore_case(u8_str, (byte_t*)rhv, wcslen(rhv) * sizeof(wchar_t), CSTL_UTF16);
}

// 检测是否为空
CSTL_EXPORT inline bool   u8_str_empty(const u8_str_t *u8_str)
{
    assert(u8_str);
    return str_empty(u8_str->data);
}

// 支持正则表达式匹配
CSTL_EXPORT bool   u8_str_matches(u8_str_t *u8_str, const byte_t *regex, size_t len, 
        const char *const charset)
{
    assert(u8_str && regex);
    char *u8_regex = raw_to_u8_raw(regex, len, charset);
    bool res = str_matches(u8_str->data, u8_regex);
    cstl_free(u8_regex);
    return res;
}

CSTL_EXPORT bool   u8_str_matches_ascii(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_matches(u8_str, (byte_t*)regex, strlen(regex), CSTL_ASCII);
}

CSTL_EXPORT bool   u8_str_matches_gbk(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_matches(u8_str, (byte_t*)regex, strlen(regex), CSTL_GBK);
}

CSTL_EXPORT bool   u8_str_matches_u8(u8_str_t *u8_str, const char *regex)
{
    assert(u8_str && regex);
    return u8_str_matches(u8_str, (byte_t*)regex, strlen(regex), CSTL_UTF8);
}

CSTL_EXPORT bool   u8_str_matches_u16(u8_str_t *u8_str, const wchar_t *regex)
{
    assert(u8_str && regex);
    return u8_str_matches(u8_str, (byte_t*)regex, wcslen(regex) * sizeof(wchar_t), CSTL_UTF16);
}

// 测试是否以value开头
CSTL_EXPORT bool   u8_str_starts_with(const u8_str_t *u8_str, const byte_t *value, size_t len,
        const char *const charset)
{
    assert(u8_str && value && charset);
    char *u8_value = raw_to_u8_raw(value, len, charset);
    bool res = str_starts_with(u8_str->data, u8_value);
    cstl_free(u8_value);
    return res;
}

CSTL_EXPORT bool   u8_str_starts_with_ascii(const u8_str_t *u8_str, const char *value)
{
    assert(u8_str && value);
    return u8_str_starts_with(u8_str, (byte_t*)value, strlen(value), CSTL_ASCII);
}

CSTL_EXPORT bool   u8_str_starts_with_gbk(const u8_str_t *u8_str, const char *value)
{
    assert(u8_str && value);
    return u8_str_starts_with(u8_str, (byte_t*)value, strlen(value), CSTL_GBK);
}

CSTL_EXPORT bool   u8_str_starts_with_u8(const u8_str_t *u8_str, const char *value)
{
    assert(u8_str && value);
    return u8_str_starts_with(u8_str, (byte_t*)value, strlen(value), CSTL_UTF8);
}

CSTL_EXPORT bool   u8_str_starts_with_u16(const u8_str_t *u8_str, const wchar_t *value)
{
    assert(u8_str && value);
    return u8_str_starts_with(u8_str, (byte_t*)value, wcslen(value) * sizeof(wchar_t), CSTL_UTF16);
}

CSTL_EXPORT unsigned int    u8_str_hash_code(u8_str_t *u8_str)
{
    assert(u8_str);
    return str_hash_code(u8_str->data);
}

CSTL_EXPORT unsigned int    u8_str_ptr_hash_code(u8_str_t **u8_str)
{
    assert(u8_str);
    return u8_str_hash_code((*u8_str));
}

CSTL_EXPORT char*  u8_str_c_u8_str(u8_str_t *u8_str)
{
    assert(u8_str);
    return str_c_str(u8_str->data);
}


CSTL_EXPORT u8_str_t* u8_str_assign(u8_str_t *u8_str, const byte_t *rhv, int len,
        const char *const charset)
{
    char *u8_raw = raw_to_u8_raw(rhv, len, charset);
    str_assign(u8_str->data, u8_raw);
    cstl_free(u8_raw);
    return u8_str;
}

CSTL_EXPORT u8_str_t* u8_str_assign_ascii(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_assign(u8_str, (const byte_t*)rhv, strlen(rhv), CSTL_ASCII);
}

CSTL_EXPORT u8_str_t* u8_str_assign_gbk(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_assign(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_GBK);
}

CSTL_EXPORT u8_str_t* u8_str_assign_u8(u8_str_t *u8_str, const char *rhv)
{
    assert(u8_str && rhv);
    return u8_str_assign(u8_str, (byte_t*)rhv, strlen(rhv), CSTL_UTF8);
}

CSTL_EXPORT u8_str_t* u8_str_assign_u16(u8_str_t *u8_str, const wchar_t *rhv)
{
    assert(u8_str && rhv);
    return u8_str_assign(u8_str, (byte_t*)rhv, wcslen(rhv) * sizeof(wchar_t), CSTL_UTF16);
}


CSTL_EXPORT const char* u8_raw_offset(const char* u8_raw, size_t offset)
{
    unsigned char ch;
    int hbit;

    // 一次循环相当于偏移一次
    while (*u8_raw && offset > 0) {
        ch = (unsigned char)*u8_raw;
        hbit = 0;
        while (ch & 0x80) {
            hbit++;
            ch = (ch & 0x7f) << 1;
        }
        hbit = (hbit > 0 ? hbit : 1);
        u8_raw += hbit;
        -- offset;
    }

    return u8_raw;
}

CSTL_EXPORT char * raw_to_u8_raw(const byte_t *raw, size_t length, const char *charset)
{
    char *inbuf = (char*)raw;
    size_t inleft = length;

    const char *fromcode = charset;
    const char *tocode = "UTF-8//IGNORE";

    char *buf = (char*)cstl_malloc(length * 4 + 1);
    *buf = '\0';
    char *outbuf = buf;
    size_t outleft = length * 4;

    iconv_t cd = iconv_open(tocode, fromcode);
    assert((iconv_t)-1 != iconv);

    // 返回可能为-1， 但是因为在tocode末尾追加了//IGNORE,所以会主动忽略无效的或者不完整的字符
    iconv(cd, &inbuf, &inleft, &outbuf, &outleft);
    iconv_close(cd);
    *outbuf = '\0';

    return buf;
}

CSTL_EXPORT size_t u8_raw_raw_pos_to_u8_pos(const char *u8_raw, size_t raw_pos)
{
    size_t u8_pos = 0;

    assert(u8_raw);
    
    const char *end = u8_raw + raw_pos;
    size_t total_len = strlen(u8_raw);
    const char *total_end = u8_raw + total_len;

    // 对于错误边界的处理
    raw_pos = raw_pos >= total_len ? total_len : raw_pos; //允许定位到'\0'

    while ((u8_raw = u8_raw_offset(u8_raw, 1)) <= end) {
        u8_pos ++;

        // 因为当u8_raw为'\0',那么以后向后偏移总是返回'\0', 所以这儿遇到这种情况就表示扫描结束了
        if (u8_raw == total_end) {
            break;
        }
    }

    return u8_pos;
}

CSTL_EXPORT size_t u8_raw_u8_pos_to_raw_pos(const char *u8_raw, size_t u8_pos)
{
    assert(u8_raw);
    const char *end = u8_raw_offset(u8_raw, u8_pos);
    return end - u8_raw;
}

CSTL_EXPORT size_t u8_str_raw_pos_to_u8_pos(const u8_str_t *u8, size_t raw_pos)
{
    assert(u8);
    return u8_raw_raw_pos_to_u8_pos(str_c_str(u8->data), raw_pos);
}

CSTL_EXPORT size_t u8_str_u8_pos_to_raw_pos(const u8_str_t *u8, size_t u8_pos)
{
    assert(u8);
    return u8_raw_u8_pos_to_raw_pos(str_c_str(u8->data), u8_pos);
}

CSTL_EXPORT void u8_str_ptr_destroy(u8_str_t **u8)
{
    assert(u8 && *u8);
    u8_str_free(*u8);
}

// 查找
CSTL_EXPORT int    u8_str_index_of(const u8_str_t *u8_str, int from_index, const byte_t *val, int len,
        const char *const charset)
{
    assert(u8_str && val && charset);
    int raw_from_index = u8_str_u8_pos_to_raw_pos(u8_str, from_index);
    char *raw_val = raw_to_u8_raw(val, len, charset);
    int res = str_index_of(u8_str->data, raw_from_index, raw_val);
    cstl_free(raw_val);
    return u8_str_raw_pos_to_u8_pos(u8_str, res);
}

CSTL_EXPORT int    u8_str_index_of_ascii(u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_index_of(u8_str, from_index, (byte_t*)val, strlen(val),
            CSTL_ASCII);
}

CSTL_EXPORT int    u8_str_index_of_gbk(u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_index_of(u8_str, from_index, (byte_t*)val, strlen(val),
            CSTL_GBK);
}

CSTL_EXPORT int    u8_str_index_of_u8(u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_index_of(u8_str, from_index, (byte_t*)val, strlen(val),
            CSTL_UTF8);
}

CSTL_EXPORT int    u8_str_index_of_u16(u8_str_t *u8_str, int from_index, const wchar_t *val)
{
    assert(u8_str && val);
    return u8_str_index_of(u8_str, from_index, (byte_t*)val,
            wcslen(val) * sizeof(wchar_t), CSTL_UTF16);
}


CSTL_EXPORT int    u8_str_rindex_of(const u8_str_t *u8_str, int from_index, const byte_t *val,
        int len, const char *const charset)
{
    assert(u8_str && val && charset);
    int raw_from_index = u8_str_u8_pos_to_raw_pos(u8_str, from_index);
    char *u8_val = raw_to_u8_raw(val, len, charset);
    int res = str_rindex_of(u8_str->data, raw_from_index, u8_val);
    cstl_free(u8_val);
    return u8_str_raw_pos_to_u8_pos(u8_str, res);
}

CSTL_EXPORT int    u8_str_rindex_of_ascii(const u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_rindex_of(u8_str, from_index, (byte_t*)val, strlen(val), CSTL_ASCII);
}

CSTL_EXPORT int    u8_str_rindex_of_gbk(const u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_rindex_of(u8_str, from_index, (byte_t*)val, strlen(val), CSTL_GBK);
}

CSTL_EXPORT int    u8_str_rindex_of_u8(const u8_str_t *u8_str, int from_index, const char *val)
{
    assert(u8_str && val);
    return u8_str_rindex_of(u8_str, from_index, (byte_t*)val, strlen(val), CSTL_UTF8);
}

CSTL_EXPORT int    u8_str_rindex_of_u16(const u8_str_t *u8_str, int from_index, const wchar_t *val)
{
    assert(u8_str && val);
    return u8_str_rindex_of(u8_str, from_index, (byte_t*)val, wcslen(val) * sizeof(wchar_t), CSTL_UTF16);
}

// 替换u8_str中的内容
CSTL_EXPORT u8_str_t*  u8_str_replace(u8_str_t *u8_str, const byte_t *old_bytes, int old_len, 
        const char *const old_charset, const byte_t *new_bytes, int new_len,
        const char *const new_charset)
{
    assert(u8_str && old_bytes && old_charset && new_bytes && new_charset);
    char *u8_old = raw_to_u8_raw(old_bytes, old_len, old_charset);
    char *u8_new = raw_to_u8_raw(new_bytes, new_len, new_charset);
    str_replace(u8_str->data, u8_old, u8_new);
    cstl_free(u8_old);
    cstl_free(u8_new);

    return u8_str;
}

CSTL_EXPORT u8_str_t*  u8_str_replace_ascii(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace(u8_str, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_ASCII,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_ASCII);
}

CSTL_EXPORT u8_str_t*  u8_str_replace_gbk(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace(u8_str, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_GBK,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_GBK);
}

CSTL_EXPORT u8_str_t*  u8_str_replace_u8(u8_str_t *u8_str, const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace(u8_str, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_UTF8,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_UTF8);
}

CSTL_EXPORT u8_str_t*  u8_str_replace_u16(u8_str_t *u8_str, const wchar_t *old_u8_str, const wchar_t *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace(u8_str, (byte_t*)old_u8_str, wcslen(old_u8_str) * sizeof(wchar_t), CSTL_UTF16,
            (byte_t*)new_u8_str, wcslen(new_u8_str) * sizeof(wchar_t), CSTL_UTF16);
}


CSTL_EXPORT u8_str_t*  u8_str_replace_first(u8_str_t *u8_str, int offset,  const byte_t *old_raw, int old_len,
       const char *const old_charset, const byte_t *new_raw, int new_len, 
       const char *const new_charset)
{
    assert(u8_str && old_raw && new_raw && old_charset && new_charset);
    char *u8_old = raw_to_u8_raw(old_raw, old_len, old_charset);
    char *u8_new = raw_to_u8_raw(new_raw, new_len, new_charset);
    int raw_offset = u8_str_u8_pos_to_raw_pos(u8_str, offset); //要先将至转化为str的pos

    str_replace_first(u8_str->data, raw_offset, u8_old, u8_new);
    cstl_free(u8_old);
    cstl_free(u8_new);

    return u8_str;
}

CSTL_EXPORT u8_str_t*  u8_str_replace_first_ascii(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace_first(u8_str, offset, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_ASCII,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_ASCII);
}
CSTL_EXPORT u8_str_t*  u8_str_replace_first_gbk(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace_first(u8_str, offset, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_GBK,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_GBK);
}

CSTL_EXPORT u8_str_t*  u8_str_replace_first_u8(u8_str_t *u8_str, int offset,  const char *old_u8_str, const char *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace_first(u8_str, offset, (byte_t*)old_u8_str, strlen(old_u8_str), CSTL_UTF8,
            (byte_t*)new_u8_str, strlen(new_u8_str), CSTL_UTF8);
}
CSTL_EXPORT u8_str_t*  u8_str_replace_first_u16(u8_str_t *u8_str, int offset,  const wchar_t *old_u8_str, const wchar_t *new_u8_str)
{
    assert(u8_str && old_u8_str && new_u8_str);
    return u8_str_replace_first(u8_str, offset, (byte_t*)old_u8_str, wcslen(old_u8_str) * sizeof(wchar_t), CSTL_UTF16,
            (byte_t*)new_u8_str, wcslen(new_u8_str) * sizeof(wchar_t), CSTL_UTF16);
}

CSTL_EXPORT u8_str_t* u8_str_to_lowercase(u8_str_t *u8_str)
{
    assert(u8_str);
    str_to_lowercase(u8_str->data);
    return u8_str;
}

CSTL_EXPORT u8_str_t* u8_str_to_uppercase(u8_str_t *u8_str)
{
    assert(u8_str);
    str_to_uppercase(u8_str->data);
    return u8_str;
}

