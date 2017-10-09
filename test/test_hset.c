/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 03 21:25:42
*/
#include <check_util.h>
#include <string.h>

#include "hset.h"
#include "test_common.h"

typedef char str_t[200];

START_TEST(test_new_free) {
    hset_t *hset = hset_new(sizeof(str_t),CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    
    ck_assert(hset != NULL);
    ck_assert(hset->data != NULL);
    ck_assert_int_eq(hset->data->key_size, sizeof(str_t));
    ck_assert_int_eq(hset->data->value_size, 0);

    hset_free(hset);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_empty) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert(hset_empty(hset));

    strcpy(value, "int");
    hset_insert(hset, value);
    ck_assert(!hset_empty(hset));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_size) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert_int_eq(0, hset_size(hset));

    strcpy(value, "int");
    hset_insert(hset, value);
    ck_assert_int_eq(1, hset_size(hset));

    strcpy(value, "float");
    hset_insert(hset, value);
    ck_assert_int_eq(2, hset_size(hset));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_clear) {
    str_t value;
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    
    strcpy(value, "int");
    hset_insert(hset, value);

    strcpy(value, "float");
    hset_insert(hset, value);

    ck_assert(!hset_empty(hset));

    hset_clear(hset);
    ck_assert(hset_empty(hset));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert_int_eq(0, hset_size(hset));

    strcpy(value, "int");
    hset_insert(hset, value);

    strcpy(value, "float");
    hset_insert(hset, value);
    ck_assert_int_eq(2, hset_size(hset));

    // 插入相同数据，不会增加新的内容
    strcpy(value, "float");
    hset_insert(hset, value);
    ck_assert_int_eq(2, hset_size(hset));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_erase) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert_int_eq(0, hset_size(hset));

    strcpy(value, "int");
    hset_insert(hset, value);
    ck_assert_int_eq(1, hset_size(hset));

    strcpy(value, "float");
    hset_insert(hset, value);
    ck_assert_int_eq(2, hset_size(hset));

    ck_assert(hset_contains(hset, value));
    hset_erase(hset, value);
    ck_assert(!hset_contains(hset, value));
    ck_assert_int_eq(1, hset_size(hset));

    // 删除不存在的值
    strcpy(value, "dummy");
    hset_erase(hset, value);
    ck_assert_int_eq(1, hset_size(hset));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_find) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert_int_eq(0, hset_size(hset));

    strcpy(value, "int");
    hset_insert(hset, value);

    strcpy(value, "float");
    hset_insert(hset, value);

    ck_assert(hset_find(hset, value) != NULL);

    // 删除不存在的值
    strcpy(value, "dummy");
    ck_assert(hset_find(hset, value) == NULL);

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_contains) {
    hset_t *hset = hset_new(sizeof(str_t), CSTL_STR_HASH_FUNC, (cmp_func_t)strcmp);
    str_t value;
    
    ck_assert_int_eq(0, hset_size(hset));

    strcpy(value, "int");
    hset_insert(hset, value);

    strcpy(value, "float");
    hset_insert(hset, value);

    ck_assert(hset_contains(hset, value));

    // 删除不存在的值
    strcpy(value, "dummy");
    ck_assert(!hset_contains(hset, value));

    hset_free(hset);
    ck_assert_no_leak();
}
END_TEST

START_DEFINE_SUITE(hset)
    TEST(test_new_free)
    TEST(test_empty)
    TEST(test_insert)
    TEST(test_size)
    TEST(test_clear)
    TEST(test_find)
    TEST(test_erase)
    TEST(test_contains)
END_DEFINE_SUITE()
