/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 11 15:37:39
*/
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <strings.h>
#include <regex.h>
#include <errno.h>

#include <stdio.h>

#include "cstl_stddef.h"
#include "leak.h"
#include "str.h"
#include "wstr.h"
#include "str_conv.h"

struct CSTLString {
    char *beg;
    int len; // 当前元素的长度
    int capacity; //最多存放元素数目（包含'\0')
};

typedef struct CSTLString str_t;

#define STR_BUF_SIZE 1024

// 居然有不清晰声明（在msys2中）
int vsnprintf (char * s, size_t n, const char * format, va_list arg );
static str_t *_str_realloc(str_t *str, int new_capacity);

static size_t __calc_new_capacity(const str_t *str, size_t char_count)
{
    return str_byte_size(str) + CSTL_MAX(str_byte_size(str), char_count) + 1;
}

CSTL_EXPORT str_t * str_new()
{
    str_t *new_str = (str_t*)cstl_malloc(sizeof(str_t));
    new_str->beg = (char*)cstl_malloc(sizeof(char));
    *(new_str->beg) = '\0'; //即使为空，也有一个'\0'
    new_str->capacity = 1;
    new_str->len = 0;
    return new_str;
}


CSTL_EXPORT str_t * str_new_from_limit(const char *str, int len)
{
    if (str == NULL || '\0' == str || len <= 0) {
        return str_new();
    }

    str_t *new_str = (str_t*)cstl_malloc(sizeof(str_t));

    new_str->capacity = sizeof(char) * (len * 2 + 1);
    new_str->beg = (char*)cstl_malloc(new_str->capacity);
    strncpy(new_str->beg, str, len); //如果str长度大于等于len, 那么末尾就不会有'\0'
    *(new_str->beg + len) = '\0';
    new_str->len = len;

    return new_str;
}

CSTL_EXPORT str_t * str_new_from(const char *str)
{
    return str_new_from_limit(str, strlen(str));
}


CSTL_EXPORT str_t * str_format(const char *fmt, ...)
{
    va_list args;
    char buf[STR_BUF_SIZE];
    int len;

    va_start (args, fmt);
    len = vsnprintf(buf, (size_t)STR_BUF_SIZE, fmt, args);
    va_end (args);

    return str_new_from_limit(buf, len);
}

CSTL_EXPORT void str_free(str_t *str)
{
    if (str->beg) {
        cstl_free(str->beg);
    }
    cstl_free(str);
}

// 返回字节数目(byte)
CSTL_EXPORT size_t str_byte_size(const str_t *str)
{
    assert(str);
    return str->len;
}


// 返回字符数目（一个字符可能有多个字节）
CSTL_EXPORT size_t str_char_size(const str_t *str)
{
    assert(str);
    wstr_t *wstr = str_to_wstr(str);
    size_t len = wstr_size(wstr);
    wstr_free(wstr);

    return len;
}

// 内部还可以使用空闲缓存的字节数目
CSTL_EXPORT size_t str_capacity(const str_t *str)
{
    assert(str);
    return str->capacity;
}

// 增加 添加字符串
CSTL_EXPORT str_t* str_append(str_t *str, const char *raw)
{
    int len;

    assert(str);
    if (!raw) return str;

    len = strlen(raw);
    if (str->capacity - str->len <= len) {
        str = _str_realloc(str, __calc_new_capacity(str, len));
    }

    strcpy(str->beg + str->len, raw);
    str->len += len;

    return str;
}

CSTL_EXPORT str_t* str_push_back(str_t *str, char ch)
{
    if (str_byte_size(str) + 1 >= str_capacity(str)) {
        _str_realloc(str, __calc_new_capacity(str, 1));
    }
    str->beg[str->len++] = ch;
    return str;
}

// 在某个位置上进行插入
CSTL_LIB str_t* str_insert(str_t *str, int pos, const char *new_str)
{
    int length;
    
    assert(str);
    if (new_str == NULL || '\0' == *new_str) {
        return str;
    }

    pos = pos < 0 ? 0 : pos;
    pos = (size_t)pos >= str_byte_size(str) ? (int)str_byte_size(str) : pos;

    length = strlen(new_str);

    // 这儿优化，避免不必要的赋值
    if (0 == length) {
        return str;
    }

    if (str_byte_size(str) + length >= str_capacity(str)) {
        _str_realloc(str, __calc_new_capacity(str, length));
    }

    // 将插入点位置上原来元素，统一向后偏移 
    for (int i = str_byte_size(str) - 1; i >= pos; i--) {
        *(str->beg + i + length) = *(str->beg + i);
    }

    memmove(str->beg + pos, new_str, length);
    str->len += length;
    *(str->beg + str->len) = '\0';

    return str;
}

