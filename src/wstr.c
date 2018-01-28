/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 11 15:37:39
*/
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <regex.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <locale.h>

#include "cstl_stddef.h"
#include "leak.h"
#include "wstr.h"

struct CSTLWString {
    wchar_t *beg;
    int len; // 当前元素的长度
    int capacity; //最多存放元素数目（包含L'\0')
};

typedef struct CSTLWString wstr_t;

#define STR_BUF_SIZE 1024

// 居然有不清晰声明（在msys2中）
static wstr_t *_wstr_realloc(wstr_t *wstr, int new_capacity);
int wcscasecmp(const wchar_t *s1, const wchar_t *s2);

static inline size_t __calc_new_capacity(wstr_t *wstr, size_t to_append_wchar_count)
{
    return wstr_size(wstr) + CSTL_MAX(wstr_size(wstr), to_append_wchar_count) + 1;
}

CSTL_EXPORT wstr_t * wstr_new()
{
    wstr_t *new_wstr = (wstr_t*)cstl_malloc(sizeof(wstr_t));
    new_wstr->beg = (wchar_t*)cstl_malloc(sizeof(wchar_t));
    *(new_wstr->beg) = L'\0'; //即使为空，也有一个L'\0'
    new_wstr->capacity = 1;
    new_wstr->len = 0;
    return new_wstr;
}


CSTL_EXPORT wstr_t * wstr_new_from_limit(const wchar_t *wstr, int len)
{
    if (wstr == NULL || L'\0' == wstr || len <= 0) {
        return wstr_new();
    }

    wstr_t *new_wstr = (wstr_t*)cstl_malloc(sizeof(wstr_t));

    new_wstr->capacity = len * 2 + 1;
    new_wstr->beg = (wchar_t*)cstl_malloc(new_wstr->capacity * sizeof(wchar_t));
    wcsncpy(new_wstr->beg, wstr, len); //如果wstr长度大于等于len, 那么末尾就不会有L'\0'
    *(new_wstr->beg + len) = L'\0';
    new_wstr->len = len;

    return new_wstr;
}

CSTL_EXPORT wstr_t * wstr_new_from(const wchar_t *wstr)
{
    return wstr_new_from_limit(wstr, wcslen(wstr));
}


CSTL_EXPORT wstr_t * wstr_format(const wchar_t *fmt, ...)
{
    va_list args;
    wchar_t buf[STR_BUF_SIZE];
    int len;

    va_start (args, fmt);
    len = vswprintf(buf, (size_t)STR_BUF_SIZE, fmt, args);
    va_end (args);

    return wstr_new_from_limit(buf, len);
}

CSTL_EXPORT void wstr_free(wstr_t *wstr)
{
    if (wstr->beg) {
        cstl_free(wstr->beg);
    }
    cstl_free(wstr);
}

// 返回字节数目(byte)
CSTL_EXPORT inline size_t wstr_size(const wstr_t *wstr)
{
    assert(wstr);
    return wstr->len;
}

// 内部还可以使用空闲缓存的字节数目
CSTL_EXPORT size_t wstr_capacity(const wstr_t *wstr)
{
    assert(wstr);
    return wstr->capacity;
}

// 增加 添加字符串
CSTL_EXPORT wstr_t* wstr_append(wstr_t *wstr, const wchar_t *raw)
{
    int len;

    assert(wstr);
    if (!raw) return wstr;

    len = wcslen(raw);
    if (wstr_size(wstr) + len >= wstr_capacity(wstr)) {
        _wstr_realloc(wstr, __calc_new_capacity(wstr, len));
    }

    wcscpy(wstr->beg + wstr->len, raw);
    wstr->len += len;

    return wstr;
}

