/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 15 08:55:38
*/
#include <check_util.h>
#include "test_common.h"
#include "utf8_str.h"
#include "vec.h"

START_TEST(test_new_from) {
    // 正面测试
    u8_str_t *u8 = u8_str_new_from((byte_t*)"hello, \xd6\xd0\xb9\xfa", strlen("hello, \xd6\xd0\xb9\xfa"),
                "GB18030");

    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    // 负面测试
    u8 = u8_str_new_from((byte_t*)"", 0, "GB18030");
    ck_assert_str_eq("", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    // 测试是否去除不完整字符
    u8 = u8_str_new_from((byte_t*)"hello, \xd6\xd0\xb9", strlen("hello, \xd6\xd0\xb9"),
                "GB18030");

    ck_assert_str_eq("hello, \xe4\xb8\xad", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    // 测试转换ascii, 
    u8 = u8_str_new_from((byte_t*)"hello, \xd6\xd0\xb9\xfa, world", strlen("hello, \xd6\xd0\xb9\xfa, world"),
                "ASCII");

    // 主动忽略掉非ascii字符
    ck_assert_str_eq("hello, , world", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    ////////////////////////////////////////////////////////////////////////////////////////
    
    // 测试各种遍历方法
    u8 = u8_str_new_from_gbk("hello, \xd6\xd0\xb9");
    ck_assert_str_eq("hello, \xe4\xb8\xad", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    u8 = u8_str_new_from_ascii("hello, \xd6\xd0\xb9\xfa, world");
    ck_assert_str_eq("hello, , world", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    u8 = u8_str_new_from_u16(L"hello, 中国");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b\xbd");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    // u8测试不完整的
    u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b");
    ck_assert_str_eq("hello, \xe4\xb8\xad", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    // u8测试无效的
    u8 = u8_str_new_from_u8("hello, \xe4\xb8\xe5\x9b\xbd");
    ck_assert_str_eq("hello, \xe5\x9b\xbd", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_size_byte_count) {
    //hello, 中国
    u8_str_t *u8 = u8_str_new_from((byte_t*)"hello, \xd6\xd0\xb9\xfa", strlen("hello, \xd6\xd0\xb9\xfa"),
                "GB18030");

    ck_assert_int_eq(9, u8_str_size(u8));
    ck_assert_int_eq(13, u8_str_byte_count(u8));
    u8_str_free(u8);

    //hello, 我是飞天虎，你是谁？
    u8 = u8_str_new_from((byte_t*)"hello, \xe6\x88\x91\xe6\x98\xaf\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xef\xbc\x8c\xe4\xbd\xa0\xe6\x98\xaf\xe8\xb0\x81\xef\xbc\x9f",
            strlen("hello, \xe6\x88\x91\xe6\x98\xaf\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xef\xbc\x8c\xe4\xbd\xa0\xe6\x98\xaf\xe8\xb0\x81\xef\xbc\x9f"),
                "UTF-8");

    ck_assert_int_eq(17, u8_str_size(u8));
    ck_assert_int_eq(strlen("hello, 我是飞天虎，你是谁？"), u8_str_byte_count(u8));
    u8_str_free(u8);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_append) {
    u8_str_t *u8 = u8_str_new_from_ascii("hello, ");

    // 中国
    u8_str_append(u8, (byte_t*)"\xe4\xb8\xad\xe5\x9b\xbd", strlen("\xe4\xb8\xad\xe5\x9b\xbd"), "UTF-8");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));

    // 飞天虎
    u8_str_append(u8, (byte_t*)"\xb7\xc9\xcc\xec\xbb\xa2", strlen("\xb7\xc9\xcc\xec\xbb\xa2"), "GB18030");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e", u8_str_c_u8_str(u8));

    // 大王
    u8_str_append(u8, (byte_t*)L"大王", wcslen(L"大王") * sizeof(wchar_t), "UTF-16LE");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xe5\xa4\xa7\xe7\x8e\x8b", u8_str_c_u8_str(u8));

    // ff
    u8_str_append(u8, (byte_t*)"ff", strlen("ff"), "ASCII");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xe5\xa4\xa7\xe7\x8e\x8b\x66\x66", u8_str_c_u8_str(u8));

    u8_str_free(u8);
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    //测试便利函数
    //

    u8 = u8_str_new_from_ascii("hello, ");

    // 中国
    u8_str_append_u8(u8, "\xe4\xb8\xad\xe5\x9b\xbd");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8));

    // 飞天虎
    u8_str_append_gbk(u8, "\xb7\xc9\xcc\xec\xbb\xa2");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e", u8_str_c_u8_str(u8));

    // 大王
    u8_str_append_u16(u8, L"大王");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xe5\xa4\xa7\xe7\x8e\x8b", u8_str_c_u8_str(u8));

    // ff
    u8_str_append_ascii(u8, "ff");
    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe9\xa3\x9e\xe5\xa4\xa9\xe8\x99\x8e\xe5\xa4\xa7\xe7\x8e\x8b\x66\x66", u8_str_c_u8_str(u8));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_pos_to_pos) {
    // hello, 中国你好
    u8_str_t *u8 = u8_str_new_from_u8("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe5\xa5\xbd");

    //从u8位置转换为字节位置
    ck_assert_int_eq(0, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 0));
    ck_assert_int_eq(3, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 3));
    ck_assert_int_eq(7, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 7));
    ck_assert_int_eq(10, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 8));
    ck_assert_int_eq(13, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 9));
    ck_assert_int_eq(16, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 10));
    ck_assert_int_eq(19, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 11)); //测试的最后的位置'\0'

    ck_assert_int_eq(19, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 15)); //越界，返回'\0'的位置

    ck_assert_int_eq(0, u8_str_u8_pos_to_raw_pos(u8, 0));
    ck_assert_int_eq(3, u8_str_u8_pos_to_raw_pos(u8, 3));
    ck_assert_int_eq(7, u8_str_u8_pos_to_raw_pos(u8, 7));
    ck_assert_int_eq(10, u8_str_u8_pos_to_raw_pos(u8, 8));
    ck_assert_int_eq(13, u8_str_u8_pos_to_raw_pos(u8, 9));
    ck_assert_int_eq(16, u8_str_u8_pos_to_raw_pos(u8, 10));
    ck_assert_int_eq(19, u8_str_u8_pos_to_raw_pos(u8, 11));

    ck_assert_int_eq(19, u8_raw_u8_pos_to_raw_pos(u8_str_c_u8_str(u8), 15)); //越界，返回'\0'的位置


    // 从字节位置转化为u8位置
    ck_assert_int_eq(0, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 0));
    ck_assert_int_eq(3, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 3));
    ck_assert_int_eq(7, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 7));
    ck_assert_int_eq(8, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 10));
    ck_assert_int_eq(9, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 13));
    ck_assert_int_eq(11, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 19));

    // 测试月结的情况
    ck_assert_int_eq(11, u8_raw_raw_pos_to_u8_pos(u8_str_c_u8_str(u8), 23)); //越界最会定位到'\0'的位置

    ck_assert_int_eq(0, u8_str_raw_pos_to_u8_pos(u8, 0));
    ck_assert_int_eq(3, u8_str_raw_pos_to_u8_pos(u8, 3));
    ck_assert_int_eq(7, u8_str_raw_pos_to_u8_pos(u8, 7));
    ck_assert_int_eq(8, u8_str_raw_pos_to_u8_pos(u8, 10));
    ck_assert_int_eq(9, u8_str_raw_pos_to_u8_pos(u8, 13));
    ck_assert_int_eq(10, u8_str_raw_pos_to_u8_pos(u8, 16));
    ck_assert_int_eq(11, u8_str_raw_pos_to_u8_pos(u8, 19));
    ck_assert_int_eq(11, u8_str_raw_pos_to_u8_pos(u8, 23));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert) {
    u8_str_t *u8 = u8_str_new_from_u8("hello, 飞飞侠");

    // 在头部插入, 
    u8_str_insert_u8(u8, 0, "中国,");
    ck_assert_str_eq("中国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在中间位置插入
    u8_str_insert_u8(u8, 1, "一");
    ck_assert_str_eq("中一国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    u8_str_insert_u8(u8, 5, "二");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在末尾插入
    u8_str_insert_u8(u8, u8_str_size(u8), "哈哈");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈", u8_str_c_u8_str(u8));

    // 在超出末尾位置插入
    u8_str_insert_u8(u8, u8_str_size(u8) + 10, "hh");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈hh", u8_str_c_u8_str(u8));
    u8_str_free(u8);


    u8 = u8_str_new_from_u8("hello, 飞飞侠");

    // 在头部插入, 中国,
    u8_str_insert_gbk(u8, 0, "\xd6\xd0\xb9\xfa,");
    ck_assert_str_eq("中国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在中间位置插入, 一
    u8_str_insert_gbk(u8, 1, "\xd2\xbb");
    ck_assert_str_eq("中一国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    // 二
    u8_str_insert_gbk(u8, 5, "\xb6\xfe");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在末尾插入, 哈哈
    u8_str_insert_gbk(u8, u8_str_size(u8), "\xb9\xfe\xb9\xfe");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈", u8_str_c_u8_str(u8));

    // 在超出末尾位置插入
    u8_str_insert_gbk(u8, u8_str_size(u8) + 10, "hh");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈hh", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    u8 = u8_str_new_from_u8("hello, 飞飞侠");

    // 在头部插入, 中国,
    u8_str_insert_u16(u8, 0, L"中国,");
    ck_assert_str_eq("中国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在中间位置插入, 一
    u8_str_insert_u16(u8, 1, L"一");
    ck_assert_str_eq("中一国,hello, 飞飞侠", u8_str_c_u8_str(u8));

    // 二
    u8_str_insert_u16(u8, 5, L"二");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠", u8_str_c_u8_str(u8));

    // 在末尾插入, 哈哈
    u8_str_insert_u16(u8, u8_str_size(u8), L"哈哈");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈", u8_str_c_u8_str(u8));

    // 在超出末尾位置插入
    u8_str_insert_gbk(u8, u8_str_size(u8) + 10, "hh");
    ck_assert_str_eq("中一国,h二ello, 飞飞侠哈哈hh", u8_str_c_u8_str(u8));
    u8_str_free(u8);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_assign) {
    u8_str_t *u8 = u8_str_new_from_ascii("");
    ck_assert_int_eq(0, u8_str_size(u8));

    //测试assign_u16
    wchar_t* datas[] = {L"hello, 中国", L"kkjj", L"", L"SKJD"};
    char* u8datas[] = {"hello, \xe4\xb8\xad\xe5\x9b\xbd", "kkjj", "", "SKJD"};
    for (size_t i = 0; i < ARRAY_SIZE(datas, wchar_t*); i++) {
        u8_str_assign_u16(u8, datas[i]);
        ck_assert_str_eq(u8datas[i], u8_str_c_u8_str(u8));
    }

    //测试assign_u8
    for (size_t i = 0; i < ARRAY_SIZE(datas, char*); i++) {
        u8_str_assign_u8(u8, u8datas[i]);
        ck_assert_str_eq(u8datas[i], u8_str_c_u8_str(u8));
    }

    //测试assign_gbk
    char* gbk_datas[] = {"hello, \xd6\xd0\xb9\xfa", "kkjj", "", "SKJD"};
    for (size_t i = 0; i < ARRAY_SIZE(gbk_datas, char*); i++) {
        u8_str_assign_gbk(u8, gbk_datas[i]);
        ck_assert_int_eq(strlen(u8datas[i]), u8_str_byte_count(u8));
    }

    //测试ssign_asci
    char* asc_datas[] = {"hello, \xd6\xd0\xb9\xfa", "kkjj", "", "SKJD"};
    int expect[] = {7, strlen("kkjj"), 0, strlen("SKJD")};
    for (size_t i = 0; i < ARRAY_SIZE(asc_datas, char*); i++) {
        u8_str_assign_ascii(u8, asc_datas[i]);
        ck_assert_int_eq(expect[i], u8_str_byte_count(u8));
    }
    
    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_at) {
    u8_str_t *u8 = u8_str_new_from_u16(L"");
    u8_char_t u8c;

    // 在指定范围内
    u8_str_at(u8, 0, &u8c);
    ck_assert_int_eq('\0', *u8c.beg);
    ck_assert_int_eq('\0', *u8c.end);

    // 当超出范围
    u8_str_at(u8, 1, &u8c);
    ck_assert_int_eq('\0', *u8c.beg);
    ck_assert_int_eq('\0', *u8c.end);

    // 正常测试
    u8_str_assign_u8(u8, "hj");

    u8_str_at(u8, 0, &u8c);
    ck_assert_int_eq('h', (char)*u8c.beg);
    ck_assert_int_eq('j', (char)*u8c.end);

    u8_str_at(u8, 1, &u8c);
    ck_assert_int_eq('j', (char)*u8c.beg);
    ck_assert_int_eq('\0', (char)*u8c.end);

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_sub) {
    u8_str_t *u8 = u8_str_new_from_ascii("hello");
    u8_str_t *sub = NULL;
    
    // 正常测试
    sub = u8_str_sub(u8, 0, 2);
    ck_assert_str_eq("he", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    sub = u8_str_sub(u8, 0, 4);
    ck_assert_str_eq("hell", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    // length 过长
    sub = u8_str_sub(u8, 0, 100);
    ck_assert_str_eq(u8_str_c_u8_str(u8), u8_str_c_u8_str(sub));
    u8_str_free(sub);

    // pos 超过范围
    sub = u8_str_sub(u8, 100, 1);
    ck_assert_str_eq("", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    u8_str_free(u8);

    u8 = u8_str_new_from_u16(L"hello, 中国你好");

    sub = u8_str_sub(u8, 7, 1);
    ck_assert_str_eq("\xe4\xb8\xad", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    sub = u8_str_sub(u8, 7, 2);
    ck_assert_str_eq("\xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    sub = u8_str_sub(u8, 7, 100);
    ck_assert_str_eq("\xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe5\xa5\xbd", u8_str_c_u8_str(sub));
    u8_str_free(sub);

    ck_assert_str_eq("hello, \xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_trim) {
    char *datas[] = {" hello", "\n hello\xe4\xb8\xad\xe5\x9b\xbd world ", "   world\t\n "};
    char *expect_trim_left[] = {"hello", "hello\xe4\xb8\xad\xe5\x9b\xbd world ", "world\t\n "};
    char *expect_trim_right[] = {" hello", "\n hello\xe4\xb8\xad\xe5\x9b\xbd world", "   world"};
    char *expect_trim[] = {"hello", "hello\xe4\xb8\xad\xe5\x9b\xbd world", "world"};
    u8_str_t *u8 = u8_str_new_from_u8("");

    for (size_t i = 0; i < ARRAY_SIZE(datas, char*); i++) {
        u8_str_assign_u8(u8, datas[i]);
        u8_str_trim_left(u8);
        ck_assert_str_eq(expect_trim_left[i], u8_str_c_u8_str(u8));

        u8_str_assign_u8(u8, datas[i]);
        u8_str_trim_right(u8);
        ck_assert_str_eq(expect_trim_right[i], u8_str_c_u8_str(u8));

        u8_str_assign_u8(u8, datas[i]);
        u8_str_trim(u8);
        ck_assert_str_eq(expect_trim[i], u8_str_c_u8_str(u8));
    }

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_split) {
    u8_str_t *u8 = u8_str_new_from_u16(L"我是::中国:人:我:");
    VEC *vec = u8_str_split_ascii(u8, "[:]");

    ck_assert_int_eq(6, vec_size(vec));
    ck_assert_str_eq("\xe6\x88\x91\xe6\x98\xaf", u8_str_c_u8_str(u8_str_vec_get(vec, 0)));
    ck_assert_str_eq("", u8_str_c_u8_str(u8_str_vec_get(vec, 1)));
    ck_assert_str_eq("\xe4\xb8\xad\xe5\x9b\xbd", u8_str_c_u8_str(u8_str_vec_get(vec, 2)));
    ck_assert_str_eq("\xe4\xba\xba", u8_str_c_u8_str(u8_str_vec_get(vec, 3)));
    ck_assert_str_eq("\xe6\x88\x91", u8_str_c_u8_str(u8_str_vec_get(vec, 4)));
    ck_assert_str_eq("", u8_str_c_u8_str(u8_str_vec_get(vec, 5)));
    vec_free(vec);

    vec = u8_str_split_ascii(u8, "[,]");
    ck_assert_int_eq(1, vec_size(vec));
    ck_assert_str_eq(u8_str_c_u8_str(u8), u8_str_c_u8_str(u8_str_vec_get(vec, 0)));
    vec_free(vec);

    vec = u8_str_split_u8(u8, "是");
    ck_assert_int_eq(2, vec_size(vec));
    ck_assert_str_eq("\xe6\x88\x91", u8_str_c_u8_str(u8_str_vec_get(vec, 0))); //我
    ck_assert_str_eq("::\xe4\xb8\xad\xe5\x9b\xbd:\xe4\xba\xba:\xe6\x88\x91:", u8_str_c_u8_str(u8_str_vec_get(vec, 1))); //::中国:人:我:
    vec_free(vec);

    vec = u8_str_split_u16(u8, L"是");
    ck_assert_int_eq(2, vec_size(vec));
    ck_assert_str_eq("\xe6\x88\x91", u8_str_c_u8_str(u8_str_vec_get(vec, 0))); //我
    ck_assert_str_eq("::\xe4\xb8\xad\xe5\x9b\xbd:\xe4\xba\xba:\xe6\x88\x91:", u8_str_c_u8_str(u8_str_vec_get(vec, 1))); //::中国:人:我:
    vec_free(vec);

    vec = u8_str_split_gbk(u8, "\xca\xc7");
    ck_assert_int_eq(2, vec_size(vec));
    ck_assert_str_eq("\xe6\x88\x91", u8_str_c_u8_str(u8_str_vec_get(vec, 0))); //我
    ck_assert_str_eq("::\xe4\xb8\xad\xe5\x9b\xbd:\xe4\xba\xba:\xe6\x88\x91:", u8_str_c_u8_str(u8_str_vec_get(vec, 1))); //::中国:人:我:
    vec_free(vec);

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_split_lines) {
    u8_str_t *u8 = u8_str_new_from_u16(L"我是\n中国\n人\n我");
    VEC *vec = u8_str_split_lines(u8);

    ck_assert_int_eq(4, vec_size(vec));
    ck_assert_str_eq("我是", u8_str_c_u8_str(u8_str_vec_get(vec, 0)));
    ck_assert_str_eq("中国", u8_str_c_u8_str(u8_str_vec_get(vec, 1)));
    ck_assert_str_eq("人", u8_str_c_u8_str(u8_str_vec_get(vec, 2)));
    ck_assert_str_eq("我", u8_str_c_u8_str(u8_str_vec_get(vec, 3)));

    u8_str_free(u8);
    vec_free(vec);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_cmp) {
    u8_str_t *u8 = u8_str_new_from_u16(L"我是中国人");

    ck_assert(0 == u8_str_cmp_gbk(u8, "\xce\xd2\xca\xc7\xd6\xd0\xb9\xfa\xc8\xcb"));
    ck_assert(0 == u8_str_cmp_u8(u8, "\xe6\x88\x91\xe6\x98\xaf\xe4\xb8\xad\xe5\x9b\xbd\xe4\xba\xba"));
    ck_assert(0 == u8_str_cmp_u16(u8, L"我是中国人"));
    ck_assert(0 != u8_str_cmp_u16(u8, L"hksjs我是中国人"));

    u8_str_assign_ascii(u8, "abc");
    ck_assert(0 == u8_str_cmp_ascii(u8, "abc"));

    u8_str_assign_ascii(u8, "");
    ck_assert(0 == u8_str_cmp_ascii(u8, ""));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_contains) {
    u8_str_t *u8 = u8_str_new_from_u16(L"hh我是中国人jj");

    ck_assert(u8_str_contains_ascii(u8, "jj"));
    ck_assert(u8_str_contains_u8(u8, "\xe4\xb8\xad\xe5\x9b\xbd"));
    ck_assert(u8_str_contains_gbk(u8, "\xd6\xd0\xb9\xfa"));
    ck_assert(u8_str_contains_u16(u8, L"中国"));
    ck_assert(!u8_str_contains_u16(u8, L"哈哈"));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_equals) {
    u8_str_t *u8 = u8_str_new_from_u16(L"k我是中国人");

    ck_assert(u8_str_equals_gbk(u8, "k\xce\xd2\xca\xc7\xd6\xd0\xb9\xfa\xc8\xcb"));
    ck_assert(u8_str_equals_u8(u8, "k\xe6\x88\x91\xe6\x98\xaf\xe4\xb8\xad\xe5\x9b\xbd\xe4\xba\xba"));
    ck_assert(u8_str_equals_u16(u8, L"k我是中国人"));
    ck_assert(!u8_str_equals_u16(u8, L"K我是中国人"));

    u8_str_assign_ascii(u8, "abc");
    ck_assert(u8_str_equals_ascii(u8, "abc"));
    ck_assert(!u8_str_equals_ascii(u8, "AbC"));

    u8_str_assign_ascii(u8, "");
    ck_assert(u8_str_equals_ascii(u8, ""));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_equals_ignore_case) {
    u8_str_t *u8 = u8_str_new_from_u16(L"k我是中国人");

    ck_assert(u8_str_equals_ignore_case_gbk(u8, "k\xce\xd2\xca\xc7\xd6\xd0\xb9\xfa\xc8\xcb"));
    ck_assert(u8_str_equals_ignore_case_u8(u8, "k\xe6\x88\x91\xe6\x98\xaf\xe4\xb8\xad\xe5\x9b\xbd\xe4\xba\xba"));
    ck_assert(u8_str_equals_ignore_case_u16(u8, L"k我是中国人"));
    ck_assert(u8_str_equals_ignore_case_u16(u8, L"K我是中国人"));

    u8_str_assign_ascii(u8, "abc");
    ck_assert(u8_str_equals_ignore_case_ascii(u8, "abc"));
    ck_assert(u8_str_equals_ignore_case_ascii(u8, "AbC"));

    u8_str_assign_ascii(u8, "");
    ck_assert(u8_str_equals_ignore_case_ascii(u8, ""));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_empty) {
    u8_str_t *u8 = u8_str_new_from_u8("");

    ck_assert(u8_str_empty(u8));
    u8_str_assign_ascii(u8, " ");
    ck_assert(!u8_str_empty(u8));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_matches) {
    u8_str_t *u8 = u8_str_new_from_u8("123中");

    ck_assert(u8_str_matches_gbk(u8, "\xd6\xd0"));
    ck_assert(u8_str_matches_u8(u8, "\xe4\xb8\xad"));
    ck_assert(u8_str_matches_u16(u8, L"[0-9]+中"));

    u8_str_assign_ascii(u8, "123");
    ck_assert(u8_str_matches_ascii(u8, "[[:digit:]]+"));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_starts_with) {
    u8_str_t *u8 = u8_str_new_from_u16(L"中国hello");

    ck_assert(u8_str_starts_with_u8(u8, "\xe4\xb8\xad"));
    ck_assert(u8_str_starts_with_u16(u8, L"中"));
    ck_assert(u8_str_starts_with_gbk(u8, "\xd6\xd0"));

    u8_str_assign_ascii(u8, "hello world");
    ck_assert(u8_str_starts_with_ascii(u8, "hello"));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_index_of) {
    u8_str_t *u8 = u8_str_new_from_u16(L"中国你, 中国你真好好");

    ck_assert_int_eq(6, u8_str_index_of_u16(u8, 2, L"国你"));
    ck_assert_int_eq(6, u8_str_index_of_gbk(u8, 2, "\xb9\xfa\xc4\xe3"));
    ck_assert_int_eq(6, u8_str_index_of_u8(u8, 2, "\xe5\x9b\xbd\xe4\xbd\xa0"));

    u8_str_assign_ascii(u8, "helloworldjjworld");
    ck_assert_int_eq(12, u8_str_index_of_ascii(u8, 6, "world"));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_rindex_of) {
    u8_str_t *u8 = u8_str_new_from_u16(L"中国你, 中国你真好好");

    ck_assert_int_eq(6, u8_str_rindex_of_u16(u8, 100, L"国你"));
    ck_assert_int_eq(6, u8_str_rindex_of_gbk(u8, 100, "\xb9\xfa\xc4\xe3"));
    ck_assert_int_eq(6, u8_str_rindex_of_u8(u8, 100, "\xe5\x9b\xbd\xe4\xbd\xa0"));

    u8_str_assign_ascii(u8, "helloworldjjworld");
    ck_assert_int_eq(12, u8_str_rindex_of_ascii(u8, 100, "world"));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_replace_first) {
    u8_str_t *u8 = u8_str_new_from_u16(L"kk中国你好，中国你真真好");
    
    u8_str_replace_first_u16(u8, 1, L"中国", L"北京");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_assign_u16(u8, L"kk中国你好，中国你真真好");
    u8_str_replace_first_u8(u8, 1, "\xe4\xb8\xad\xe5\x9b\xbd", "\xe5\x8c\x97\xe4\xba\xac");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_assign_u16(u8, L"kk中国你好，中国你真真好");
    u8_str_replace_first_gbk(u8, 1, "\xd6\xd0\xb9\xfa", "\xb1\xb1\xbe\xa9");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe4\xb8\xad\xe5\x9b\xbd\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));


    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_replace) {
    u8_str_t *u8 = u8_str_new_from_u16(L"kk中国你好，中国你真真好");
    
    u8_str_replace_u16(u8, L"中国", L"北京");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_assign_u16(u8, L"kk中国你好，中国你真真好");
    u8_str_replace_u8(u8, "\xe4\xb8\xad\xe5\x9b\xbd", "\xe5\x8c\x97\xe4\xba\xac");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_assign_u16(u8, L"kk中国你好，中国你真真好");
    u8_str_replace_gbk(u8, "\xd6\xd0\xb9\xfa", "\xb1\xb1\xbe\xa9");
    ck_assert_str_eq("kk\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe5\xa5\xbd\xef\xbc\x8c\xe5\x8c\x97\xe4\xba\xac\xe4\xbd\xa0\xe7\x9c\x9f\xe7\x9c\x9f\xe5\xa5\xbd", u8_str_c_u8_str(u8));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_lower_upper_case) {
    u8_str_t *u8 = u8_str_new_from_ascii("hElLo");

    u8_str_to_uppercase(u8);
    ck_assert_str_eq("HELLO", u8_str_c_u8_str(u8));

    u8_str_to_lowercase(u8);
    ck_assert_str_eq("hello", u8_str_c_u8_str(u8));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_u8_char) {
    u8_str_t *u8 = u8_str_new_from_u16(L"中国你好");
    u8_char_t u8c;

    u8_str_at(u8, 1, &u8c);
    ck_assert_int_eq(3, u8_char_byte_size(&u8c));

    char buf[4];
    u8_char_to_u8_str(&u8c, buf, 4);
    ck_assert_str_eq("国", buf);

    ck_assert(0 == u8_char_cmp(&u8c, (byte_t*)buf, (byte_t*)(buf + 4)));

    u8_str_free(u8);
    ck_assert_no_leak();
}
END_TEST

START_DEFINE_SUITE(u8_str)
    TEST(test_new_from)
    TEST(test_size_byte_count)
    TEST(test_append)
    TEST(test_pos_to_pos)
    TEST(test_insert)
    TEST(test_assign)
    TEST(test_at)
    TEST(test_sub)
    TEST(test_trim)
    TEST(test_split)
    TEST(test_split_lines)
    TEST(test_cmp)
    TEST(test_contains)
    TEST(test_equals)
    TEST(test_equals_ignore_case)
    TEST(test_empty)
    TEST(test_matches)
    TEST(test_starts_with)
    TEST(test_index_of)
    TEST(test_rindex_of)
    TEST(test_replace_first)
    TEST(test_replace)
    TEST(test_lower_upper_case)
    TEST(test_u8_char)
END_DEFINE_SUITE()