CSTL_EXPORT str_t* str_erase(str_t *str, int pos)
{
    size_t size = str_byte_size(str);
    if (size == 0) return str;

    pos = pos < 0 ? 0 : pos;
    pos = (size_t)pos >= size ? size - 1 : pos;


    size = size - 1;
    memmove(str->beg + pos, str->beg + pos + 1, size - pos);
    *(str->beg + size) = '\0';
    str->len = size;
    return str;
}

// 获取单个字节（有可能数据不完整）
CSTL_EXPORT char str_at_byte(str_t *str, int index)
{
    assert(str);

    if (index > str->len || index < 0) {
        return (char)-1;
    }

    return *(str->beg + index);
}

// 只去除头部空白字符
CSTL_LIB str_t* str_trim_left(str_t *str)
{
    char *pos;
    assert(str);

    // 定位pos到左侧第一个非空字符
    pos = str->beg;
    while (*pos && isspace((int)*pos)) ++pos;

    // 将右侧的所有字符统一向左侧进行拷贝
    if (pos != str->beg && *pos) { //表明左侧有空白字符，右侧有非空白字符
        str->len -= pos - str->beg;
        memmove(str->beg, pos, str->len);
        *(str->beg + str->len) = '\0';
    } else if (*pos == '\0') { //表示所有内容为空
        str->len = 0;
        *(str->beg) = '\0';
    } // 如果长度不变，则不进行处理

    return str;
}

// 只去除尾部空白字符
CSTL_LIB str_t* str_trim_right(str_t *str)
{
    char *pos;
    assert(str);

    // 定位到从右向左起，第一个非空白字符
    pos = str->beg + str->len - 1;
    while (pos >= str->beg && isspace((int)*pos)) {
        pos --;
    }

    str->len = pos - str->beg + 1;
    *(str->beg + str->len) = '\0';

    return str;
}

// 去除收尾空白字符(不移动字符了)
CSTL_LIB str_t* str_trim(str_t *str)
{
    str_trim_left(str);
    str_trim_right(str);
    return str;
}

CSTL_EXPORT char*  str_c_str(str_t *str);
CSTL_EXPORT str_t* str_sub_str(str_t *str, int offset, int length);


// 分割字符集
CSTL_LIB VEC*   str_split(str_t *str, const char *regex)
{
    assert(str && regex);
    VEC *vec = vec_new(sizeof(str_t*), (destroy_func_t)str_ptr_destroy);
    int offset = 0;
    str_t *sub_str;

    regex_t trim_regex;
    regmatch_t trim_match;

    // 先编译
    if (0 != regcomp(&trim_regex, regex, REG_EXTENDED)) {
        return NULL;
    }

    // 进行匹配
    while (0 == regexec(&trim_regex, str->beg + offset, 1, &trim_match, 0)) {
        sub_str = str_sub_str(str, offset, trim_match.rm_so);
        vec_push_back(vec, &sub_str);
        offset += trim_match.rm_eo;
    }

    sub_str = str_sub_str(str, offset, str->len);
    vec_push_back(vec, &sub_str);
    return vec;
}

// 比较字符集
CSTL_LIB int    str_cmp(str_t *str, const char *rhv)
{
    assert(str && rhv);
    return strcmp(str->beg, rhv);
}

////////////////////////////////////////测试函数//////////////////////////////


// 比较两者内容释放相等
CSTL_LIB bool   str_equals(const str_t *str, const char *rhv)
{
    assert(str && rhv);
    return 0 == strcmp(str->beg, rhv);
}

// 忽略大小写比较
CSTL_LIB bool   str_equals_ignore_case(const str_t *str, const char *rhv)
{
    assert(str && rhv);
    return 0 == strcasecmp(str->beg, rhv);
}

// 检测是否为空
CSTL_LIB bool   str_empty(const str_t *str)
{
    assert(str);
    return 0 == str->len;
}