// 在某个位置上进行插入
CSTL_LIB wstr_t* wstr_insert(wstr_t *wstr, int pos, const wchar_t *new_wstr)
{
    int length;
    
    assert(wstr);
    if (new_wstr == NULL || L'\0' == *new_wstr) {
        return wstr;
    }

    pos = pos < 0 ? 0 : pos;
    pos = (size_t)pos >= wstr_size(wstr) ? (int)wstr_size(wstr) : pos;

    length = wcslen(new_wstr);

    // 这儿优化，避免不必要的赋值
    if (0 == length) {
        return wstr;
    }

    if (wstr_size(wstr) + length >= wstr_capacity(wstr)) {
        _wstr_realloc(wstr, __calc_new_capacity(wstr, length));
    }

    // 将插入点位置上原来元素，统一向后偏移 
    for (int i = wstr_size(wstr) - 1; i >= pos; i--) {
        *(wstr->beg + i + length) = *(wstr->beg + i);
    }

    memmove(wstr->beg + pos, new_wstr, length * sizeof(wchar_t));
    wstr->len += length;
    *(wstr->beg + wstr->len) = L'\0';

    return wstr;
}

// 获取单个字节（有可能数据不完整）
CSTL_EXPORT wchar_t wstr_at(const wstr_t *wstr, int index)
{
    assert(wstr);

    if (wstr->len < index || index < 0) {
        return (wchar_t)-1;
    }

    return *(wstr->beg + index);
}

// 只去除头部空白字符
CSTL_LIB wstr_t* wstr_trim_left(wstr_t *wstr)
{
    wchar_t *pos;
    assert(wstr);

    // 定位pos到左侧第一个非空字符
    pos = wstr->beg;
    while (*pos && isspace((int)*pos)) ++pos;

    // 将右侧的所有字符统一向左侧进行拷贝
    if (pos != wstr->beg && *pos) { //表明左侧有空白字符，右侧有非空白字符
        wstr->len -= pos - wstr->beg;
        memmove(wstr->beg, pos, wstr->len * sizeof(wchar_t));
        *(wstr->beg + wstr->len) = L'\0';
    } else if (*pos == L'\0') { //表示所有内容为空
        wstr->len = 0;
        *(wstr->beg) = L'\0';
    } // 如果长度不变，则不进行处理

    return wstr;
}

// 只去除尾部空白字符
CSTL_LIB wstr_t* wstr_trim_right(wstr_t *wstr)
{
    wchar_t *pos;
    assert(wstr);

    // 定位到从右向左起，第一个非空白字符
    pos = wstr->beg + wstr->len - 1;
    while (pos >= wstr->beg && isspace((int)*pos)) {
        pos --;
    }

    wstr->len = pos - wstr->beg + 1;
    *(wstr->beg + wstr->len) = L'\0';

    return wstr;
}

// 去除收尾空白字符(不移动字符了)
CSTL_LIB wstr_t* wstr_trim(wstr_t *wstr)
{
    wstr_trim_left(wstr);
    wstr_trim_right(wstr);
    return wstr;
}

CSTL_EXPORT wchar_t*  wstr_c_wstr(wstr_t *wstr);
CSTL_EXPORT wstr_t* wstr_sub_wstr(const wstr_t *wstr, int offset, int length);


// 分割字符集
CSTL_LIB VEC*   wstr_split(const wstr_t *wstr, const wchar_t *regex)
{
    assert(wstr && regex);
    VEC *vec = vec_new(sizeof(wstr_t*), (destroy_func_t)wstr_ptr_destroy);
    wstr_t *sub_wstr;

    regex_t trim_regex;
    regmatch_t trim_match;

    // 转换宽字符正则到窄字符正则
    int regex_len = wcslen(regex) * 3 + 1;
    char regex_str[regex_len];

    char *old_locale = setlocale(LC_ALL, NULL); // 先保存老的locale
    setlocale(LC_ALL, "");
    int end = wcstombs(regex_str, regex, regex_len);
    assert(end != -1);
    *(regex_str + end) = '\0';

    // 先编译
    if (0 != regcomp(&trim_regex, regex_str, REG_EXTENDED)) {
        return NULL;
    }

    int this_str_len = 3 * wstr_size(wstr) + 1;
    char this_str[this_str_len];
    char tmp_str[this_str_len];
    wchar_t tmp_wstr[wstr_size(wstr)];

    end = wcstombs(this_str, wstr_c_wstr((wstr_t*)wstr), this_str_len);
    assert(end != -1);
    *(this_str + end) = '\0';

    int offset = 0;
    // 进行匹配
    while (0 == regexec(&trim_regex, this_str + offset, 1, &trim_match, 0)) {
        // 将窄字符转化为宽字符返回出去
        strncpy(tmp_str, this_str + offset, trim_match.rm_so);
        *(tmp_str + trim_match.rm_so) = '\0';

        end = mbstowcs(tmp_wstr, tmp_str, wstr_size(wstr));
        assert(end != -1);
        *(tmp_wstr + end) = L'\0';

        sub_wstr = wstr_new_from(tmp_wstr);
        vec_push_back(vec, &sub_wstr);
        offset += trim_match.rm_eo;
    }

    sub_wstr = wstr_sub_wstr(wstr, offset, wstr->len);
    vec_push_back(vec, &sub_wstr);
    setlocale(LC_ALL, old_locale);
    return vec;
}

