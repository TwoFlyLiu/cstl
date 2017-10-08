/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 09 30 16:48:18
*/
#include <check_util.h>
#include <stdio.h>
#include <string.h>
#include "vec.h"

#define ARRAY_SIZE(array, type) ((int)(sizeof(array) / sizeof(type)))

START_TEST(test_new) {
    VEC *vec = vec_new(sizeof(int), NULL); //默认元素原谅是10
    ck_assert(vec != NULL);
    ck_assert_int_eq(vec_capacity(vec), 10);
    vec_free(vec);

    vec = vec_new_with_capacity(sizeof(int), NULL, 5);
    ck_assert_int_eq(vec_capacity(vec), 5);
    vec_free(vec);
}
END_TEST

START_TEST(test_push_back) {
    VEC *vec = vec_new(sizeof(int), NULL);

    ck_assert_int_eq(0, vec_size(vec));
    ck_assert(NULL == vec_get(vec, 0));

    int i = 100;
    vec_push_back(vec, &i);
    ck_assert_int_eq(100, *(int*)vec_get(vec, 0));

    i = 10;
    vec_push_back(vec, &i);
    ck_assert_int_eq(10, *(int*)vec_get(vec, 1));

    ck_assert_int_eq(2, vec_size(vec));

    vec_free(vec);
}
END_TEST

START_TEST(test_resize) {
    VEC *vec = vec_new_with_capacity(sizeof(int), NULL, 5);
    int datas[] = {3, 2, 1, 4, 6, 8, 19, 10};
    int default_value = 55;

    ck_assert_int_eq(5, vec_capacity(vec));
    ck_assert_int_eq(0, vec_size(vec));

    // 测试自动扩容
    for (int i = 0; i < ARRAY_SIZE(datas, int); i++) {
        vec_push_back(vec, &datas[i]);
    }

    ck_assert_int_eq(ARRAY_SIZE(datas, int), vec_size(vec));
    ck_assert_int_eq(10, vec_capacity(vec));

    for (int i = 0; i < ARRAY_SIZE(datas, int); i++) {
        ck_assert_int_eq(datas[i], *(int*)vec_get(vec, i));
    }

    // 缩小容量
    vec_resize(vec, 5, &default_value);
    ck_assert_int_eq(10, vec_capacity(vec));
    ck_assert_int_eq(5, vec_size(vec));

    for (int i = 0; i < 5; i++) {
        ck_assert_int_eq(datas[i], *(int*)vec_get(vec, i));
    }

    // 再次扩容
    vec_push_back(vec, &datas[5]);
    ck_assert_int_eq(10, vec_capacity(vec));
    ck_assert_int_eq(6, vec_size(vec));

    for (int i = 0; i < 6; i++) {
        ck_assert_int_eq(datas[i], *(int*)vec_get(vec, i));
    }

    // 使用resize来扩大容量
    vec_resize(vec, 100, &default_value);
    ck_assert_int_eq(100, vec_size(vec));
    ck_assert_int_eq(default_value, *(int*)vec_get(vec, 99));

    vec_free(vec);
}
END_TEST

START_TEST(test_pop_front) {
    VEC *vec = vec_new(sizeof(int), NULL);

    vec_pop_front(vec); //测试空 弹出首元素

    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    ck_assert_int_eq(0, *(int*)vec_front(vec));
    ck_assert_int_eq(9, *(int*)vec_back(vec));
    
    vec_pop_front(vec);
    ck_assert_int_eq(1, *(int*)vec_front(vec));
    ck_assert_int_eq(2, *(int*)vec_get(vec, 1));
    ck_assert_int_eq(9, *(int*)vec_back(vec));

    ck_assert_int_eq(9, vec_size(vec));
    ck_assert_int_eq(9, *(int*)vec_get(vec, vec_size(vec) - 1));

    vec_free(vec);
}
END_TEST