// 支持正则表达式匹配
CSTL_LIB bool   str_matches(str_t *str, const char *regex)
{
    assert(str && regex);
    bool matches = false;
    regex_t match_regex;

    // 首先进行编译
    regcomp(&match_regex, regex, REG_EXTENDED | REG_NOSUB);
    matches = (0 == regexec(&match_regex, str->beg, 0, NULL, 0));
    regfree(&match_regex);

    return matches;
}

// 测试是否以value开头
CSTL_LIB bool   str_starts_with(const str_t *str, const char *value)
{
    int len;

    assert(str && value);
    len = strlen(value);
    return 0 == strncmp(str->beg, value, len);
}

// 返回内部的内部的内容
CSTL_EXPORT char*  str_c_str(str_t *str)
{
    assert(str);
    return str->beg;
}

// 计算hash值
CSTL_EXPORT unsigned int    str_hash_code(str_t *str)
{
    unsigned int hash = 0;

    assert(str);

    for (int i = 0; i < str->len; i++) {
        hash += (unsigned char)*(str->beg + i);
    }
    return hash;
}

CSTL_EXPORT unsigned int   str_ptr_hash_code(str_t **str)
{
    if (NULL == *str) {
        return (unsigned int)0;
    }
    return str_hash_code(*str);
}

// 查找
CSTL_EXPORT int  str_index_of(const str_t *str, int from_index, const char *val)
{
    int len;
    assert(str && val);

    from_index = from_index < 0 ? 0 : from_index;
    len = strlen(val);
    for (int i = from_index; i < str->len; i++) {
        int j;
        for (j = 0; j < len && (i + j) < str->len; j++) {
            if (*(str->beg + i + j) != *(val + j)) {
                break;
            }
        }

        if (j == len) {
            return i;
        }

        if (i + j >= str->len) {
            return -1;
        }
    }
    return -1;
}

CSTL_EXPORT int    str_index_of_char(const str_t *str, int from_index, char val)
{
    assert(str);

    for (int i = from_index; i < str->len; i++) {
        if (*(str->beg + i) == val) {
            return i;
        }
    }

    return -1;
}

CSTL_LIB int    str_rindex_of(const str_t *str, int from_index, const char *val)
{
    int len;
    assert(str);

    if (!*val || '\0' == *val) {
        return -1;
    }

    from_index = (from_index < 0) ? (str->len - 1) : from_index;
    len = strlen(val);

    for (int i = from_index; i >= 0; --i) {
        int j, k;
        for (j = len - 1, k = 0; j >= 0 && (i -k >= 0); --j, k++) {
            if (*(val + j) != *(str->beg + i -k)) {
                break;
            }
        }
        if (j < 0) {
            return i - len + 1;
        }
        if (i < k) {
            return -1;
        }
    }

    return -1;
}

CSTL_LIB int    str_rindex_of_char(const str_t *str, int from_index, char val)
{
    assert(str);

    from_index = (from_index < 0) ? (str->len - 1) : from_index;
    for (int i = from_index; i >= 0; i--) {
        if (*(str->beg + i) == val) {
            return i;
        }
    }
    return -1;
}

CSTL_EXPORT str_t*  str_replace_first(str_t *str, int offset, const char *old_str, const char *new_str);

// 替换str中的内容
CSTL_EXPORT str_t*  str_replace(str_t *str, const char *old_str, const char *new_str)
{
    int offset= 0, index;
    int new_len;

    assert(str && old_str && new_str);

    new_len = strlen(new_str);

    while (-1 != (index = str_index_of(str, offset, old_str))) {
        str_replace_first(str, offset, old_str, new_str);
        offset = index + new_len;
    }
    return str;
}


CSTL_LIB str_t*  str_replace_byte(str_t *str, const char old_ch, char new_ch)
{
    assert(str);

    for (int i = 0; i < str->len; i++) {
        if (old_ch == *(str->beg + i)) {
            *(str->beg + i) = new_ch;
        }
    }

    return str;
}