// 比较字符集
CSTL_LIB int    wstr_cmp(const wstr_t *wstr, const wchar_t *rhv)
{
    assert(wstr && rhv);
    return wcscmp(wstr->beg, rhv);
}

////////////////////////////////////////测试函数//////////////////////////////


// 比较两者内容释放相等
CSTL_LIB bool   wstr_equals(const wstr_t *wstr, const wchar_t *rhv)
{
    assert(wstr && rhv);
    return 0 == wcscmp(wstr->beg, rhv);
}

// 忽略大小写比较
CSTL_LIB bool   wstr_equals_ignore_case(const wstr_t *wstr, const wchar_t *rhv)
{
    assert(wstr && rhv);
    return 0 == wcscasecmp(wstr->beg, rhv);
}

// 检测是否为空
CSTL_LIB bool   wstr_empty(const wstr_t *wstr)
{
    assert(wstr);
    return 0 == wstr->len;
}

// 支持正则表达式匹配
CSTL_LIB bool   wstr_matches(const wstr_t *wstr, const wchar_t *regex)
{
    assert(wstr && regex);
    bool matches = false;
    regex_t match_regex;

    // 将正则表达式宽字符串转换为窄字符串
    int regex_len = wcslen(regex);
    char regex_str[3 * regex_len + 1];

    char *old_locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "");
    int end = wcstombs(regex_str, regex, 3 * regex_len);

    if (-1 == end) {
        return false;
    }
    *(regex_str + end) = '\0';

    // 首先进行编译
    regcomp(&match_regex, regex_str, REG_EXTENDED | REG_NOSUB);

    // 将要进行匹配的字符串转化为窄字符串
    // 利用c99的这个动态数组特性，就不用使用malloc了
    char this_str[3 * wstr_size(wstr) + 1];
    end = wcstombs(this_str, wstr_c_wstr((wstr_t*)wstr), 3 * wstr_size(wstr) + 1);

    if (-1 == end) {
        return false;
    }
    *(this_str + end) = '\0';

    matches = (0 == regexec(&match_regex, this_str, 0, NULL, 0));
    regfree(&match_regex);

    setlocale(LC_ALL, old_locale);
    return matches;
}

// 测试是否以value开头
CSTL_LIB bool   wstr_starts_with(const wstr_t *wstr, const wchar_t *value)
{
    int len;

    assert(wstr && value);
    len = wcslen(value);
    return 0 == wcsncmp(wstr->beg, value, len);
}

// 返回内部的内部的内容
CSTL_EXPORT wchar_t*  wstr_c_wstr(wstr_t *wstr)
{
    assert(wstr);
    return wstr->beg;
}

// 计算hash值
CSTL_EXPORT unsigned int    wstr_hash_code(const wstr_t *wstr)
{
    unsigned int hash = 0;

    assert(wstr);

    for (int i = 0; i < wstr->len; i++) {
        hash += (unsigned int)*(wstr->beg + i);
    }
    return hash;
}

CSTL_EXPORT unsigned int    wstr_ptr_hash_code(const wstr_t **wstr)
{
    assert(wstr);
    if (NULL == *wstr) {
        return 0;
    }
    return wstr_hash_code(*wstr);
}

// 查找
CSTL_EXPORT int  wstr_index_of(const wstr_t *wstr, int from_index, const wchar_t *val)
{
    int len;
    assert(wstr && val);

    from_index = from_index < 0 ? 0 : from_index;
    len = wcslen(val);
    for (int i = from_index; i < wstr->len; i++) {
        int j;
        for (j = 0; j < len && (i + j) < wstr->len; j++) {
            if (*(wstr->beg + i + j) != *(val + j)) {
                break;
            }
        }

        if (j == len) {
            return i;
        }

        if (i + j >= wstr->len) {
            return -1;
        }
    }
    return -1;
}

