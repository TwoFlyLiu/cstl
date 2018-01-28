#ifndef CHECK_UTIL_H_H
#define CHECK_UTIL_H_H

#include <check.h>
#include <stdlib.h>
#include <stdio.h>

// 当宏中出现#或者##, 那么当出现嵌套宏的时候，宏内参数不会自动展开
// 解决方法是使用包装，就是让直接宏定义中不出现#或者##，这样宏就会自动展开了

// 下面如果直接使用_CAT2的话，那么当lhs, rhs两者有一个是宏的话，那么输出结果是没有展开的输出结果
#define _CAT2(lhs,rhs) lhs##_##rhs

//如何lhs, 和rhs有一个是宏的话，那么下面的版本，就会将lhs, rhs自动展开来
#define CAT2(lhs, rhs) _CAT2(lhs, rhs)

#define _STR(name) #name

// name会被自动展开(当展开的时候，就会是递归展开)
#define STR(name) _STR(name)

#define SUITE_NAME(name) STR(_CAT2(name, suite))
#define TCASE_NAME(name) STR(_CAT2(name, tcase))
//#define SUITE_FUNC(name) STR(CAT3(make, name, suite))

// 定义构建suite函数，因为check中非常常用，所以使用宏来进行包装生成
// 方便下次直接使用



// 辅助生成suite的工厂函数
// 并且为每个tcase设置了tags
#define START_DEFINE_TESTS(name) \
    static void __tcase_add_tests(TCase *tcase); \
    \
    Suite * make_##name##_suite(void) {\
        Suite *suite = suite_create(SUITE_NAME(name));\
        TCase *tcase = tcase_create(TCASE_NAME(name));\
        tcase_set_tags(tcase, #name);\
        \
        suite_add_tcase(suite, tcase);\
        __tcase_add_tests(tcase);\
        return suite;\
    }\
    \
    static void __tcase_add_tests(TCase *tcase) {\

#define END_DEFINE_TESTS() }    

// 定义下面两个函数，不适用SUITE_NAME()和TCASE_NAME()避免不必要的宏展开
static inline const char *
__suite_name(const char *name)
{
    static char buf[BUFSIZ];
    sprintf(buf, "%s_suite", name);
    return buf;
}

static inline const char *
__tcase_name(const char *name)
{
    static char buf[BUFSIZ];
    sprintf(buf, "%s_tcase", name);
    return buf;
}

// 和上面内容是一样的，但是不能写成
// #define START_DEFINE_SUITE(name) START_DEFINE_TESTS(name)
// 因为#和##的宏展开问题
// 
// 为了解决宏展开问题，可以合成字符
#define START_DEFINE_SUITE(name) \
    static void __tcase_add_tests(TCase *tcase); \
    \
    Suite * make_##name##_suite(void) {\
        Suite *suite = suite_create(__suite_name(#name));\
        TCase *tcase = tcase_create(__tcase_name(#name));\
        tcase_set_tags(tcase, #name);\
        \
        suite_add_tcase(suite, tcase);\
        __tcase_add_tests(tcase);\
        return suite;\
    }\
    \
    static void __tcase_add_tests(TCase *tcase) {\

#define END_DEFINE_SUITE() }    

#define DECLARE_SUITE(name) \
    Suite * make_##name##_suite(void)

#define MAKE_SUITE(name) make_##name##_suite()

#define TEST(name) tcase_add_test(tcase, name);

//main函数包装宏

#define START_CHECK_MAIN(master_name) int main(void) { \
    SRunner *srunner; \
    Suite *suite; \
    int res; \
    \
    suite = suite_create(#master_name); \
    \
    srunner = srunner_create(suite); 

//srunner_run_all 第二个参数可以是下面参数(用来设置输出的测试格式)
//CK_SILENT
//CK_MINIMAL
//CK_NORMAL
//CK_VERBOSE
//CK_ENV
//CK_SUBUNIT
#define END_CHECK_MAIN() \
    srunner_run_all(srunner, CK_ENV);\
    \
    res = srunner_ntests_failed(srunner);\
    srunner_free(srunner);\
    \
    return (res == 0) ? EXIT_SUCCESS : EXIT_FAILURE;\
    }

// 当测试执行完毕后，调用after函数，参数是测试的返回值
#define END_CHECK_MAIN_AFTER(after) \
    srunner_run_all(srunner, CK_ENV);\
    \
    res = srunner_ntests_failed(srunner);\
    srunner_free(srunner);\
    \
    return (after(res)== 0) ? EXIT_SUCCESS : EXIT_FAILURE;\
    }

#define SUITE(name) srunner_add_suite(srunner, make_##name##_suite());

#endif //CHECK_UTIL_H_H