CSTL_EXPORT str_t*  str_replace_first(str_t *str, int offset, const char *old_str, const char *new_str)
{
    int index;
    int old_len, new_len, diff;

    assert(str && old_str && new_str);

    old_len = strlen(old_str);
    new_len = strlen(new_str);
    diff = old_len - new_len;

    index = str_index_of(str, offset, old_str);
    if (index != -1) {
        // 如果新的比老的长，则原来老的后面的字符要向后偏移
        // 短，则向前编译
        // 相等则不变

        if (diff > 0) { //向前偏移diff
            char *old_str_end = str->beg + index + old_len;
            memmove(old_str_end - diff, old_str_end, str->len - diff);
            str->len -= diff;
            *(str->beg + str->len) = '\0';
        } else if (diff < 0) { // 向后偏移
            diff = -diff;

            if (str->len + diff >= str->capacity) {
                _str_realloc(str, __calc_new_capacity(str, diff));
            }

            int end = index + old_len;
            // 向后拷贝
            for (int i = str->len - 1; i >= end; i--) {
                *(str->beg + i + diff) = *(str->beg + i);
            }
            str->len += diff;
            *(str->beg + str->len) = '\0';
        } 
        memmove(str->beg + index, new_str, new_len);
    }

    return str;
}

CSTL_EXPORT str_t* str_to_lowercase(str_t *str)
{
    assert(str);

    for (int i = 0; i < str->len; i++) {
        char ch = (char)*(str->beg + i);
        if (ch >= 'A' && ch <= 'Z') {
            *(str->beg + i) = ch - 'A' + 'a';
        }
    }
    return str;
}

CSTL_EXPORT str_t* str_to_uppercase(str_t *str)
{
    assert(str);

    for (int i = 0; i < str->len; i++) {
        char ch = (char)*(str->beg + i);
        if (ch >= 'a' && ch <= 'z') {
            *(str->beg + i) = ch - 'a' + 'A';
        }
    }
    return str;
}

// 用来测试
CSTL_LIB bool   str_contains(const str_t *str, const char *sub)
{
    return str_index_of(str, 0, sub) != -1;
}

static str_t *_str_realloc(str_t *str, int new_capacity)
{
    assert(str);
    str->beg = (char*)cstl_realloc(str->beg, new_capacity * sizeof(char));
    str->capacity = new_capacity;
    return str;
}

CSTL_LIB str_t* str_assign(str_t *str, const char *rhv)
{
    assert(str && rhv);
    size_t rhv_len = strlen(rhv);

    if (str_capacity(str) <= rhv_len) {
        _str_realloc(str, __calc_new_capacity(str, rhv_len - str_byte_size(str)));
    }

    *str->beg = '\0';
    strcpy(str->beg, rhv);
    str->len = rhv_len;
    return str;
}

CSTL_EXPORT str_t* str_sub_str(str_t *str, int offset, int length)
{
    assert(str);

    offset = (offset < 0 ? 0 : (offset >= str->len ? str->len : offset));
    length = (length < 0 ? 0 : (length + offset  >= str->len ? str->len - offset : length));

    return str_new_from_limit(str->beg + offset, length);
}

// 如果buflen 不够的话，那么就通过errno来返回一个值，表明缓冲区长度不足
// 末尾即使buf长度不足，也会放入'\0', 只是有效值少一个
CSTL_EXPORT int str_sub_str_buf(str_t *str, int offset, int length,
        char *buf, int buflen, int *err)
{
    assert(str);

    *err = 0;
    offset = (offset < 0 ? 0 : (offset >= str->len ? str->len : offset));
    length = (length < 0 ? 0 : (length + offset  >= str->len ? str->len - offset : length));

    if (NULL == buf) {
        return length + 1;
    }
    assert(str && buf && buflen > 0);

    // 即使两者容量相同，因为会在末尾增加'\0', buf有效长度是buflen - 1
    if (buflen <= length) {
        length = buflen - 1;
        *err = ENOMEM; //表明buf不是不够大不足
    }

    strncpy(buf, str->beg + offset, length);
    *(buf + length) = '\0';

    return length;
}

CSTL_EXPORT void str_ptr_destroy(str_t **str)
{
    assert(*str);
    str_free(*str);
}

// 切割行
CSTL_EXPORT VEC*   str_split_lines(str_t *str)
{
    return str_split(str, "\n");
}

// 此方法主要是为了预分配缓存而设置
CSTL_EXPORT str_t* str_resize(str_t *str, size_t new_size, char ch)
{
    assert(str);

    if (new_size >= str_capacity(str)) {
        _str_realloc(str, new_size + 1);
    }

    // 增大
    if (new_size > (size_t)str->len) {
        memset(str->beg + str->len, ch, new_size - str->len);
        if (ch == '\0') {
            *(str->beg + new_size) = '\0'; //容量变化，但是字符串的长度病没有发生变化
            return str;
        }
    }

    *(str->beg + new_size) = '\0'; //容量变化，字符串长度改变了
    str->len = new_size;
    return str;
}