CSTL_EXPORT int    wstr_index_of_wchar(const wstr_t *wstr, int from_index, wchar_t val)
{
    assert(wstr);

    from_index = from_index < 0 ? 0 : from_index;
    for (int i = from_index; i < wstr->len; i++) {
        if (*(wstr->beg + i) == val) {
            return i;
        }
    }

    return -1;
}

CSTL_LIB int    wstr_rindex_of(const wstr_t *wstr, int from_index, const wchar_t *val)
{
    int len;
    assert(wstr);

    if (!*val || L'\0' == *val) {
        return -1;
    }

    from_index = (from_index < 0) ? (wstr->len - 1) : from_index;
    len = wcslen(val);

    for (int i = from_index; i >= 0; --i) {
        int j, k;
        for (j = len - 1, k = 0; j >= 0 && (i -k >= 0); --j, k++) {
            if (*(val + j) != *(wstr->beg + i -k)) {
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

CSTL_LIB int    wstr_rindex_of_wchar(const wstr_t *wstr, int from_index, wchar_t val)
{
    assert(wstr);

    from_index = (from_index < 0) ? (wstr->len - 1) : from_index;
    for (int i = from_index; i >= 0; i--) {
        if (*(wstr->beg + i) == val) {
            return i;
        }
    }
    return -1;
}

CSTL_EXPORT wstr_t*  wstr_replace_first(wstr_t *wstr, int offset, const wchar_t *old_wstr, const wchar_t *new_wstr);

// 替换wstr中的内容
CSTL_EXPORT wstr_t*  wstr_replace(wstr_t *wstr, const wchar_t *old_wstr, const wchar_t *new_wstr)
{
    int offset= 0, index;
    int new_len;

    assert(wstr && old_wstr && new_wstr);

    new_len = wcslen(new_wstr);

    while (-1 != (index = wstr_index_of(wstr, offset, old_wstr))) {
        wstr_replace_first(wstr, offset, old_wstr, new_wstr);
        offset = index + new_len;
    }
    return wstr;
}


CSTL_LIB wstr_t*  wstr_replace_wchar(wstr_t *wstr, const wchar_t old_ch, wchar_t new_ch)
{
    assert(wstr);

    for (int i = 0; i < wstr->len; i++) {
        if (old_ch == *(wstr->beg + i)) {
            *(wstr->beg + i) = new_ch;
        }
    }

    return wstr;
}

CSTL_EXPORT wstr_t*  wstr_replace_first(wstr_t *wstr, int offset, const wchar_t *old_wstr, const wchar_t *new_wstr)
{
    int index;
    int old_len, new_len, diff;

    assert(wstr && old_wstr && new_wstr);

    old_len = wcslen(old_wstr);
    new_len = wcslen(new_wstr);
    diff = old_len - new_len;

    index = wstr_index_of(wstr, offset, old_wstr);
    if (index != -1) {
        // 如果新的比老的长，则原来老的后面的字符要向后偏移
        // 短，则向前编译
        // 相等则不变

        if (diff > 0) { //向前偏移diff
            wchar_t *old_wstr_end = wstr->beg + index + old_len;
            memmove(old_wstr_end - diff, old_wstr_end, (wstr->len - diff) * sizeof(wchar_t));
            wstr->len -= diff;
            *(wstr->beg + wstr->len) = L'\0';
        } else if (diff < 0) { // 向后偏移
            diff = -diff;

            if (wstr->len + diff >= wstr->capacity) {
                _wstr_realloc(wstr, __calc_new_capacity(wstr, diff));
            }

            int end = index + old_len;
            // 向后拷贝
            for (int i = wstr->len - 1; i >= end; i--) {
                *(wstr->beg + i + diff) = *(wstr->beg + i);
            }
            wstr->len += diff;
            *(wstr->beg + wstr->len) = L'\0';
        } 
        memmove(wstr->beg + index, new_wstr, new_len * sizeof(wchar_t));
    }

    return wstr;
}

CSTL_EXPORT wstr_t* wstr_to_lowercase(wstr_t *wstr)
{
    assert(wstr);

    for (int i = 0; i < wstr->len; i++) {
        wchar_t ch = (wchar_t)*(wstr->beg + i);
        if (ch >= L'A' && ch <= L'Z') {
            *(wstr->beg + i) = ch - L'A' + L'a';
        }
    }
    return wstr;
}

CSTL_EXPORT wstr_t* wstr_to_uppercase(wstr_t *wstr)
{
    assert(wstr);

    for (int i = 0; i < wstr->len; i++) {
        wchar_t ch = (wchar_t)*(wstr->beg + i);
        if (ch >= L'a' && ch <= L'z') {
            *(wstr->beg + i) = ch - L'a' + L'A';
        }
    }
    return wstr;
}

// 用来测试
CSTL_LIB bool   wstr_contains(const wstr_t *wstr, const wchar_t *sub)
{
    return wstr_index_of(wstr, 0, sub) != -1;
}

static wstr_t *_wstr_realloc(wstr_t *wstr, int new_capacity)
{
    wstr->beg = (wchar_t*)cstl_realloc(wstr->beg, new_capacity * sizeof(wchar_t));
    wstr->capacity = new_capacity;
    return wstr;
}

CSTL_LIB wstr_t* wstr_assign(wstr_t *wstr, const wchar_t *rhv)
{
    assert(wstr && rhv);
    size_t rhv_len = wcslen(rhv);

    if (wstr_capacity(wstr) <= rhv_len) {
        _wstr_realloc(wstr, __calc_new_capacity(wstr, rhv_len - wstr_size(wstr)));
    }

    *wstr->beg = L'\0';
    wcscpy(wstr->beg, rhv);
    wstr->len = rhv_len;
    return wstr;
}

CSTL_EXPORT wstr_t* wstr_sub_wstr(const wstr_t *wstr, int offset, int length)
{
    assert(wstr);

    offset = (offset < 0 ? 0 : (offset >= wstr->len ? wstr->len : offset));
    length = (length < 0 ? 0 : (length + offset  >= wstr->len ? wstr->len - offset : length));

    return wstr_new_from_limit(wstr->beg + offset, length);
}

// 如果buflen 不够的话，那么就通过errno来返回一个值，表明缓冲区长度不足
// 末尾即使buf长度不足，也会放入L'\0', 只是有效值少一个
CSTL_EXPORT int wstr_sub_wstr_buf(const wstr_t *wstr, int offset, int length,
        wchar_t *buf, int buflen, int *err)
{
    assert(wstr);

    *err = 0;
    offset = (offset < 0 ? 0 : (offset >= wstr->len ? wstr->len : offset));
    length = (length < 0 ? 0 : (length + offset  >= wstr->len ? wstr->len - offset : length));

    if (NULL == buf) {
        return length + 1;
    }
    assert(buf && buflen > 0);

    // 即使两者容量相同，因为会在末尾增加L'\0', buf有效长度是buflen - 1
    if (buflen <= length) {
        length = buflen - 1;
        *err = ENOMEM; //表明buf不是不够大不足
    }

    wcsncpy(buf, wstr->beg + offset, length);
    *(buf + length) = L'\0';

    return length;
}

CSTL_EXPORT void wstr_ptr_destroy(wstr_t **wstr)
{
    assert(*wstr);
    wstr_free(*wstr);
}

// 切割行
CSTL_EXPORT VEC*   wstr_split_lines(const wstr_t *wstr)
{
    return wstr_split(wstr, L"\n");
}

CSTL_EXPORT wstr_t* wstr_resize(wstr_t *wstr, size_t new_size, wchar_t value)
{
    assert(wstr);

    if (new_size >= wstr_capacity(wstr)) {
        _wstr_realloc(wstr, new_size + 1);
    }

    if (new_size > (size_t)wstr->len) {
        for (size_t i = wstr->len; i < new_size; i++) {
            *(wstr->beg + i) = value;
        }

        if (value == L'\0') {
            *(wstr->beg + new_size) = L'\0'; 
            return wstr;
        }
    }

    *(wstr->beg + new_size) = L'\0'; //容量变化，但是字符串的长度病没有发生变化
    wstr->len = new_size;
    return wstr;
}
