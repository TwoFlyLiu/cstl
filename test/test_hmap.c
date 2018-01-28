/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 01 18:38:08
*/
#include <check_util.h>
#include <string.h>
#include <assert.h>

#include "hmap.h"
#include "test_common.h"

START_TEST(test_create) {
    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
             CSTL_NUM_CMP_FUNC(char));

    ck_assert_int_eq(sizeof(char), hmap->key_size);
    ck_assert_int_eq(sizeof(int), hmap->value_size);
    ck_assert_int_eq(0, hmap->len);

    ck_assert(CSTL_NUM_HASH_FUNC(char) == hmap->hash_func);
    ck_assert(CSTL_NUM_CMP_FUNC(char) == hmap->key_cmp_func);

    hmap_free(hmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert_erase_size) {

    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
            CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;

    hmap_insert(hmap, &key, &val);
    ck_assert_int_eq(1, hmap_size(hmap)); 
    ck_assert_int_eq(false, hmap_empty(hmap));

    key = 'b'; val = 98;
    hmap_insert(hmap, &key, &val);
    ck_assert_int_eq(2, hmap_size(hmap));

    hmap_erase(hmap, &key);
    ck_assert_int_eq(1, hmap_size(hmap));

    key = 'a';
    hmap_erase(hmap, &key);
    ck_assert_int_eq(0, hmap_size(hmap));

    hmap_free(hmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_get_set) {
    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
            CSTL_NUM_CMP_FUNC(char));
    ck_assert(hmap_empty(hmap));

    char key = 'a';
    int val = 97;

    hmap_insert(hmap, &key, &val);
    ck_assert(*(int*)hmap_get(hmap, &key) == val);
    ck_assert_int_eq(1, hmap_size(hmap));

    val = 100;
    hmap_set(hmap, &key, &val);
    ck_assert(*(int*)hmap_get(hmap, &key) == 100);
    ck_assert_int_eq(1, hmap_size(hmap));

    key = 'b';
    ck_assert(!hmap_has_key(hmap, &key));

    hmap_free(hmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_has_key) {
    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
             CSTL_NUM_CMP_FUNC(char));
    int key = 'a';
    int value = 97;

    ck_assert(!hmap_has_key(hmap, &key));

    hmap_insert(hmap, &key, &value);
    ck_assert(hmap_has_key(hmap, &key));

    key = 'b';
    ck_assert(!hmap_has_key(hmap, &key));

    value = 98;
    hmap_insert(hmap, &key, &value);
    ck_assert(hmap_has_key(hmap, &key));

    hmap_free(hmap);
    ck_assert_no_leak();
}
END_TEST

typedef struct {
    char key;
    int val;
} data_t;

typedef struct {
    data_t *datas;
    int pos;
} data_cursor_t;

static void
__for_each(char *key, int *value, data_cursor_t *user_data)
{
    ck_assert_int_eq(*value, user_data->datas[user_data->pos].val);
    ck_assert_int_eq(*key, user_data->datas[user_data->pos].key);
    ++ user_data->pos;
}

START_TEST(test_for_each) {
    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
             CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;
    data_t expect_datas[] = {
        {.key='a', .val=97},
        {.key='b', .val=98},
        {.key='c', .val=99}
    };
    data_cursor_t data_cursor = {.datas=expect_datas, .pos=0};

    hmap_insert(hmap, &key, &val);

    key = 'b'; val = 98;
    hmap_insert(hmap, &key, &val);

    key = 'c'; val = 99;
    hmap_insert(hmap, &key, &val);

    /*key = 'd';*/
    /*ck_assert(hmap_get(hmap, &key) == NULL);*/
    /*ck_assert(!hmap_has_key(hmap, &key));*/

    hmap_for_each(hmap, (HMAP_FOR_EACH)__for_each, &data_cursor);
    hmap_free(hmap);
    ck_assert_no_leak();
}
END_TEST

