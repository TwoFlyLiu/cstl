/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 12 19:28:10
*/
#include <check_util.h>
#include <wchar.h>
#include <assert.h>
#include "test_common.h"
#include <regex.h>
#include <errno.h>
#include "wstr.h"

START_TEST(test_new_free) {
    // 不带参数的new
    wstr_t *wstr = wstr_new();
    ck_assert_int_eq(wstr_size(wstr), 0);
    ck_assert_int_eq(wstr_capacity(wstr), 1); //有个'\0', 所以容量是1
    ck_assert(wstr_at(wstr, 0) == '\0');
    wstr_free(wstr);

    // new_from_wstr
    wchar_t name[] = L"hello world";
    wstr = wstr_new_from(name);
    ck_assert_int_eq(wcslen(name), wstr_size(wstr));
    ck_assert_wstr_eq(name, wstr_c_wstr(wstr));

    wstr_free(wstr);

    // new_from_limit
    wstr = wstr_new_from_limit(name, 5);
    ck_assert_int_eq(wcslen(L"hello"), wstr_size(wstr));
    ck_assert_wstr_eq(L"hello", wstr_c_wstr(wstr));
    wstr_free(wstr);

    // format
    wstr = wstr_format(L"%d-%d", 2, 3);
    ck_assert_int_eq(wcslen(L"2-3"), wstr_size(wstr));
    ck_assert_wstr_eq(L"2-3", wstr_c_wstr(wstr));
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_size) {
    wstr_t *wstr = wstr_new_from(L"hello");
    ck_assert_int_eq(5, wstr_size(wstr)); //字节尺寸 
    ck_assert_int_eq(5, wstr_size(wstr)); //字符尺寸，对于ascii字符，两者值是相同的
    wstr_free(wstr);

    // 对于unicode字符
    wstr = wstr_new_from(L"hello中国你好");
    ck_assert_int_eq(9, wstr_size(wstr)); //获取肉眼针对读到的字符长度
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_append) {
    wstr_t *wstr = wstr_new_from(L"hello");

    ck_assert_wstr_eq(L"hello", wstr_c_wstr(wstr));

    wstr_append(wstr, L"world");
    ck_assert_wstr_eq(L"helloworld", wstr_c_wstr(wstr));

    wstr_append(wstr, L"中国你好");
    ck_assert_wstr_eq(L"helloworld中国你好", wstr_c_wstr(wstr));

    wstr_append(wstr, L"I am very happy");
    ck_assert_wstr_eq(L"helloworld中国你好I am very happy", wstr_c_wstr(wstr));

    wstr_append(wstr, L"kdjkjskdjkdjsdkdjsdjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkksjdksjskeu");
    ck_assert_wstr_eq(L"helloworld中国你好I am very happykdjkjskdjkdjsdkdjsdjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkksjdksjskeu", wstr_c_wstr(wstr));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_at) {
    wstr_t *wstr = wstr_new_from(L"hello中国，你好，哈哈kk");
    wchar_t ch;

    ck_assert_int_eq(L'h',  wstr_at(wstr, 0));
    ck_assert_int_eq(L'o',  wstr_at(wstr, 4));
    ck_assert_int_eq((wchar_t)-1, wstr_at(wstr, 100)); //对于不存在的索引at_byte返回-1

    ch = wstr_at(wstr, 5);
    ck_assert_int_eq(L'中', ch);
    ck_assert_int_eq((wchar_t)-1, wstr_at(wstr, -1));
    ck_assert_int_eq((wchar_t)-1, wstr_at(wstr, 100));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_sub) {
    wstr_t *wstr = wstr_new_from(L"hello中国world");
    wstr_t *sub, *sub2;
    
    sub = wstr_sub_wstr(wstr, 0, 5);
    ck_assert_wstr_eq(L"hello", wstr_c_wstr(sub));
    wstr_free(sub);

    sub = wstr_sub_wstr(wstr, 1, 5);
    ck_assert_wstr_eq(L"ello中", wstr_c_wstr(sub));
    wstr_free(sub);

    // 对于偏移小于0， 则就按照0处理
    sub = wstr_sub_wstr(wstr, -1, 5);
    ck_assert_wstr_eq(L"hello", wstr_c_wstr(sub));
    wstr_free(sub);

    sub = wstr_sub_wstr(wstr, 5, 2);
    ck_assert_wstr_eq(L"中国", wstr_c_wstr(sub));
    wstr_free(sub);

    // 对于长度大于总长度，则按照最大长度进行处理
    sub = wstr_sub_wstr(wstr, 5, 100);
    ck_assert_wstr_eq(L"中国world", wstr_c_wstr(sub));
    wstr_free(sub);

    sub = wstr_new_from(L"hello world");

    sub2 = wstr_sub_wstr(sub, -1, 2);
    ck_assert_wstr_eq(L"he", wstr_c_wstr(sub2));
    wstr_free(sub2);

    sub2 = wstr_sub_wstr(sub, 3, 100);
    ck_assert_wstr_eq(L"lo world", wstr_c_wstr(sub2));
    wstr_free(sub2);

    sub2 = wstr_sub_wstr(sub, 2, 4);
    ck_assert_wstr_eq(L"llo ", wstr_c_wstr(sub2));
    wstr_free(sub2);

    wstr_free(sub);

    wstr_free(wstr);

    // 测试无效偏移和长度情况
    wstr = wstr_new_from(L"我是中国人");

    // offset 无效
    sub = wstr_sub_wstr(wstr, -1, wstr_size(wstr));
    ck_assert_wstr_eq(wstr_c_wstr(wstr), wstr_c_wstr(sub));
    wstr_free(sub);

    sub = wstr_sub_wstr(wstr, wstr_size(wstr), wstr_size(wstr));
    ck_assert_wstr_eq(L"", wstr_c_wstr(sub));
    ck_assert_int_eq(0, wstr_size(sub));
    wstr_free(sub);
    
    // length 无效
    sub = wstr_sub_wstr(wstr, 0, -1);
    ck_assert_wstr_eq(L"", wstr_c_wstr(sub));
    ck_assert_int_eq(0, wstr_size(sub));
    wstr_free(sub);

    sub = wstr_sub_wstr(wstr, 2, 100);
    ck_assert_wstr_eq(L"中国人", wstr_c_wstr(sub));
    ck_assert_int_eq(3, wstr_size(sub));
    wstr_free(sub);

    // 两者都无效
    sub = wstr_sub_wstr(wstr, 200, 200);
    ck_assert_wstr_eq(L"", wstr_c_wstr(sub));
    ck_assert_int_eq(0, wstr_size(sub));
    wstr_free(sub);

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_trim) {
    wstr_t *wstr = wstr_new_from(L"  \thello xx \t     ");

    wstr_trim_left(wstr);
    ck_assert_int_eq(wcslen(L"hello xx \t     "), wstr_size(wstr));
    ck_assert_wstr_eq(L"hello xx \t     ", wstr_c_wstr(wstr));

    wstr_trim_right(wstr);
    ck_assert_wstr_eq(L"hello xx", wstr_c_wstr(wstr));
    ck_assert_int_eq(wcslen(L"hello xx"), wstr_size(wstr));

    wstr_free(wstr);

    wstr = wstr_new_from(L"  \thello xx \t \n ");
    wstr_trim(wstr);
    ck_assert_wstr_eq(L"hello xx", wstr_c_wstr(wstr));
    ck_assert_int_eq(wcslen(L"hello xx"), wstr_size(wstr));
    wstr_free(wstr);

    wstr = wstr_new_from(L"  "); //全部空白字符
    wstr_trim(wstr);
    ck_assert_int_eq(0, wstr_size(wstr));
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_cmp) {
    wstr_t *wstr = wstr_new_from(L"hello");

    ck_assert_int_eq(0, wstr_cmp(wstr, L"hello"));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_detect) {
    wstr_t *wstr = wstr_new_from(L"helloAa");
    wstr_t *wstr2 = wstr_new_from(L"hello 中文kdjkd");

    ck_assert(wstr_contains(wstr, L"llo"));
    ck_assert(wstr_contains(wstr, L"el"));
    ck_assert(wstr_contains(wstr2, L"中"));

    ck_assert(wstr_equals(wstr, L"helloAa"));
    ck_assert(!wstr_equals(wstr, L"helloaa"));

    ck_assert(wstr_equals_ignore_case(wstr, L"helloAa"));
    ck_assert(wstr_equals_ignore_case(wstr, L"helloaa"));

    ck_assert(!wstr_empty(wstr));
    ck_assert(wstr_starts_with(wstr, L"hel"));


    wstr_free(wstr2);
    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_index) {
    wstr_t *wstr = wstr_new_from(L"/usr/local/lib/pkgconfig/defer.pc");

    ck_assert_int_eq(4, wstr_index_of(wstr, 0, L"/l"));
    ck_assert_int_eq(15, wstr_index_of(wstr, 0, L"pkg"));
    ck_assert_int_eq(-1, wstr_index_of(wstr, 0, L"ksjsk"));

    ck_assert_int_eq(10, wstr_rindex_of(wstr, -1, L"/lib")); //11
    ck_assert_int_eq(-1, wstr_rindex_of(wstr, -1, L"kjskdjksj"));

    // 第二个参数是扫描的起始位置
    ck_assert_int_eq(24, wstr_rindex_of_wchar(wstr, -1, L'/'));
    ck_assert_int_eq(-1, wstr_rindex_of(wstr, -1, L"ksjsk"));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_case) {
    wstr_t *wstr = wstr_new_from(L"aAbcDeF");

    wstr_to_lowercase(wstr);
    ck_assert_wstr_eq(L"aabcdef", wstr_c_wstr(wstr));

    wstr_to_uppercase(wstr);
    ck_assert_wstr_eq(L"AABCDEF", wstr_c_wstr(wstr));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_replace) {
    wstr_t * wstr = wstr_new_from(L"/usr/local/lib/pkgconfig/defer.pc");

    wstr_replace_wchar(wstr, L'/', L':');
    ck_assert_wstr_eq(L":usr:local:lib:pkgconfig:defer.pc", wstr_c_wstr(wstr));

    wstr_replace_wchar(wstr, L':', L'/');
    ck_assert_wstr_eq(L"/usr/local/lib/pkgconfig/defer.pc", wstr_c_wstr(wstr));

    // 替换长度是相同的情况
    wstr_replace_first(wstr, 0, L"/l", L"/A");
    ck_assert_wstr_eq(L"/usr/Aocal/lib/pkgconfig/defer.pc", wstr_c_wstr(wstr));
    wstr_replace_first(wstr, 0, L"/A", L"/l");
    ck_assert_wstr_eq(L"/usr/local/lib/pkgconfig/defer.pc", wstr_c_wstr(wstr));

    // 替换老的比新的长
    wstr_replace_first(wstr, 0, L"/loc", L"/A");
    ck_assert_wstr_eq(L"/usr/Aal/lib/pkgconfig/defer.pc", wstr_c_wstr(wstr));
    
    // 替换老的比新的短
    wstr_replace_first(wstr, 0, L"/lib", L"/lib2");
    ck_assert_wstr_eq(L"/usr/Aal/lib2/pkgconfig/defer.pc", wstr_c_wstr(wstr));

    wstr_replace_first(wstr, 0, L"/pkg", L"/package");
    ck_assert_wstr_eq(L"/usr/Aal/lib2/packageconfig/defer.pc", wstr_c_wstr(wstr));
    wstr_free(wstr);

    wstr = wstr_new_from(L"/usr/local/lib/pkgconfig/defer.pc");
    wstr_replace(wstr, L"/l", L"/usr");
    ck_assert_wstr_eq(L"/usr/usrocal/usrib/pkgconfig/defer.pc", wstr_c_wstr(wstr));

    wstr_replace(wstr, L"/usr", L"/aadefer");
    ck_assert_wstr_eq(L"/aadefer/aadeferocal/aadeferib/pkgconfig/defer.pc", wstr_c_wstr(wstr));


    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_matches) {
    wstr_t *wstr = wstr_new_from(L"123");

    // 进行正则表达式校验
    ck_assert(wstr_matches(wstr, L"^[0-9]+$"));

    wstr_assign(wstr, L"abcde");
    ck_assert(wstr_matches(wstr, L"^[a-zA-Z]+$"));

    wstr_assign(wstr, L" \n\t  ");
    ck_assert(wstr_matches(wstr, L"[[:space:]]+")); //不支持\s, 支持posix扩展

    wstr_assign(wstr, L"hello world xix haha");
    ck_assert(wstr_matches(wstr, L"[[:space:]]+"));

    wstr_assign(wstr, L"中国");
    ck_assert(wstr_matches(wstr, L"[[:print:]]+")); 
    ck_assert(wstr_matches(wstr, L"[[:alnum:]]+"));//校验通过，中文居然是属于字母和数字

    wstr_assign(wstr, L"  ");
    ck_assert(!wstr_matches(wstr, L"[[:alnum:]]+"));


    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_assign) {
    wstr_t *wstr = wstr_new();

    wstr_assign(wstr, L"hello"); //内部有缓存不会每次都扩容
    ck_assert_wstr_eq(L"hello", wstr_c_wstr(wstr));

    wstr_assign(wstr, L"ksjdkdjkdjdkdjkdjdkddjkdjdkdj");
    ck_assert_wstr_eq(L"ksjdkdjkdjdkdjkdjdkddjkdjdkdj", wstr_c_wstr(wstr));

    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_split) {
    wstr_t *wstr = wstr_new_from(L"hello  world xixi haha");
    VEC *vec = wstr_split(wstr, L"[[:space:]]+");
    ck_assert_int_eq(4, vec_size(vec));
    ck_assert_wstr_eq(L"hello", wstr_c_wstr(wstr_vec_get(vec, 0)));
    ck_assert_wstr_eq(L"world", wstr_c_wstr(wstr_vec_get(vec, 1)));
    ck_assert_wstr_eq(L"xixi", wstr_c_wstr(wstr_vec_get(vec, 2)));
    ck_assert_wstr_eq(L"haha", wstr_c_wstr(wstr_vec_get(vec, 3)));

    // 释放内存时候有点问题
    vec_free(vec);
    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert) {
    wstr_t *wstr = wstr_new_from(L"hello");

    // 在开始位置插入
    wstr_insert(wstr, 0, L"ab");
    ck_assert_wstr_eq(L"abhello", wstr_c_wstr(wstr));

    // 在尾部插入
    wstr_insert(wstr, wstr_size(wstr), L"kk");
    ck_assert_wstr_eq(L"abhellokk", wstr_c_wstr(wstr));

    // 在中间插入
    wstr_insert(wstr, 3, L"jj");
    ck_assert_wstr_eq(L"abhjjellokk", wstr_c_wstr(wstr));

    // 空白字符
    wstr_insert(wstr, 3, L"");
    ck_assert_wstr_eq(L"abhjjellokk", wstr_c_wstr(wstr));

    // 处理不合法的pos
    wstr_insert(wstr, -1, L"zz"); //负值
    ck_assert_wstr_eq(L"zzabhjjellokk", wstr_c_wstr(wstr));

    wstr_insert(wstr, 100, L"qq");
    ck_assert_wstr_eq(L"zzabhjjellokkqq", wstr_c_wstr(wstr));

    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_sub_buf) {
    wstr_t *wstr = wstr_new_from(L"hello");
    wchar_t long_buf[200];
    int err = 0;
    int nread;

                
    // 足够长(缓冲区长度只有大于 子字符串的长度，才能够完整的获取整个字符串)
    nread = wstr_sub_wstr_buf(wstr, 1, 2, long_buf, 200, &err);
    ck_assert_int_eq(wcslen(L"el"), nread);
    ck_assert_wstr_eq(L"el", long_buf);
    ck_assert(0 == err);

    // 短
    nread = wstr_sub_wstr_buf(wstr, 1, 3, long_buf, 2, &err);
    ck_assert_int_eq(wcslen(L"e"), nread);
    ck_assert_wstr_eq(L"e", long_buf);
    ck_assert_int_eq(ENOMEM, err);

    // 相等
    nread = wstr_sub_wstr_buf(wstr, 1, 3, long_buf, 3, &err);
    ck_assert_int_eq(wcslen(L"el"), nread);
    ck_assert_wstr_eq(L"el", long_buf);
    ck_assert(ENOMEM == err);

    // buf为NULL, 返回包含'\0'的缓冲区容量
    nread = wstr_sub_wstr_buf(wstr, 1, 3, NULL, 3, &err);
    ck_assert_int_eq(nread, 3 + 1);
    ck_assert(0 == err);

    nread = wstr_sub_wstr_buf(wstr, wstr_size(wstr) - 2, 100, NULL, 3, &err);
    ck_assert_int_eq(nread, 2 + 1);
    ck_assert(0 == err);

    nread = wstr_sub_wstr_buf(wstr, wstr_size(wstr) - 2, 100, long_buf, 3, &err);
    ck_assert_int_eq(nread, 2);
    ck_assert_wstr_eq(L"lo", long_buf);
    ck_assert(0 == err);

    wstr_free(wstr);

    ck_assert_no_leak();
}
END_TEST

