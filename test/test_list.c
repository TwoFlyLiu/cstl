/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 09 30 16:48:18
*/
#include <check_util.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "test_common.h"

START_TEST(test_new) {
    LIST *list = list_new(sizeof(int), NULL); //默认元素原谅是10
    ck_assert(list != NULL);
    ck_assert_int_eq(list_capacity(list), (size_t)-1);
    list_free(list);

    ck_assert_no_leak();
}
END_TEST

START_TEST(test_push_back) {
    LIST *list = list_new(sizeof(int), NULL);

    ck_assert_int_eq(0, list_size(list));
    ck_assert(NULL == list_get(list, 0));

    int i = 100;
    list_push_back(list, &i);
    ck_assert_int_eq(100, *(int*)list_get(list, 0));

    i = 10;
    list_push_back(list, &i);
    ck_assert_int_eq(10, *(int*)list_get(list, 1));

    ck_assert_int_eq(2, list_size(list));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_resize) {
    LIST *list = list_new(sizeof(int), NULL);
    int datas[] = {3, 2, 1, 4, 6, 8, 19, 10};
    int default_value = 55;

    ck_assert_int_eq((size_t)-1, list_capacity(list));
    ck_assert_int_eq(0, list_size(list));

    // 测试自动扩容
    for (size_t i = 0; i < ARRAY_SIZE(datas, int); i++) {
        list_push_back(list, &datas[i]);
    }

    ck_assert_int_eq(ARRAY_SIZE(datas, int), list_size(list));
    ck_assert_int_eq((size_t)-1, list_capacity(list));

    for (size_t i = 0; i < ARRAY_SIZE(datas, int); i++) {
        ck_assert_int_eq(datas[i], *(int*)list_get(list, i));
    }

    // 缩小容量
    list_resize(list, 5, &default_value);
    ck_assert_int_eq((size_t)-1, list_capacity(list));
    ck_assert_int_eq(5, list_size(list));

    for (size_t i = 0; i < 5; i++) {
        ck_assert_int_eq(datas[i], *(int*)list_get(list, i));
    }

    // 再次扩容
    list_push_back(list, &datas[5]);
    ck_assert_int_eq((size_t)-1, list_capacity(list));
    ck_assert_int_eq(6, list_size(list));

    for (size_t i = 0; i < 6; i++) {
        ck_assert_int_eq(datas[i], *(int*)list_get(list, i));
    }

    // 使用resize来扩大容量
    list_resize(list, 100, &default_value);
    ck_assert_int_eq(100, list_size(list));
    ck_assert_int_eq(default_value, *(int*)list_get(list, 99));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_pop_front) {
    LIST *list = list_new(sizeof(int), NULL);

    list_pop_front(list); //测试空 弹出首元素

    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    ck_assert_int_eq(0, *(int*)list_front(list));
    ck_assert_int_eq(9, *(int*)list_back(list));
    
    list_pop_front(list);
    ck_assert_int_eq(1, *(int*)list_front(list));
    ck_assert_int_eq(2, *(int*)list_get(list, 1));
    ck_assert_int_eq(9, *(int*)list_back(list));

    ck_assert_int_eq(9, list_size(list));
    ck_assert_int_eq(9, *(int*)list_get(list, list_size(list) - 1));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_pop_back) {
    LIST *list = list_new(sizeof(int), NULL);
    
    ck_assert(list_empty(list));

    list_pop_back(list); // 当list为空的时候，要保证不会出错

    for (size_t i = 0; i < 5; i++) {
        list_push_back(list, &i);
    }

    ck_assert(!list_empty(list));
    ck_assert_int_eq(5, list_size(list));
    ck_assert_int_eq((size_t)-1, list_capacity(list)); //容量没有发生变化
    ck_assert_int_eq(4, *(int*)list_get(list, list_size(list) - 1));

    list_pop_back(list);
    ck_assert_int_eq(4, list_size(list));
    ck_assert_int_eq((size_t)-1, list_capacity(list)); //容量没有发生变化

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_front_back_get) {
    LIST *list = list_new(sizeof(int), NULL);
    int i = 100;

    ck_assert(NULL == list_front(list));
    ck_assert(NULL == list_back(list));
    ck_assert(NULL == list_get(list, 10000));

    list_push_back(list, &i);

    ck_assert_int_eq(100, *(int*)list_front(list));
    ck_assert_int_eq(100, *(int*)list_back(list));
    ck_assert_int_eq(100, *(int*)list_get(list, 0));
    ck_assert_int_eq(100, *(int*)list_get(list, list_size(list) - 1));


    i = 50;
    list_push_back(list, &i);

    ck_assert_int_eq(100, *(int*)list_front(list));
    ck_assert_int_eq(50, *(int*)list_back(list));
    ck_assert_int_eq(100, *(int*)list_get(list, 0));
    ck_assert_int_eq(50, *(int*)list_get(list, list_size(list) - 1));

    list_free(list); 
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_push_front) {
    LIST *list = list_new(sizeof(int), NULL);
    int elem = 100;

    ck_assert(list_empty(list));
    
    list_push_front(list, &elem);
    ck_assert(!list_empty(list));
    ck_assert_int_eq(1, list_size(list));

    ck_assert_int_eq(100, *(int*)list_front(list));
    ck_assert_int_eq(100, *(int*)list_back(list));

    elem = 200;
    list_push_front(list, &elem);
    ck_assert_int_eq(200, *(int*)list_front(list));
    ck_assert_int_eq(100, *(int*)list_back(list));

    elem = 300;
    list_push_front(list, &elem);
    ck_assert_int_eq(300, *(int*)list_front(list));
    ck_assert_int_eq(100, *(int*)list_back(list));
    ck_assert_int_eq(3, list_size(list));

    list_free(list);

    list = list_new(sizeof(int), NULL);

    for (size_t i = 0; i < 100; i++) {
        list_push_front(list, &i);
    }

    for (size_t i = 0; i < 100; i++) {
        ck_assert_int_eq(99 - i, *(int*)list_get(list, i));
    }

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

typedef struct {
    int x, y;
} POINT;

static int __point_compare(void *lhs, void *rhs)
{
    POINT *pt_lhs = (POINT *)lhs;
    POINT *pt_rhs = (POINT *)rhs;

    return (pt_lhs->x == pt_rhs->x && pt_lhs->y == pt_rhs->y) ? 0 : 1;
}

START_TEST(test_find) {
    LIST *list = list_new(sizeof(POINT), NULL);
    POINT pt = {.x = 1, .y = 2};
    POINT to_find = {5, 6};

    // 空容器查找
    ck_assert(NULL == list_find(list, NULL, __point_compare));

    // 有值时候
    list_push_back(list, &pt);
    ck_assert(NULL == list_find(list, &to_find, __point_compare));

    list_push_back(list, &to_find);
    ck_assert(NULL != list_find(list, &to_find, __point_compare));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_insert) {
    int elem = 100;
    int old_back;

    // 测试空值插入
    LIST *list = list_new(sizeof(int), NULL);
    list_insert(list, 0, &elem);
    ck_assert_int_eq(100, *(int*)list_front(list));
    list_free(list);

    list = list_new(sizeof(int), NULL);
    list_insert(list, 100, &elem); //在无效位置插入，会变成在尾部插入
    ck_assert_int_eq(100, *(int*)list_back(list));
    list_free(list);

    // 测试有值插入
    list = list_new(sizeof(int), NULL);

    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    for (size_t i = 0; i < 10; i++) {
        ck_assert_int_eq(i, *(int*)list_get(list, i));
    }

    list_insert(list, 5, &elem);
    ck_assert_int_eq(elem, *(int*)list_get(list, 5));

    // 因为从原来的索引5开始，值都向后移动了一位，所以向下面这样进行测试
    for (size_t i = 6; i < 11; i++) {
        ck_assert_int_eq(i - 1, *(int*)list_get(list, i));
    }

    // 单独测试有值时候，在行首和行尾插入效果
    elem = 1000;
    list_insert(list, 0, &elem);
    ck_assert_int_eq(elem, *(int*)list_front(list));

    elem = 500;
    list_insert(list, 1000, &elem);
    ck_assert_int_eq(elem, *(int*)list_back(list));

    old_back = *(int*)list_back(list);
    
    // 测试在最后一个元素位置上面插入（边界情况）
    elem = 55;
    list_insert(list, list_size(list) - 1, &elem);
    ck_assert_int_eq(old_back, *(int*)list_back(list));
    ck_assert_int_eq(elem, *(int*)list_get(list, list_size(list) - 2));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_extend) {
    // 先测试在末尾插入
    LIST *list = list_new(sizeof(int), NULL); 
    LIST *insert_list = list_new(sizeof(int), NULL);

    for (size_t i = 0; i < 20; i++) {
        list_push_back(insert_list, &i);
    }

    ck_assert_int_eq((size_t)-1, list_capacity(insert_list));
    ck_assert_int_eq(20, list_size(insert_list));

    list_extend(list, list_size(list), insert_list);

    for (size_t i = 0; i < 20; i++) {
        ck_assert_int_eq(i, *(int *)list_get(list, i));
    }

    list_free(list);

    list = list_new(sizeof(int), NULL);
    for (size_t i = 100; i < 1000; i += 50) {
        list_push_back(list, &i);
    }

    // 在非位置位置插入值
    // 
    // 在头部插入值
    list_extend(list, 0, insert_list);
    ck_assert_int_eq(0, *(int*)list_front(list));
    ck_assert_int_eq(100, *(int*)list_get(list, 20));

    list_free(list);

    // 在最后一个元素位置插入值(相当于在中间插入)
    list = list_new(sizeof(int), NULL);
    for (size_t i = 100; i < 1000; i += 50) {
        list_push_back(list, &i);
    }

    list_extend(list, list_size(list) - 1, insert_list);
    ck_assert_int_eq(950, *(int*)list_back(list));
    ck_assert_int_eq(19, *(int*)list_get(list, list_size(list) - 2));
    ck_assert_int_eq(0, *(int*)list_get(list, list_size(list) - 21));
    ck_assert_int_eq(38, list_size(list));

    list_free(list);

    // 在中间插入一个元素
    list = list_new(sizeof(int), NULL);
    for (size_t i = 100; i < 1000; i += 50) {
        list_push_back(list, &i);
    }

    list_extend(list, 2, insert_list);
    ck_assert_int_eq(38, list_size(list));
    ck_assert_int_eq(0, *(int*)list_get(list, 2));

    list_free(list);

    list_free(insert_list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_set) {
    LIST *list = list_new(sizeof(int), NULL);
    int data = 100;

    list_push_back(list, &data);
    ck_assert_int_eq(data, *(int*)list_get(list, 0));

    data = 200;
    list_set(list, 0, &data);
    ck_assert_int_eq(data, *(int*)list_get(list, 0));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_erase) {
    LIST *list = list_new(sizeof(int), NULL);
    
    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    // 移除头部
    list_erase(list, 0);
    ck_assert_int_eq(1, *(int*)list_front(list));
    ck_assert_int_eq(9, list_size(list));

    // 移除尾部
    list_erase(list, list_size(list) - 1);
    ck_assert_int_eq(8, *(int*)list_back(list));
    ck_assert_int_eq(8, list_size(list));

    /*// 移除中间元素*/
    list_erase(list, 1);
    ck_assert_int_eq(3, *(int*)list_get(list, 1));
    ck_assert_int_eq(7, list_size(list));

    list_erase(list, 1);
    ck_assert_int_eq(4, *(int*)list_get(list, 1));
    ck_assert_int_eq(6, list_size(list));

    list_free(list);

    list = list_new(sizeof(int), NULL);

    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    ck_assert_int_eq(0, *(int*)list_get(list, 0));
    ck_assert_int_eq(1, *(int*)list_get(list, 1));
    ck_assert_int_eq(2, *(int*)list_get(list, 2));
    ck_assert_int_eq(3, *(int*)list_get(list, 3));
    ck_assert_int_eq(4, *(int*)list_get(list, 4));
    ck_assert_int_eq(5, *(int*)list_get(list, 5));
    ck_assert_int_eq(6, *(int*)list_get(list, 6));
    ck_assert_int_eq(7, *(int*)list_get(list, 7));
    ck_assert_int_eq(8, *(int*)list_get(list, 8));
    ck_assert_int_eq(9, *(int*)list_get(list, 9));

    list_erase(list, 8);
    ck_assert_int_eq(9, *(int*)list_get(list, 8));
    ck_assert_int_eq(9, list_size(list));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_clear) {
    LIST *list = list_new(sizeof(int), NULL);

    // 删除空的(保证不出错)
    list_clear(list);
    ck_assert_int_eq(0, list_size(list));
    ck_assert_int_eq((size_t)-1, list_capacity(list));

    // 有值删除
    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    ck_assert_int_eq(10, list_size(list));

    list_clear(list);
    ck_assert(list_empty(list));

    // 有值删除(测试第二次，是为了确定调用clear后，list还是有效的)
    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    ck_assert_int_eq(10, list_size(list));

    list_clear(list);
    ck_assert(list_empty(list));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_remove) {
    LIST *list = list_new(sizeof(int), NULL);
    int data = 8;

    for (size_t i = 0; i < 10; i++) {
        list_push_back(list, &i);
    }

    // 没有移除之前可以找到
    ck_assert(NULL != list_find(list, &data, CSTL_NUM_CMP_FUNC(int)));

    // 移除之后就不能找到了
    list_remove(list, &data, CSTL_NUM_CMP_FUNC(int));
    ck_assert(NULL == list_find(list, &data, CSTL_NUM_CMP_FUNC(int)));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_sort) {
    LIST *list = list_new(sizeof(int), NULL);

    int data = 5;
    list_push_back(list, &data);

    data = 1;
    list_push_back(list, &data);

    data = 3;
    list_push_back(list, &data);

    data = 2;
    list_push_back(list, &data);

    data = 4;
    list_push_back(list, &data);

    list_sort(list, CSTL_NUM_CMP_FUNC(int));

    for (size_t i = 0; i < list_size(list); i++) {
        ck_assert_int_eq(i+1, *(int*)list_get(list, i));
    }

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_swap) {
    LIST *list = list_new(sizeof(int), NULL);
    int data = 1;

    list_push_back(list, &data);

    data = 2;
    list_push_back(list, &data);

    ck_assert_int_eq(1, *(int*)list_get(list, 0));
    ck_assert_int_eq(2, *(int*)list_get(list, 1));

    list_swap(list, 0, 1);
    ck_assert_int_eq(2, *(int*)list_get(list, 0));
    ck_assert_int_eq(1, *(int*)list_get(list, 1));

    data = 3;
    list_push_back(list, &data);

    list_swap(list, 1, 2);
    ck_assert_int_eq(3, *(int*)list_get(list, 1));
    ck_assert_int_eq(1, *(int*)list_get(list, 2));

    list_free(list);
    ck_assert_no_leak();
}
END_TEST


START_TEST(test_list_num_compare) {
    cmp_func_t compare = CSTL_NUM_CMP_FUNC(int);
    int num1 = 1, num2 = 2;

    ck_assert_int_eq((*compare)(&num1, &num2), -1);
    ck_assert_int_eq((*compare)(&num2, &num1), 1);
    ck_assert((*compare)(&num1, &num1) == 0);
    ck_assert((*compare)(&num1, &num2) <= 0);
    ck_assert((*compare)(&num1, &num1) <= 0);
    ck_assert_no_leak();
}
END_TEST

START_TEST(test_list_type_macro) {
    LIST *list = int_list_new();
    int data = 10;
    
    list_push_back(list, &data);
    ck_assert_int_eq(10, *int_list_get(list, 0));
    ck_assert_int_eq(10, *int_list_at(list, 0));
    ck_assert(NULL == int_list_get(list, 100));
    ck_assert(NULL == int_list_at(list, 100));

    data = 20;
    list_push_back(list, &data);

    ck_assert_int_eq(10, *int_list_front(list));
    ck_assert_int_eq(20, *int_list_back(list));

    ck_assert(int_list_find(list, 10) != NULL);


    int_list_push_front(list, 50);
    ck_assert_int_eq(50, *int_list_front(list));

    int_list_push_back(list, 1000);
    ck_assert_int_eq(1000, *int_list_back(list));

    int_list_insert(list, 2, 22);
    ck_assert_int_eq(22, *int_list_get(list, 2));

    int_list_set(list, 2, 50);
    ck_assert_int_eq(50, *int_list_get(list, 2));

    int_list_remove(list, 1000);
    ck_assert(int_list_find(list, 1000) == NULL);

    list_free(list);    

    list = int_list_new();
    
    int_list_push_back(list, 4);
    int_list_push_back(list, 1);
    int_list_push_back(list, 3);
    int_list_push_back(list, 5);
    int_list_push_back(list, 2);

    int_list_sort(list);

    for (size_t i = 0; i < list_size(list); i++) {
        ck_assert_int_eq(i+1, *int_list_get(list, i));
    }

    int_list_resize(list, 10000, 5);
    ck_assert_int_eq(10000, list_size(list));
    ck_assert_int_eq(5, *int_list_get(list, 9999));

    list_free(list);

    
    // 无符号数值类型测试
    list = unsigned_int_list_new();

    unsigned_int_list_push_back(list, -10);
    ck_assert_uint_eq((unsigned int)-10, *unsigned_int_list_front(list));
    list_free(list);

    list = double_list_new();

    double_list_push_back(list, 3.14);
    ck_assert(double_list_find(list, 3.140000000002) != NULL);

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

typedef struct {
   int data; 
} data_t, DATA;

static int
__data_cmp(void *lhs, void *rhs)
{
    DATA *v_lhs = (DATA *)lhs;
    DATA *v_rhs = (DATA *)rhs;
    cmp_func_t cmp = CSTL_NUM_CMP_FUNC(int);

    return (*cmp)(&v_lhs->data, &v_rhs->data);
}

#define DATA_CMP __data_cmp

DEFINE_STRUCT_TYPE_LIST(data_t, DATA_CMP)

typedef struct {
    const char *name;
    int age;
} person_t, PERSON;

static int
__person_name_cmp(void *lhs, void *rhs)
{
    PERSON *v_lhs = (PERSON*)lhs;
    PERSON *v_rhs = (PERSON*)rhs;

    return strcmp(v_lhs->name, v_rhs->name);
}

static int
__person_age_cmp(void *lhs, void *rhs)
{
    PERSON *v_lhs = (PERSON*)lhs;
    PERSON *v_rhs = (PERSON*)rhs;
    cmp_func_t cmp = CSTL_NUM_CMP_FUNC(int);

    return (*cmp)(&(v_lhs->age), &(v_rhs->age));
}

DEFINE_STRUCT_TYPE_LIST(person_t, __person_name_cmp)

typedef struct {
    PERSON* expect_persons;
    int index;
} expect_person_t;

static void _ck_assert_persons(void *p, void *e)
{
    person_t* person = (person_t*)p;
    expect_person_t *ep = (expect_person_t*)e;
    person_t *expect_person = &(ep->expect_persons[ep->index++]);

    ck_assert_str_eq(person->name, expect_person->name);
    ck_assert_int_eq(person->age, expect_person->age);
}

START_TEST(test_struct_list) {
    LIST *list = int_list_new();
    DATA data;

    PERSON zs = {.name="zs", .age=22};
    PERSON ls = {.name="ls", .age=25};
    PERSON wh = {.name="wh", .age=18};
    PERSON tt = {.name="tt", .age=20};

    PERSON expected_persons_sort_by_name[] = {
        {.name="ls", .age=25},
        {.name="tt", .age=20},
        {.name="wh", .age=18},
        {.name="zs", .age=22}
    };

    PERSON expected_persons_sort_by_age[] = {
        {.name="wh", .age=18},
        {.name="tt", .age=20},
        {.name="zs", .age=22},
        {.name="ls", .age=25}
    };

    expect_person_t ep;

    int_list_push_back(list, 5);
    int_list_push_back(list, 1);
    int_list_push_back(list, 3);
    int_list_push_back(list, 2);
    int_list_push_back(list, 4);

    int_list_sort(list);
    for (size_t i = 0; i < list_size(list); i++) {
        ck_assert_int_eq(i+1, *int_list_get(list, i));
    }

    list_free(list);


    list = data_t_list_new();
    
    data.data = 5;
    data_t_list_push_back(list, &data);

    data.data = 1;
    data_t_list_push_back(list, &data);

    data.data = 3;
    data_t_list_push_back(list, &data);

    data.data = 2;
    data_t_list_push_back(list, &data);

    data.data = 4;
    data_t_list_push_back(list, &data);

    data_t_list_sort(list);
    for (size_t i = 0; i < list_size(list); i++) {
        ck_assert_int_eq(i+1, data_t_list_get(list, i)->data);
    }

    list_free(list);


    list = person_t_list_new();
    
    person_t_list_push_back(list, &zs);
    person_t_list_push_back(list, &ls);
    person_t_list_push_back(list, &wh);
    person_t_list_push_back(list, &tt);

    ep.index = 0;
    ep.expect_persons = expected_persons_sort_by_name;
    person_t_list_sort(list);
    list_foreach(list, _ck_assert_persons, &ep);

    ep.index = 0;
    ep.expect_persons = expected_persons_sort_by_age;
    person_t_list_sort(list);
    person_t_list_sort_func(list, __person_age_cmp);

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

/*typedef struct {*/
    /*int *datas;*/
    /*int index;*/
/*} int_cursor_t;*/

/*static void _ck_assert_int_array(void *v, int_cursor_t *cursor)*/
/*{*/
    /*int expect_data = cursor->datas[cursor->index];*/
    /*ck_assert_int_eq(*(int*)v, expect_data);*/
/*}*/

START_TEST(test_bin_find)
{
    LIST *list = list_new(sizeof(int), NULL);
    int datas[] = {5, 2, 4, 10, 8, 100, 66, 77};
    /*int expect_datas[] = {2, 4, 5, 8, 10, 66, 77, 100};*/
    int n_datas[] = {76, 88};

    // 空值查找，保证不会粗错
    ck_assert(list_bin_find(list, &datas[0], CSTL_NUM_CMP_FUNC(int)) == NULL);

    // 单个值查找，保证，可以搜索正确
    list_push_back(list, &datas[0]);
    
    ck_assert_int_eq(1, list_size(list));
    ck_assert(list_bin_find(list, &n_datas[0], CSTL_NUM_CMP_FUNC(int)) == NULL);
    ck_assert(list_bin_find(list, &datas[0], CSTL_NUM_CMP_FUNC(int)) != NULL);

    // 两个值，查找，保证结果正确
    list_clear(list);
    ck_assert_int_eq(0, list_size(list));


    for (size_t i = 0; i < 2; i++) {
        list_push_back(list, &datas[i]);
    }
    list_sort(list, CSTL_NUM_CMP_FUNC(int));

    for (size_t i = 0; i < 2; i++) {
        ck_assert(list_bin_find(list, &datas[i], CSTL_NUM_CMP_FUNC(int)) != NULL);
    }

    for (size_t i = 0; i < ARRAY_SIZE(n_datas, int); i++) {
        ck_assert(list_bin_find(list, &n_datas[i], CSTL_NUM_CMP_FUNC(int)) == NULL);
    }
    
    // 多个值，查找，保证结果正确
    list_clear(list);
    ck_assert_int_eq(0, list_size(list));
    ck_assert_int_eq((size_t)-1, list_capacity(list));

    ck_assert_int_eq(8, (ARRAY_SIZE(datas, int)));
    for (size_t i = 0; i < ARRAY_SIZE(datas, int); i++) {
        list_push_back(list, &datas[i]);
    }
    ck_assert_int_eq(ARRAY_SIZE(datas, int), list_size(list));

    // 首先进行排序
    list_sort(list, CSTL_NUM_CMP_FUNC(int));

    // 正面测试
    for (size_t i = 0; i < ARRAY_SIZE(datas, int); i++) {
        ck_assert(list_bin_find(list, &datas[i], CSTL_NUM_CMP_FUNC(int)) != NULL);
    }

    // 负面测试
    for (size_t i = 0; i < ARRAY_SIZE(n_datas, int); i++) {
        ck_assert(list_bin_find(list, &n_datas[i], CSTL_NUM_CMP_FUNC(int)) == NULL);
    }

    list_free(list);
    ck_assert_no_leak();
}
END_TEST

static void int_ptr_destroy(int **elem)
{
    cstl_free(*elem);
}

START_TEST(test_destory_func) {
    LIST *list = list_new(sizeof(int*), (destroy_func_t)int_ptr_destroy);
    int *p;
    
    for (size_t i = 0; i < 100; i++) {
        p = cstl_malloc(sizeof(int));
        *p = i + 1;
        list_push_back(list, &p);
    }

    ck_assert_int_eq(1, **(int**)list_get(list, 0));

    list_free(list);
    ck_assert_no_leak(); // 只要断言成功，则说明每个元素的destroy函数都被调用了
}
END_TEST

START_DEFINE_SUITE(list)
    TEST(test_new)
    TEST(test_push_front)
    TEST(test_push_back)
    TEST(test_resize)
    TEST(test_pop_back)
    TEST(test_pop_front)
    TEST(test_front_back_get)
    TEST(test_find)
    TEST(test_insert)
    TEST(test_extend)
    TEST(test_set)
    TEST(test_erase)
    TEST(test_clear)
    TEST(test_remove)
    TEST(test_sort)
    TEST(test_swap)
    TEST(test_list_num_compare)
    TEST(test_list_type_macro)
    TEST(test_struct_list)
    TEST(test_bin_find)
    TEST(test_destory_func)
END_DEFINE_SUITE()

#undef ARRAY_SIZE