static unsigned int
__str_hash_func(const void *key)
{
    char *v_key = (char *)key;
    int len = strlen(v_key);
    unsigned int hash = 0;

    for (int i = 0; i < len; i++) {
        hash += v_key[i];
    }
    return hash;
}

static int
__str_cmp(const void *lhs, const void *rhs)
{
    return strcmp((char*)lhs, (char*)rhs);
}

typedef struct {
    const char *key;
    int val;
} __str_for_each_data_t;

// hmap中的数据是无序的
static void
__str_for_each(const void *key, void *value, void *user_data)
{
    const char *v_key = (const char*)key;
    int v_value = *(int*)value;
    user_data = NULL;
    printf("key=%s, val=%d\n", v_key, v_value);
}

START_TEST(test_arr_key) {
    // 只能使用数组，不能使用指针
    char key[100] = "zs"; 
    int value = 22;
    HMAP *hmap = hmap_new(sizeof(key), sizeof(int),
            __str_hash_func, __str_cmp);

    hmap_insert(hmap, key, &value);
    ck_assert_int_eq(1, hmap_size(hmap));

    strcpy(key, "ls");
    value = 23;
    hmap_insert(hmap, key, &value);

    strcpy(key, "wh");
    value = 24;
    hmap_insert(hmap, key, &value);

    hmap_for_each(hmap, __str_for_each, NULL);

    hmap_free(hmap);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_erase_clear) {
    HMAP *hmap = hmap_new(sizeof(char), sizeof(int), CSTL_NUM_HASH_FUNC(char),
             CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;

    hmap_insert(hmap, &key, &val);

    key = 'b'; val = 98;
    hmap_insert(hmap, &key, &val);

    key = 'c'; val = 99;
    hmap_insert(hmap, &key, &val);

    key = 'b';
    ck_assert(hmap_has_key(hmap, &key));

    hmap_erase(hmap, &key);
    ck_assert(!hmap_has_key(hmap, &key));

    key = 'a';
    hmap_clear(hmap);
    ck_assert(!hmap_has_key(hmap, &key));

    hmap_free(hmap);
    ck_assert_no_leak();
}
END_TEST

typedef struct {
    int age;
    char name[50];
} person_t;

static int person_hmap_key_equal(char **lhv, char **rhv)
{
    return strcmp(*lhv, *rhv);
}

static void person_hmap_val_destroy(person_t ** val)
{
    cstl_free(*val);
}

static person_t *person_create(int age, const char *name)
{
    person_t *person = (person_t*)cstl_malloc(sizeof(person_t));
    strcpy(person->name, name);
    person->age = age;

    return person;
}

START_TEST(test_destroy) {
    // 创建const char *const -> person_t*
    HMAP *hmap = hmap_new_with_destroy_func(sizeof(const char *const), sizeof(person_t*),
            CSTL_PTR_HASH_FUNC(),
            (cmp_func_t)&person_hmap_key_equal,
            NULL,
            (destroy_func_t)&person_hmap_val_destroy
            );
    person_t *person = person_create(22, "ffx");

    // 必须要用一个char**用保存指向数组的指针，不能直接 &数组名，因为 &数组名 返回的是数组本身的地址
    char *name = person->name;
    ck_assert_str_eq(name, person->name);

    hmap_insert(hmap, &name, &person);
    ck_assert_int_eq(1, hmap_size(hmap));

    ck_assert(person == *(person_t**)hmap_get(hmap, &name));
    hmap_free(hmap);

    ck_assert_no_leak();
}
END_TEST

START_DEFINE_SUITE(hmap)
    TEST(test_create)
    TEST(test_insert_erase_size)
    TEST(test_get_set)
    TEST(test_has_key)
    TEST(test_for_each)
    TEST(test_arr_key)
    TEST(test_erase_clear)
    TEST(test_destroy)
END_DEFINE_SUITE()
