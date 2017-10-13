/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 12 19:28:10
*/
#include <check_util.h>
#include <string.h>
#include <assert.h>
#include "test_common.h"
#include <regex.h>
#include "str.h"

START_TEST(test_new_free) {
    // 不带参数的new
    str_t *str = str_new();
    ck_assert_int_eq(str_byte_size(str), 0);
    ck_assert_int_eq(str_capacity(str), 1); //有个'\0', 所以容量是1
    ck_assert(str_at_byte(str, 0) == '\0');
    str_free(str);

    // new_from_str
    char name[] = "hello world";
    str = str_new_from(name);
    ck_assert_int_eq(strlen(name), str_byte_size(str));
    ck_assert_str_eq(name, str_c_str(str));
    str_free(str);

    // new_from_limit
    str = str_new_from_limit(name, 5);
    ck_assert_int_eq(strlen("hello"), str_byte_size(str));
    ck_assert_str_eq("hello", str_c_str(str));
    str_free(str);

    // format
    str = str_format("%d-%d", 2, 3);
    ck_assert_int_eq(strlen("2-3"), str_byte_size(str));
    ck_assert_str_eq("2-3", str_c_str(str));
    str_free(str);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_size) {
    str_t *str = str_new_from("hello");
    ck_assert_int_eq(5, str_byte_size(str)); //字节尺寸 
    ck_assert_int_eq(5, str_char_size(str)); //字符尺寸，对于ascii字符，两者值是相同的
    str_free(str);

    // 对于unicode字符
    str = str_new_from("hello中国你好");
    ck_assert_int_eq(9, str_char_size(str)); //获取肉眼针对读到的字符长度
    ck_assert(str_byte_size(str) > 9); //原始字节的数目，即strlen(char*)
    str_free(str);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_append) {
    str_t *str = str_new_from("hello");

    ck_assert_str_eq("hello", str_c_str(str));

    str_append(str, "world");
    ck_assert_str_eq("helloworld", str_c_str(str));

    str_append(str, "中国你好");
    ck_assert_str_eq("helloworld中国你好", str_c_str(str));

    str_append(str, "I am very happy");
    ck_assert_str_eq("helloworld中国你好I am very happy", str_c_str(str));

    str_append(str, "kdjkjskdjkdjsdkdjsdjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkksjdksjskeu");
    ck_assert_str_eq("helloworld中国你好I am very happykdjkjskdjkdjsdkdjsdjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkjkksjdksjskeu", str_c_str(str));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_at) {
    str_t *str = str_new_from("hello中国，你好，哈哈kk");
    str_t *ch;

    ck_assert_int_eq((int)'h',  str_at_byte(str, 0));
    ck_assert_int_eq((int)'o',  str_at_byte(str, 4));
    ck_assert_int_eq(-1, str_at_byte(str, 100)); //对于不存在的索引at_byte返回-1

    ch = str_at(str, 5);
    ck_assert_str_eq("中", str_c_str(ch));
    str_free(ch);
    ck_assert(NULL == str_at(str, -1));
    ck_assert(NULL == str_at(str, 100));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_sub) {
    str_t *str = str_new_from("hello中国world");
    str_t *sub, *sub2;
    
    sub = str_sub(str, 0, 5);
    ck_assert_str_eq("hello", str_c_str(sub));
    str_free(sub);

    sub = str_sub(str, 1, 5);
    ck_assert_str_eq("ello中", str_c_str(sub));
    str_free(sub);

    // 对于偏移小于0， 则就按照0处理
    sub = str_sub(str, -1, 5);
    ck_assert_str_eq("hello", str_c_str(sub));
    str_free(sub);

    sub = str_sub(str, 5, 2);
    ck_assert_str_eq("中国", str_c_str(sub));
    str_free(sub);

    // 对于长度大于总长度，则按照最大长度进行处理
    sub = str_sub(str, 5, 100);
    ck_assert_str_eq("中国world", str_c_str(sub));
    str_free(sub);

    sub = str_new_from("hello world");

    sub2 = str_sub_str(sub, -1, 2);
    ck_assert_str_eq("he", str_c_str(sub2));
    str_free(sub2);

    sub2 = str_sub_str(sub, 3, 100);
    ck_assert_str_eq("lo world", str_c_str(sub2));
    str_free(sub2);

    sub2 = str_sub_str(sub, 2, 4);
    ck_assert_str_eq("llo ", str_c_str(sub2));
    str_free(sub2);

    str_free(sub);

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_trim) {
    str_t *str = str_new_from("  \thello xx \t     ");

    str_trim_left(str);
    ck_assert_int_eq(strlen("hello xx \t     "), str_byte_size(str));
    ck_assert_str_eq("hello xx \t     ", str_c_str(str));

    str_trim_right(str);
    ck_assert_str_eq("hello xx", str_c_str(str));
    ck_assert_int_eq(strlen("hello xx"), str_byte_size(str));

    str_free(str);

    str = str_new_from("  \thello xx \t \n ");
    str_trim(str);
    ck_assert_str_eq("hello xx", str_c_str(str));
    ck_assert_int_eq(strlen("hello xx"), str_byte_size(str));
    str_free(str);

    str = str_new_from("  "); //全部空白字符
    str_trim(str);
    ck_assert_int_eq(0, str_byte_size(str));
    str_free(str);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_cmp) {
    str_t *str = str_new_from("hello");

    ck_assert_int_eq(0, str_cmp(str, "hello"));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_detect) {
    str_t *str = str_new_from("helloAa");
    str_t *str2 = str_new_from("hello 中文kdjkd");

    ck_assert(str_contains(str, "llo"));
    ck_assert(str_contains(str, "el"));
    ck_assert(str_contains(str2, "中"));

    ck_assert(str_equals(str, "helloAa"));
    ck_assert(!str_equals(str, "helloaa"));

    ck_assert(str_equals_ignore_case(str, "helloAa"));
    ck_assert(str_equals_ignore_case(str, "helloaa"));

    ck_assert(!str_empty(str));
    ck_assert(str_starts_with(str, "hel"));


    str_free(str2);
    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_index) {
    str_t *str = str_new_from("/usr/local/lib/pkgconfig/defer.pc");

    ck_assert_int_eq(4, str_index_of(str, 0, "/l"));
    ck_assert_int_eq(15, str_index_of(str, 0, "pkg"));
    ck_assert_int_eq(-1, str_index_of(str, 0, "ksjsk"));

    ck_assert_int_eq(10, str_rindex_of(str, -1, "/lib")); //11
    ck_assert_int_eq(-1, str_rindex_of(str, -1, "kjskdjksj"));

    // 第二个参数是扫描的起始位置
    ck_assert_int_eq(24, str_rindex_of_char(str, -1, '/'));
    ck_assert_int_eq(-1, str_rindex_of(str, -1, "ksjsk"));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_case) {
    str_t *str = str_new_from("aAbcDeF");

    str_to_lowercase(str);
    ck_assert_str_eq("aabcdef", str_c_str(str));

    str_to_uppercase(str);
    ck_assert_str_eq("AABCDEF", str_c_str(str));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_replace) {
    str_t * str = str_new_from("/usr/local/lib/pkgconfig/defer.pc");

    str_replace_byte(str, '/', ':');
    ck_assert_str_eq(":usr:local:lib:pkgconfig:defer.pc", str_c_str(str));

    str_replace_byte(str, ':', '/');
    ck_assert_str_eq("/usr/local/lib/pkgconfig/defer.pc", str_c_str(str));

    // 替换长度是相同的情况
    str_replace_first(str, 0, "/l", "/A");
    ck_assert_str_eq("/usr/Aocal/lib/pkgconfig/defer.pc", str_c_str(str));
    str_replace_first(str, 0, "/A", "/l");
    ck_assert_str_eq("/usr/local/lib/pkgconfig/defer.pc", str_c_str(str));

    // 替换老的比新的长
    str_replace_first(str, 0, "/loc", "/A");
    ck_assert_str_eq("/usr/Aal/lib/pkgconfig/defer.pc", str_c_str(str));
    
    // 替换老的比新的短
    str_replace_first(str, 0, "/lib", "/lib2");
    ck_assert_str_eq("/usr/Aal/lib2/pkgconfig/defer.pc", str_c_str(str));

    str_replace_first(str, 0, "/pkg", "/package");
    ck_assert_str_eq("/usr/Aal/lib2/packageconfig/defer.pc", str_c_str(str));
    str_free(str);

    str = str_new_from("/usr/local/lib/pkgconfig/defer.pc");
    str_replace(str, "/l", "/usr");
    ck_assert_str_eq("/usr/usrocal/usrib/pkgconfig/defer.pc", str_c_str(str));

    str_replace(str, "/usr", "/aadefer");
    ck_assert_str_eq("/aadefer/aadeferocal/aadeferib/pkgconfig/defer.pc", str_c_str(str));


    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_matches) {
    str_t *str = str_new_from("123");

    // 进行正则表达式校验
    ck_assert(str_matches(str, "^[0-9]+$"));

    str_assign(str, "abcde");
    ck_assert(str_matches(str, "^[a-zA-Z]+$"));

    str_assign(str, " \n\t  ");
    ck_assert(str_matches(str, "[[:space:]]+")); //不支持\s, 支持posix扩展

    str_assign(str, "hello world xix haha");
    ck_assert(str_matches(str, "[[:space:]]+"));

    str_assign(str, "中国");
    ck_assert(str_matches(str, "[[:print:]]+")); 
    ck_assert(str_matches(str, "[[:alnum:]]+"));//校验通过，中文居然是属于字母和数字

    str_assign(str, "  ");
    ck_assert(!str_matches(str, "[[:alnum:]]+"));


    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_assign) {
    str_t *str = str_new();

    str_assign(str, "hello"); //内部有缓存不会每次都扩容
    ck_assert_str_eq("hello", str_c_str(str));

    str_assign(str, "ksjdkdjkdjdkdjkdjdkddjkdjdkdj");
    ck_assert_str_eq("ksjdkdjkdjdkdjkdjdkddjkdjdkdj", str_c_str(str));

    str_free(str);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_split) {
    str_t *str = str_new_from("hello  world xixi haha");
    VEC *vec = str_split(str, "[[:space:]]+");
    ck_assert_int_eq(4, vec_size(vec));
    ck_assert_str_eq("hello", str_c_str(str_vec_get(vec, 0)));
    ck_assert_str_eq("world", str_c_str(str_vec_get(vec, 1)));
    ck_assert_str_eq("xixi", str_c_str(str_vec_get(vec, 2)));
    ck_assert_str_eq("haha", str_c_str(str_vec_get(vec, 3)));

    // 释放内存时候有点问题
    vec_free(vec);
    str_free(str);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert) {
    str_t *str = str_new_from("hello");

    // 在开始位置插入
    str_insert(str, 0, "ab");
    ck_assert_str_eq("abhello", str_c_str(str));

    // 在尾部插入
    str_insert(str, str_byte_size(str), "kk");
    ck_assert_str_eq("abhellokk", str_c_str(str));

    // 在中间插入
    str_insert(str, 3, "jj");
    ck_assert_str_eq("abhjjellokk", str_c_str(str));

    // 空白字符
    str_insert(str, 3, "");
    ck_assert_str_eq("abhjjellokk", str_c_str(str));

    // 处理不合法的pos
    str_insert(str, -1, "zz"); //负值
    ck_assert_str_eq("zzabhjjellokk", str_c_str(str));

    str_insert(str, 100, "qq");
    ck_assert_str_eq("zzabhjjellokkqq", str_c_str(str));

    str_free(str);
}
END_TEST

START_DEFINE_SUITE(str)
    TEST(test_new_free)
    TEST(test_size)
    TEST(test_append)
    TEST(test_at)
    TEST(test_sub)
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
END_DEFINE_SUITE()