START_TEST(test_pop_back) {
    VEC *vec = vec_new(sizeof(int), NULL);
    
    ck_assert(vec_empty(vec));

    vec_pop_back(vec); // 当vec为空的时候，要保证不会出错

    for (int i = 0; i < 5; i++) {
        vec_push_back(vec, &i);
    }

    ck_assert(!vec_empty(vec));
    ck_assert_int_eq(5, vec_size(vec));
    ck_assert_int_eq(10, vec_capacity(vec)); //容量没有发生变化
    ck_assert_int_eq(4, *(int*)vec_get(vec, vec_size(vec) - 1));

    vec_pop_back(vec);
    ck_assert_int_eq(4, vec_size(vec));
    ck_assert_int_eq(10, vec_capacity(vec)); //容量没有发生变化

    vec_free(vec);
}
END_TEST

START_TEST(test_front_back_get) {
    VEC *vec = vec_new(sizeof(int), NULL);
    int i = 100;

    ck_assert(NULL == vec_front(vec));
    ck_assert(NULL == vec_back(vec));
    ck_assert(NULL == vec_get(vec, 10000));

    vec_push_back(vec, &i);

    ck_assert_int_eq(100, *(int*)vec_front(vec));
    ck_assert_int_eq(100, *(int*)vec_back(vec));
    ck_assert_int_eq(100, *(int*)vec_get(vec, 0));
    ck_assert_int_eq(100, *(int*)vec_get(vec, vec_size(vec) - 1));


    i = 50;
    vec_push_back(vec, &i);

    ck_assert_int_eq(100, *(int*)vec_front(vec));
    ck_assert_int_eq(50, *(int*)vec_back(vec));
    ck_assert_int_eq(100, *(int*)vec_get(vec, 0));
    ck_assert_int_eq(50, *(int*)vec_get(vec, vec_size(vec) - 1));

    vec_free(vec); 
}
END_TEST

START_TEST(test_push_front) {
    VEC *vec = vec_new(sizeof(int), NULL);
    int elem = 100;

    ck_assert(vec_empty(vec));
    
    vec_push_front(vec, &elem);
    ck_assert(!vec_empty(vec));
    ck_assert_int_eq(1, vec_size(vec));
    ck_assert_int_eq(100, *(int*)vec_front(vec));
    ck_assert_int_eq(100, *(int*)vec_back(vec));

    elem = 200;
    vec_push_front(vec, &elem);
    ck_assert_int_eq(200, *(int*)vec_front(vec));
    ck_assert_int_eq(100, *(int*)vec_back(vec));

    elem = 300;
    vec_push_front(vec, &elem);
    ck_assert_int_eq(300, *(int*)vec_front(vec));
    ck_assert_int_eq(100, *(int*)vec_back(vec));
    ck_assert_int_eq(3, vec_size(vec));

    vec_free(vec);

    vec = vec_new(sizeof(int), NULL);

    for (int i = 0; i < 100; i++) {
        vec_push_front(vec, &i);
    }

    for (int i = 0; i < 100; i++) {
        ck_assert_int_eq(99 - i, *(int*)vec_get(vec, i));
    }

    vec_free(vec);
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
    VEC *vec = vec_new(sizeof(POINT), NULL);
    POINT pt = {.x = 1, .y = 2};
    POINT to_find = {5, 6};

    // 空容器查找
    ck_assert(NULL == vec_find(vec, NULL, __point_compare));

    // 有值时候
    vec_push_back(vec, &pt);
    ck_assert(NULL == vec_find(vec, &to_find, __point_compare));

    vec_push_back(vec, &to_find);
    ck_assert(NULL != vec_find(vec, &to_find, __point_compare));

    vec_free(vec);
}
END_TEST