// 内部使用split,对于复杂的就可以使用split来进行分割
START_TEST(test_split_lines) {
    wstr_t *wstr = wstr_new_from(L"hello world\n\nwelcome, lady and gentleman");
    VEC *lines = wstr_split_lines(wstr);

    ck_assert_int_eq(3, vec_size(lines));
    ck_assert_wstr_eq(L"hello world", wstr_c_wstr(wstr_vec_get(lines, 0)));
    ck_assert_wstr_eq(L"", wstr_c_wstr(wstr_vec_get(lines, 1)));
    ck_assert_wstr_eq(L"welcome, lady and gentleman", wstr_c_wstr(wstr_vec_get(lines, 2)));

    vec_free(lines);
    wstr_free(wstr);

    wstr = wstr_new_from(L"hello world\r\n\r\nwelcome, lady and gentleman");
    lines = wstr_split(wstr, L"[\r\n]+"); //不但可以分行，而且还去除空白的行

    ck_assert_int_eq(2, vec_size(lines));
    ck_assert_wstr_eq(L"hello world", wstr_c_wstr(wstr_vec_get(lines, 0)));
    ck_assert_wstr_eq(L"welcome, lady and gentleman", wstr_c_wstr(wstr_vec_get(lines, 1)));

    vec_free(lines);
    wstr_free(wstr);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_resize) {
    wstr_t *str = wstr_new_from(L"hello world");
    int old_capacity = wstr_capacity(str);
    int old_size = wstr_size(str);

    // 元素数目减少
    wstr_resize(str, 3, L'a');
    ck_assert_wstr_eq(L"hel", wstr_c_wstr(str));
    ck_assert_int_eq(3, wstr_size(str));
    ck_assert_int_eq(old_capacity, wstr_capacity(str));

    // 元素数目大于原来的容量
    wstr_resize(str, old_size, L'a');
    ck_assert_wstr_eq(L"helaaaaaaaa", wstr_c_wstr(str));
    ck_assert_int_eq(old_size, wstr_size(str));

    for (int i = 3; i < old_size; i++) {
        ck_assert_int_eq(L'a', *(wstr_c_wstr(str) + i));
    }

    wstr_free(str);
}
END_TEST

START_DEFINE_SUITE(wstr)
    TEST(test_new_free)
    TEST(test_size)
    TEST(test_append)
    TEST(test_at)
    TEST(test_sub)
    TEST(test_sub_buf)
    TEST(test_trim)
    TEST(test_cmp)
    TEST(test_detect)
    TEST(test_index)
    TEST(test_case)
    TEST(test_replace)
    TEST(test_matches)
    TEST(test_assign)
    TEST(test_split)
    TEST(test_insert)
    TEST(test_split_lines)
    TEST(test_resize)
END_DEFINE_SUITE()
