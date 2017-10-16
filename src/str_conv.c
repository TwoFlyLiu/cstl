/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 14 18:36:54
*/
#include <locale.h>
#include <assert.h>
#include <iconv.h>
#include <string.h>

#include "cstl_stddef.h"
#include "str_conv.h"
#include "str.h"
#include "wstr.h"
#include "utf8_str.h"
#include "leak.h"

CSTL_EXPORT str_t *wstr_to_str(const wstr_t *wstr)
{
    assert(wstr);

    int len = wstr_size(wstr) * 3 + 1;
    char *buf = (char*)cstl_malloc(len); //不适用char buf[len], 因为存储在栈上，是有限制的
    char *old_locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "");
    *buf = '\0';

    len = wcstombs(buf, wstr_c_wstr((wstr_t*)wstr), len);
    *(buf + len) = '\0';

    setlocale(LC_ALL, old_locale);
    str_t *str = str_new_from(buf);

    cstl_free(buf);
    return str;
}

CSTL_EXPORT u8_str_t *wstr_to_u8_str(const wstr_t *wstr)
{
    assert(wstr);

    const char *to_code = "UTF-8//IGNORE";
    const char *from_code = "UTF-16LE"; // msys上，utf16是小端存储

    int len = wstr_size(wstr) * 3 + 1;
    char *buf = (char*)cstl_malloc(len);
    *buf = '\0';

    char *out = buf;
    size_t out_left = len;

    char *in = (char*)wstr_c_wstr((wstr_t*)wstr);
    size_t in_left = (wstr_size(wstr) + 1) * sizeof(wchar_t);

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = '\0'; //添加空结束标志

    u8_str_t *u8 = u8_str_new_from_u8(buf);
    cstl_free(buf);
    return u8;
}


CSTL_EXPORT wstr_t *str_to_wstr(const str_t *str)
{
    int len = str_byte_size(str) + 1;
    wchar_t *buf = cstl_malloc(len);
    char *old_locale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "");
    *buf = '\0';

    len = mbstowcs(buf, str_c_str((str_t*)str), len);
    *(buf + len) = '\0';

    setlocale(LC_ALL, old_locale);
    wstr_t *wstr = wstr_new_from(buf);
    cstl_free(buf);
    return wstr;
}

//str是不确定字符集类型，所以通过wchar_t中间类型转换
CSTL_EXPORT u8_str_t *str_to_u8_str(const str_t *str)
{
    wstr_t *wstr = str_to_wstr(str);
    u8_str_t *u8 = wstr_to_u8_str(wstr);
    wstr_free(wstr);

    return u8;
}

CSTL_EXPORT wstr_t *u8_str_to_wstr(const u8_str_t *u8_str)
{
    assert(u8_str);

    const char *to_code = "UTF-16LE//IGNORE"; // msys上，utf16是小端存储
    const char *from_code = "UTF-8";

    int len = u8_str_size(u8_str) * 2  + 1;
    wchar_t *buf = (wchar_t*)cstl_malloc(len * sizeof(wchar_t));
    *buf = L'\0';

    char *out = (char*)buf;
    size_t out_left = len * sizeof(wchar_t);

    char *in = (char*)u8_str_c_u8_str((u8_str_t*)u8_str);
    size_t in_left = u8_str_byte_count(u8_str);

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = L'\0'; //添加空结束标志

    wstr_t *wstr = wstr_new_from(buf);
    cstl_free(buf);
    return wstr;
}

CSTL_EXPORT str_t *u8_str_to_str(const u8_str_t *u8_str)
{
    assert(u8_str);
    wstr_t *wstr = u8_str_to_wstr(u8_str);
    str_t *str = wstr_to_str(wstr);
    wstr_free(wstr);
    return str;
}

CSTL_EXPORT str_t *wstr_to_gbk(const wstr_t *wstr)
{
    assert(wstr);

    const char *to_code = "GB18030//IGNORE";
    const char *from_code = "UTF-16LE"; // msys上，utf16是小端存储

    int len = wstr_size(wstr) * 2 + 1;
    char *buf = (char*)cstl_malloc(len);
    *buf = '\0';

    char *out = buf;
    size_t out_left = len;

    char *in = (char*)wstr_c_wstr((wstr_t*)wstr);
    size_t in_left = (wstr_size(wstr) + 1) * sizeof(wchar_t);

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = '\0'; //添加空结束标志

    str_t *gbk = str_new_from(buf);
    cstl_free(buf);
    return gbk;
}

CSTL_EXPORT str_t *u8_str_to_gbk(const u8_str_t *u8)
{
    assert(u8);

    const char *to_code = "GB18030//IGNORE";
    const char *from_code = "UTF-8"; // msys上，utf16是小端存储

    int len = u8_str_size(u8) * 2 + 1;
    char *buf = (char*)cstl_malloc(len);
    *buf = '\0';

    char *out = buf;
    size_t out_left = len;

    char *in = (char*)u8_str_c_u8_str((u8_str_t*)u8);
    size_t in_left = u8_str_byte_count(u8);

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = '\0'; //添加空结束标志

    str_t *gbk = str_new_from(buf);
    cstl_free(buf);
    return gbk;
}

CSTL_EXPORT wstr_t *gbk_to_wstr(const str_t *gbk)
{
    assert(gbk);

    const char *to_code = "UTF-16LE//IGNORE"; // msys上，utf16是小端存储
    const char *from_code = "GB18030";

    int gbk_len = str_byte_size(gbk);
    int len = gbk_len * 2  + 1;
    wchar_t *buf = (wchar_t*)cstl_malloc(len * sizeof(wchar_t));
    *buf = L'\0';

    char *out = (char*)buf;
    size_t out_left = len * sizeof(wchar_t);

    char *in = str_c_str((str_t*)gbk);
    size_t in_left = gbk_len + 1;

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = L'\0'; //添加空结束标志

    wstr_t *wstr = wstr_new_from(buf);
    cstl_free(buf);
    return wstr;
}

CSTL_EXPORT u8_str_t *gbk_to_u8_str(const str_t *gbk)
{
    assert(gbk);

    const char *to_code = "UTF-8//IGNORE";
    const char *from_code = "GB18030"; // msys上，utf16是小端存储

    int gbk_len = str_byte_size(gbk);
    int len = gbk_len * 3 + 1;
    char *buf = (char*)cstl_malloc(len);
    *buf = '\0';

    char *out = buf;
    size_t out_left = len;

    char *in = str_c_str((str_t*)gbk);
    size_t in_left = gbk_len + 1;

    iconv_t  icv = iconv_open(to_code, from_code);
    assert((iconv_t)-1 != icv);

    assert(-1 != iconv(icv, &in, &in_left, &out, &out_left));
    iconv_close(icv);

    *out = '\0'; //添加空结束标志

    u8_str_t *u8 = u8_str_new_from_u8(buf);
    cstl_free(buf);
    return u8;
}

CSTL_EXPORT str_t  *str_to_gbk(const str_t *str)
{
    wstr_t *wstr = str_to_wstr(str);
    str_t  *gbk = wstr_to_gbk(wstr);
    wstr_free(wstr);
    return gbk;
}

CSTL_EXPORT str_t  *gbk_to_str(const str_t *gbk)
{
    wstr_t *wstr = gbk_to_wstr(gbk);
    str_t *str = wstr_to_str(wstr);
    wstr_free(wstr);
    return str;
}