START_TEST(test_insert) {
    int elem = 100;
    int old_back;

    // 测试空值插入
    VEC *vec = vec_new(sizeof(int), NULL);
    vec_insert(vec, 0, &elem);
    ck_assert_int_eq(100, *(int*)vec_front(vec));
    vec_free(vec);

    vec = vec_new(sizeof(int), NULL);
    vec_insert(vec, 100, &elem); //在无效位置插入，会变成在尾部插入
    ck_assert_int_eq(100, *(int*)vec_back(vec));
    vec_free(vec);

    // 测试有值插入
    vec = vec_new(sizeof(int), NULL);

    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    for (int i = 0; i < 10; i++) {
        ck_assert_int_eq(i, *(int*)vec_get(vec, i));
    }

    vec_insert(vec, 5, &elem);
    ck_assert_int_eq(elem, *(int*)vec_get(vec, 5));

    // 因为从原来的索引5开始，值都向后移动了一位，所以向下面这样进行测试
    for (int i = 6; i < 11; i++) {
        ck_assert_int_eq(i - 1, *(int*)vec_get(vec, i));
    }

    // 单独测试有值时候，在行首和行尾插入效果
    elem = 1000;
    vec_insert(vec, 0, &elem);
    ck_assert_int_eq(elem, *(int*)vec_front(vec));

    elem = 500;
    vec_insert(vec, 1000, &elem);
    ck_assert_int_eq(elem, *(int*)vec_back(vec));

    old_back = *(int*)vec_back(vec);
    
    // 测试在最后一个元素位置上面插入（边界情况）
    elem = 55;
    vec_insert(vec, vec_size(vec) - 1, &elem);
    ck_assert_int_eq(old_back, *(int*)vec_back(vec));
    ck_assert_int_eq(elem, *(int*)vec_get(vec, vec_size(vec) - 2));

    vec_free(vec);
}
END_TEST

START_TEST(test_extend) {
    // 先测试在末尾插入
    VEC *vec = vec_new(sizeof(int), NULL); 
    VEC *insert_vec = vec_new(sizeof(int), NULL);

    for (int i = 0; i < 20; i++) {
        vec_push_back(insert_vec, &i);
    }

    ck_assert_int_eq(20, vec_capacity(insert_vec));
    ck_assert_int_eq(20, vec_size(insert_vec));

    vec_extend(vec, vec_size(vec), insert_vec);

    for (int i = 0; i < 20; i++) {
        ck_assert_int_eq(i, *(int *)vec_get(vec, i));
    }

    vec_free(vec);

    vec = vec_new(sizeof(int), NULL);
    for (int i = 100; i < 1000; i += 50) {
        vec_push_back(vec, &i);
    }

    // 在非位置位置插入值
    // 
    // 在头部插入值
    vec_extend(vec, 0, insert_vec);
    ck_assert_int_eq(0, *(int*)vec_front(vec));
    ck_assert_int_eq(100, *(int*)vec_get(vec, 20));

    vec_free(vec);

    // 在最后一个元素位置插入值(相当于在中间插入)
    vec = vec_new(sizeof(int), NULL);
    for (int i = 100; i < 1000; i += 50) {
        vec_push_back(vec, &i);
    }

    vec_extend(vec, vec_size(vec) - 1, insert_vec);
    ck_assert_int_eq(950, *(int*)vec_back(vec));
    ck_assert_int_eq(19, *(int*)vec_get(vec, vec_size(vec) - 2));
    ck_assert_int_eq(0, *(int*)vec_get(vec, vec_size(vec) - 21));
    ck_assert_int_eq(38, vec_size(vec));

    vec_free(vec);

    // 在中间插入一个元素
    vec = vec_new(sizeof(int), NULL);
    for (int i = 100; i < 1000; i += 50) {
        vec_push_back(vec, &i);
    }

    vec_extend(vec, 2, insert_vec);
    ck_assert_int_eq(38, vec_size(vec));
    ck_assert_int_eq(0, *(int*)vec_get(vec, 2));

    vec_free(vec);

    vec_free(insert_vec);
}
END_TEST

START_TEST(test_assign) {
    VEC *vec = vec_new(sizeof(int), NULL);
    int data = 100;

    vec_push_back(vec, &data);
    ck_assert_int_eq(data, *(int*)vec_get(vec, 0));

    data = 200;
    vec_assign(vec, 0, &data);
    ck_assert_int_eq(data, *(int*)vec_get(vec, 0));

    vec_free(vec);
}
END_TEST

