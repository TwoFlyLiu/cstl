/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 14 19:08:54
*/
#include <check_util.h>
#include <stdio.h>
#include <assert.h>

#include "test_common.h"
#include "str_conv.h"
#include "str.h"
#include "wstr.h"
#include "utf8_str.h"

START_TEST(test_wstr_to_str) {
    str_t *str = str_new_from("hello, 中国");
    wstr_t *wstr = wstr_new_from(L"hello, 中国");

    str_t *converted = wstr_to_str(wstr);
    ck_assert_str_eq(str_c_str(str), str_c_str(converted));
    str_free(converted);

    wstr_assign(wstr, L"");
    converted = wstr_to_str(wstr);
    ck_assert_str_eq("", str_c_str(converted));
    str_free(converted);

    str_free(str);
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_str_to_wstr) {
    str_t *str = str_new_from("hello, 中国");
    wstr_t *wstr = wstr_new_from(L"hello, 中国");

    wstr_t *converted = str_to_wstr(str);
    ck_assert_wstr_eq(wstr_c_wstr(wstr), wstr_c_wstr(converted));
    wstr_free(converted);

    str_assign(str, "");
    converted = str_to_wstr(str);
    ck_assert_wstr_eq(L"", wstr_c_wstr(converted));
    wstr_free(converted);

    str_free(str);
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_wchar_to_u8) {
    char *u8 = "hello, \xe4\xb8\xad\xe5\x9b\xbd"; // 对于msys, 默认编码就是u8
    wstr_t *wstr = wstr_new_from(L"hello, 中国");

    u8_str_t *converted = wstr_to_u8_str(wstr);
    ck_assert_str_eq(u8, u8_str_c_u8_str(converted));
    u8_str_free(converted);

    wstr_assign(wstr, L"");
    converted = wstr_to_u8_str(wstr);
    ck_assert_str_eq("", u8_str_c_u8_str(converted));
    u8_str_free(converted);

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_str_to_utf8) {
    str_t *str = str_new_from("hello, 中国");
    u8_str_t *u8 = str_to_u8_str(str);
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    str_assign(str, "");
    u8 = str_to_u8_str(str);
    ck_assert_str_eq("", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_utf8_to_wstr) {
    u8_str_t *u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b\xbd");

    wstr_t *wstr = u8_str_to_wstr(u8);
    ck_assert_wstr_eq(L"hello, 中国", wstr_c_wstr(wstr));
    wstr_free(wstr);

    u8_str_assign_u8(u8, "");
    wstr = u8_str_to_wstr(u8);
    ck_assert_wstr_eq(L"", wstr_c_wstr(wstr));
    wstr_free(wstr);

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_utf8_to_str) {
    u8_str_t *u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b\xbd");

    str_t *str = u8_str_to_str(u8);
    ck_assert_str_eq("hello, 中国", str_c_str(str));
    str_free(str);

    u8_str_assign_u8(u8, "");
    str = u8_str_to_str(u8);
    ck_assert_str_eq("", str_c_str(str));
    str_free(str);

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_wchar_to_gbk) {
    wstr_t *wstr = wstr_new_from(L"hello, 中国");

    str_t *gbk = wstr_to_gbk(wstr);
    ck_assert_str_eq("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa", str_c_str(gbk));
    str_free(gbk);

    wstr_assign(wstr, L"");
    gbk = wstr_to_gbk(wstr);
    ck_assert_str_eq("", str_c_str(gbk));
    str_free(gbk);

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_utf8_to_gbk) {
    u8_str_t *u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b\xbd");

    str_t *gbk = u8_str_to_gbk(u8);
    ck_assert_str_eq("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa", str_c_str(gbk));
    str_free(gbk);

    u8_str_assign_u8(u8, "");
    gbk = u8_str_to_gbk(u8);
    ck_assert_str_eq("", str_c_str(gbk));
    str_free(gbk);

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_gbk_to_wstr) {
    str_t *gbk = str_new_from("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa");

    wstr_t *wstr = gbk_to_wstr(gbk);
    ck_assert_wstr_eq(L"hello, 中国", wstr_c_wstr(wstr));
    wstr_free(wstr);

    str_assign(gbk, "");
    wstr = gbk_to_wstr(gbk);
    ck_assert_wstr_eq(L"", wstr_c_wstr(wstr));
    wstr_free(wstr);

    str_free(gbk);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_gbk_to_utf8) {
    str_t *gbk = str_new_from("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa");

    u8_str_t *u8 = gbk_to_u8_str(gbk);
    ck_assert_str_eq("hello, 中国", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    str_free(gbk);
    ck_assert_no_leak();
}
END_TEST

/*START_TEST(test_part_invalid_gbk_to_utf8) {*/
    /*FILE *fp = fopen("./ybtx.html", "r");*/
    /*assert(fp && "open ybtx.html failed!");*/

    /*char buf[256];*/
    /*str_t *gbk = str_new();*/
    /*int nread;*/

    /*// 先读取整个文件*/
    /*while ((nread = fread(buf, 1, 256, fp)) > 0) {*/
        /*buf[nread] = '\0';*/
        /*str_append(gbk, buf);*/
    /*}*/
    /*fclose(fp);*/

    /*// 然后将gbk转换为utf8*/
    /*u8_str_t *u8 = gbk_to_u8_str(gbk);*/
    /*[>printf("%s\n", u8_str_c_u8_str(u8));<]*/
    /*ck_assert(u8_str_size(u8) > 0);*/

    /*u8_str_free(u8);*/
    /*str_free(gbk);*/
    /*ck_assert_no_leak();*/
/*}*/
/*END_TEST*/

START_TEST(test_gbk_to_str) {
    str_t *gbk = str_new_from("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa");

    str_t *str = gbk_to_str(gbk);
    ck_assert_str_eq("hello, 中国", str_c_str(str));
    str_free(str);

    str_assign(gbk, "");
    str = gbk_to_str(gbk);
    ck_assert_str_eq("", str_c_str(str));
    str_free(str);
    
    str_free(gbk);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_str_to_gbk) {
    str_t *str = str_new_from("hello, 中国");

    str_t *gbk = str_to_gbk(str);
    ck_assert_str_eq("\x68\x65\x6c\x6c\x6f\x2c\x20\xd6\xd0\xb9\xfa", str_c_str(gbk));
    str_free(gbk);

    str_assign(str, "");
    gbk = str_to_gbk(str);
    ck_assert_str_eq("", str_c_str(gbk));
    str_free(gbk);

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_DEFINE_SUITE(str_conv)
    TEST(test_wstr_to_str)
    TEST(test_str_to_wstr)
    TEST(test_wchar_to_u8)
    TEST(test_str_to_utf8)
    TEST(test_utf8_to_wstr)
    TEST(test_utf8_to_str)
    TEST(test_wchar_to_gbk)
    TEST(test_utf8_to_gbk)
    TEST(test_gbk_to_wstr)
    TEST(test_gbk_to_utf8)
    /*[>TEST(test_part_invalid_gbk_to_utf8)<]*/
    TEST(test_gbk_to_str)
    TEST(test_str_to_gbk)
END_DEFINE_SUITE()
