/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 11 12 23:21:06
*/
#include <check_util.h>

#include "rmap.h"
#include "test_common.h"

START_TEST(test_create) {
    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
             CSTL_NUM_CMP_FUNC(char));
    ck_assert_int_eq(0, rmap_size(rmap));
    rmap_free(rmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert_erase_size) {

    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
            CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;

    rmap_insert(rmap, &key, &val);
    ck_assert_int_eq(1, rmap_size(rmap)); 
    ck_assert_int_eq(false, rmap_empty(rmap));

    key = 'b'; val = 98;
    rmap_insert(rmap, &key, &val);
    ck_assert_int_eq(2, rmap_size(rmap));

    rmap_erase(rmap, &key);
    ck_assert_int_eq(1, rmap_size(rmap));

    key = 'a';
    rmap_erase(rmap, &key);
    ck_assert_int_eq(0, rmap_size(rmap));

    rmap_free(rmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_get_set) {
    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
            CSTL_NUM_CMP_FUNC(char));
    ck_assert(rmap_empty(rmap));

    char key = 'a';
    int val = 97;

    rmap_insert(rmap, &key, &val);
    ck_assert(*(int*)rmap_get(rmap, &key) == val);
    ck_assert_int_eq(1, rmap_size(rmap));

    val = 100;
    rmap_set(rmap, &key, &val);
    ck_assert(*(int*)rmap_get(rmap, &key) == 100);
    ck_assert_int_eq(1, rmap_size(rmap));

    key = 'b';
    ck_assert(!rmap_has_key(rmap, &key));

    rmap_free(rmap);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_has_key) {
    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
             CSTL_NUM_CMP_FUNC(char));
    int key = 'a';
    int value = 97;

    ck_assert(!rmap_has_key(rmap, &key));

    rmap_insert(rmap, &key, &value);
    ck_assert(rmap_has_key(rmap, &key));

    key = 'b';
    ck_assert(!rmap_has_key(rmap, &key));

    value = 98;
    rmap_insert(rmap, &key, &value);
    ck_assert(rmap_has_key(rmap, &key));

    rmap_free(rmap);
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
__for_each(char *key, int *value, data_cursor_t __attribute__((unused)) *user_data)
{
    //因为是无序的，不能使用那样测试，因为顺序不确定，不能按照先后顺序来进行测试
    /*ck_assert_int_eq(*value, user_data->datas[user_data->pos].val);*/
    /*ck_assert_int_eq(*key, user_data->datas[user_data->pos].key);*/
    /*++ user_data->pos;*/
    printf("rmap-> key=%c, value=%d\n", *key, *value);
}

START_TEST(test_for_each) {
    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
             CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;
    data_t expect_datas[] = {
        {.key='a', .val=97},
        {.key='b', .val=98},
        {.key='c', .val=99}
    };
    data_cursor_t UNUSED data_cursor = {.datas=expect_datas, .pos=0};

    rmap_insert(rmap, &key, &val);

    key = 'b'; val = 98;
    rmap_insert(rmap, &key, &val);

    key = 'c'; val = 99;
    rmap_insert(rmap, &key, &val);

    key = 'd';
    ck_assert(rmap_get(rmap, &key) == NULL);
    ck_assert(!rmap_has_key(rmap, &key));

    rmap_for_each(rmap, (RMAP_FOR_EACH)__for_each, &data_cursor);
    rmap_free(rmap);
    ck_assert_no_leak();
}
END_TEST

static int
__str_cmp(const void *lhs, const void *rhs)
{
    return strcmp((char*)lhs, (char*)rhs);
}

typedef struct {
    const char *key;
    int val;
} __str_for_each_data_t;

// rmap中的数据是无序的
// 输出内部的key和value
/*static void*/
/*__str_for_each(const void *key, void *value, void *user_data)*/
/*{*/
    /*const char *v_key = (const char*)key;*/
    /*int v_value = *(int*)value;*/
    /*user_data = NULL;*/
    /*printf("key=%s, val=%d\n", v_key, v_value);*/
/*}*/

START_TEST(test_arr_key) {
    // 只能使用数组，不能使用指针
    char key[100] = "zs"; 
    int value = 22;
    rmap_t *rmap = rmap_new(sizeof(key), sizeof(int),
             __str_cmp);

    rmap_insert(rmap, key, &value);
    ck_assert_int_eq(1, rmap_size(rmap));

    strcpy(key, "ls");
    value = 23;
    rmap_insert(rmap, key, &value);

    strcpy(key, "wh");
    value = 24;
    rmap_insert(rmap, key, &value);

    rmap_free(rmap);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_erase_clear) {
    rmap_t *rmap = rmap_new(sizeof(char), sizeof(int), 
             CSTL_NUM_CMP_FUNC(char));
    char key = 'a';
    int val = 97;

    rmap_insert(rmap, &key, &val);

    key = 'b'; val = 98;
    rmap_insert(rmap, &key, &val);

    key = 'c'; val = 99;
    rmap_insert(rmap, &key, &val);

    key = 'b';
    ck_assert(rmap_has_key(rmap, &key));

    rmap_erase(rmap, &key);
    ck_assert(!rmap_has_key(rmap, &key));

    key = 'a';
    rmap_clear(rmap);
    ck_assert(!rmap_has_key(rmap, &key));

    rmap_free(rmap);
    ck_assert_no_leak();
}
END_TEST

typedef struct {
    int age;
    char name[50];
} person_t;

static int person_rmap_key_equal(char **lhv, char **rhv)
{
    return strcmp(*lhv, *rhv);
}

static void person_rmap_val_destroy(person_t ** val)
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
    rmap_t *rmap = rmap_new_with_destroy_func(sizeof(const char *const), sizeof(person_t*),
            (cmp_func_t)&person_rmap_key_equal,
            NULL,
            (destroy_func_t)&person_rmap_val_destroy
            );
    person_t *person = person_create(22, "ffx");

    // 必须要用一个char**用保存指向数组的指针，不能直接 &数组名，因为 &数组名 返回的是数组本身的地址
    char *name = person->name;
    ck_assert_str_eq(name, person->name);

    rmap_insert(rmap, &name, &person);
    ck_assert_int_eq(1, rmap_size(rmap));

    ck_assert(person == *(person_t**)rmap_get(rmap, &name));
    rmap_free(rmap);

    ck_assert_no_leak();
}
END_TEST

START_DEFINE_SUITE(rmap)
    TEST(test_create)
    TEST(test_insert_erase_size)
    TEST(test_get_set)
    TEST(test_has_key)
    TEST(test_for_each)
    TEST(test_arr_key)
    TEST(test_erase_clear)
    TEST(test_destroy)
END_DEFINE_SUITE()