START_TEST(test_erase) {
    VEC *vec = vec_new(sizeof(int), NULL);
    
    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    // 移除头部
    vec_erase(vec, 0);
    ck_assert_int_eq(1, *(int*)vec_front(vec));
    ck_assert_int_eq(9, vec_size(vec));

    // 移除尾部
    vec_erase(vec, vec_size(vec) - 1);
    ck_assert_int_eq(8, *(int*)vec_back(vec));
    ck_assert_int_eq(8, vec_size(vec));

    /*// 移除中间元素*/
    vec_erase(vec, 1);
    ck_assert_int_eq(3, *(int*)vec_get(vec, 1));
    ck_assert_int_eq(7, vec_size(vec));

    vec_erase(vec, 1);
    ck_assert_int_eq(4, *(int*)vec_get(vec, 1));
    ck_assert_int_eq(6, vec_size(vec));

    vec_free(vec);

    vec = vec_new(sizeof(int), NULL);

    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    ck_assert_int_eq(0, *(int*)vec_get(vec, 0));
    ck_assert_int_eq(1, *(int*)vec_get(vec, 1));
    ck_assert_int_eq(2, *(int*)vec_get(vec, 2));
    ck_assert_int_eq(3, *(int*)vec_get(vec, 3));
    ck_assert_int_eq(4, *(int*)vec_get(vec, 4));
    ck_assert_int_eq(5, *(int*)vec_get(vec, 5));
    ck_assert_int_eq(6, *(int*)vec_get(vec, 6));
    ck_assert_int_eq(7, *(int*)vec_get(vec, 7));
    ck_assert_int_eq(8, *(int*)vec_get(vec, 8));
    ck_assert_int_eq(9, *(int*)vec_get(vec, 9));

    vec_erase(vec, 8);
    ck_assert_int_eq(9, *(int*)vec_get(vec, 8));
    ck_assert_int_eq(9, vec_size(vec));

    vec_free(vec);
}
END_TEST

START_TEST(test_clear) {
    VEC *vec = vec_new_with_capacity(sizeof(int), NULL, 5);

    // 删除空的(保证不出错)
    vec_clear(vec);
    ck_assert_int_eq(0, vec_size(vec));
    ck_assert_int_eq(10, vec_capacity(vec));

    // 有值删除
    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    ck_assert_int_eq(10, vec_size(vec));

    vec_clear(vec);
    ck_assert(vec_empty(vec));

    // 有值删除(测试第二次，是为了确定调用clear后，vec还是有效的)
    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    ck_assert_int_eq(10, vec_size(vec));

    vec_clear(vec);
    ck_assert(vec_empty(vec));

    vec_free(vec);
}
END_TEST

START_TEST(test_remove) {
    VEC *vec = vec_new(sizeof(int), NULL);
    int data = 8;

    for (int i = 0; i < 10; i++) {
        vec_push_back(vec, &i);
    }

    // 没有移除之前可以找到
    ck_assert(NULL != vec_find(vec, &data, CSTL_NUM_CMP_FUNC(int)));

    // 移除之后就不能找到了
    vec_remove(vec, &data, CSTL_NUM_CMP_FUNC(int));
    ck_assert(NULL == vec_find(vec, &data, CSTL_NUM_CMP_FUNC(int)));

    vec_free(vec);
}
END_TEST

START_TEST(test_sort) {
    VEC *vec = vec_new(sizeof(int), NULL);

    int data = 5;
    vec_push_back(vec, &data);

    data = 1;
    vec_push_back(vec, &data);

    data = 3;
    vec_push_back(vec, &data);

    data = 2;
    vec_push_back(vec, &data);

    data = 4;
    vec_push_back(vec, &data);

    vec_sort(vec, CSTL_NUM_CMP_FUNC(int));

    for (int i = 0; i < vec_size(vec); i++) {
        ck_assert_int_eq(i+1, *(int*)vec_get(vec, i));
    }

    vec_free(vec);
}
END_TEST

START_TEST(test_swap) {
    VEC *vec = vec_new(sizeof(int), NULL);
    int data = 1;

    vec_push_back(vec, &data);

    data = 2;
    vec_push_back(vec, &data);

    ck_assert_int_eq(1, *(int*)vec_get(vec, 0));
    ck_assert_int_eq(2, *(int*)vec_get(vec, 1));

    vec_swap(vec, 0, 1);
    ck_assert_int_eq(2, *(int*)vec_get(vec, 0));
    ck_assert_int_eq(1, *(int*)vec_get(vec, 1));

    data = 3;
    vec_push_back(vec, &data);

    vec_swap(vec, 1, 2);
    ck_assert_int_eq(3, *(int*)vec_get(vec, 1));
    ck_assert_int_eq(1, *(int*)vec_get(vec, 2));

    vec_free(vec);
}
END_TEST


START_TEST(test_vec_num_compare) {
    cmp_func_t compare = CSTL_NUM_CMP_FUNC(int);
    int num1 = 1, num2 = 2;

    ck_assert_int_eq((*compare)(&num1, &num2), -1);
    ck_assert_int_eq((*compare)(&num2, &num1), 1);
    ck_assert((*compare)(&num1, &num1) == 0);
    ck_assert((*compare)(&num1, &num2) <= 0);
    ck_assert((*compare)(&num1, &num1) <= 0);
}
END_TEST

START_TEST(test_vec_type_macro) {
    VEC *vec = int_vec_new();
    int data = 10;
    
    vec_push_back(vec, &data);
    ck_assert_int_eq(10, *int_vec_get(vec, 0));
    ck_assert_int_eq(10, *int_vec_at(vec, 0));
    ck_assert(NULL == int_vec_get(vec, 100));
    ck_assert(NULL == int_vec_at(vec, 100));

    data = 20;
    vec_push_back(vec, &data);

    ck_assert_int_eq(10, *int_vec_front(vec));
    ck_assert_int_eq(20, *int_vec_back(vec));

    ck_assert(int_vec_find(vec, 10) != NULL);


    int_vec_push_front(vec, 50);
    ck_assert_int_eq(50, *int_vec_front(vec));

    int_vec_push_back(vec, 1000);
    ck_assert_int_eq(1000, *int_vec_back(vec));

    int_vec_insert(vec, 2, 22);
    ck_assert_int_eq(22, *int_vec_get(vec, 2));

    int_vec_assign(vec, 2, 50);
    ck_assert_int_eq(50, *int_vec_get(vec, 2));

    int_vec_remove(vec, 1000);
    ck_assert(int_vec_find(vec, 1000) == NULL);

    vec_free(vec);    

    vec = int_vec_new();
    
    int_vec_push_back(vec, 4);
    int_vec_push_back(vec, 1);
    int_vec_push_back(vec, 3);
    int_vec_push_back(vec, 5);
    int_vec_push_back(vec, 2);

    int_vec_sort(vec);

    for (int i = 0; i < vec_size(vec); i++) {
        ck_assert_int_eq(i+1, *int_vec_get(vec, i));
    }

    int_vec_resize(vec, 10000, 5);
    ck_assert_int_eq(10000, vec_size(vec));
    ck_assert_int_eq(5, *int_vec_get(vec, 9999));

    vec_free(vec);

    
    // 无符号数值类型测试
    vec = unsigned_int_vec_new();

    unsigned_int_vec_push_back(vec, -10);
    ck_assert_uint_eq((unsigned int)-10, *unsigned_int_vec_front(vec));
    vec_free(vec);

    vec = double_vec_new();

    double_vec_push_back(vec, 3.14);
    ck_assert(double_vec_find(vec, 3.140000000002) != NULL);

    vec_free(vec);
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

DEFINE_STRUCT_TYPE_VEC(data_t, DATA_CMP)

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

DEFINE_STRUCT_TYPE_VEC(person_t, __person_name_cmp)

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

START_TEST(test_struct_vec) {
    VEC *vec = int_vec_new();
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

    int_vec_push_back(vec, 5);
    int_vec_push_back(vec, 1);
    int_vec_push_back(vec, 3);
    int_vec_push_back(vec, 2);
    int_vec_push_back(vec, 4);

    int_vec_sort(vec);
    for (int i = 0; i < vec_size(vec); i++) {
        ck_assert_int_eq(i+1, *int_vec_get(vec, i));
    }

    vec_free(vec);


    vec = data_t_vec_new();
    
    data.data = 5;
    data_t_vec_push_back(vec, &data);

    data.data = 1;
    data_t_vec_push_back(vec, &data);

    data.data = 3;
    data_t_vec_push_back(vec, &data);

    data.data = 2;
    data_t_vec_push_back(vec, &data);

    data.data = 4;
    data_t_vec_push_back(vec, &data);

    data_t_vec_sort(vec);
    for (int i = 0; i < vec_size(vec); i++) {
        ck_assert_int_eq(i+1, data_t_vec_get(vec, i)->data);
    }

    vec_free(vec);


    vec = person_t_vec_new();
    
    person_t_vec_push_back(vec, &zs);
    person_t_vec_push_back(vec, &ls);
    person_t_vec_push_back(vec, &wh);
    person_t_vec_push_back(vec, &tt);

    ep.index = 0;
    ep.expect_persons = expected_persons_sort_by_name;
    person_t_vec_sort(vec);
    vec_foreach(vec, _ck_assert_persons, &ep);

    ep.index = 0;
    ep.expect_persons = expected_persons_sort_by_age;
    person_t_vec_sort(vec);
    person_t_vec_sort_func(vec, __person_age_cmp);

    vec_free(vec);
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
    VEC *vec = vec_new(sizeof(int), NULL);
    int datas[] = {5, 2, 4, 10, 8, 100, 66, 77};
    /*int expect_datas[] = {2, 4, 5, 8, 10, 66, 77, 100};*/
    int n_datas[] = {76, 88};

    // 空值查找，保证不会粗错
    ck_assert(vec_bin_find(vec, &datas[0], CSTL_NUM_CMP_FUNC(int)) == NULL);

    // 单个值查找，保证，可以搜索正确
    vec_push_back(vec, &datas[0]);
    
    ck_assert_int_eq(1, vec_size(vec));
    ck_assert(vec_bin_find(vec, &n_datas[0], CSTL_NUM_CMP_FUNC(int)) == NULL);
    ck_assert(vec_bin_find(vec, &datas[0], CSTL_NUM_CMP_FUNC(int)) != NULL);

    // 两个值，查找，保证结果正确
    vec_clear(vec);
    ck_assert_int_eq(0, vec_size(vec));


    for (int i = 0; i < 2; i++) {
        vec_push_back(vec, &datas[i]);
    }
    vec_sort(vec, CSTL_NUM_CMP_FUNC(int));

    for (int i = 0; i < 2; i++) {
        ck_assert(vec_bin_find(vec, &datas[i], CSTL_NUM_CMP_FUNC(int)) != NULL);
    }

    for (int i = 0; i < ARRAY_SIZE(n_datas, int); i++) {
        ck_assert(vec_bin_find(vec, &n_datas[i], CSTL_NUM_CMP_FUNC(int)) == NULL);
    }
    
    // 多个值，查找，保证结果正确
    vec_clear(vec);
    ck_assert_int_eq(0, vec_size(vec));
    ck_assert_int_eq(10, vec_capacity(vec));

    ck_assert_int_eq(8, (ARRAY_SIZE(datas, int)));
    for (int i = 0; i < ARRAY_SIZE(datas, int); i++) {
        vec_push_back(vec, &datas[i]);
    }
    ck_assert_int_eq(ARRAY_SIZE(datas, int), vec_size(vec));

    // 首先进行排序
    vec_sort(vec, CSTL_NUM_CMP_FUNC(int));

    // 正面测试
    for (int i = 0; i < ARRAY_SIZE(datas, int); i++) {
        ck_assert(vec_bin_find(vec, &datas[i], CSTL_NUM_CMP_FUNC(int)) != NULL);
    }

    // 负面测试
    for (int i = 0; i < ARRAY_SIZE(n_datas, int); i++) {
        ck_assert(vec_bin_find(vec, &n_datas[i], CSTL_NUM_CMP_FUNC(int)) == NULL);
    }

    vec_free(vec);
}
END_TEST

START_DEFINE_SUITE(vec)
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
    TEST(test_assign)
    TEST(test_erase)
    TEST(test_clear)
    TEST(test_remove)
    TEST(test_sort)
    TEST(test_swap)
    TEST(test_vec_num_compare)
    TEST(test_vec_type_macro)
    TEST(test_struct_vec)
    TEST(test_bin_find)
END_DEFINE_SUITE()

#undef ARRAY_SIZE
