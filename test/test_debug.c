/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 08 18:19:50
*/
#include <check_util.h>

// 强制开启调试功能
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "debug.h"

// 检测内存分配是否成功
START_TEST(test_cstl_malloc) {
    int *d = (int*)cstl_malloc(sizeof(int));
    *d = 10;
    ck_assert_int_eq(10, *d);
}
END_TEST

START_TEST(test_cstl_free) {
    int *d;

    cstl_free(NULL); //对于空值free根据表示是不算出错的

    d = (int*)cstl_malloc(sizeof(int));
    cstl_free(d); //对正常值进行free
}
END_TEST

START_TEST(test_cstl_leak_test) {
    int *d;
    int i;

    ck_assert_int_eq(0, cstl_leak_test());

    d = cstl_malloc(sizeof(int));
    ck_assert_int_eq(1, cstl_leak_test());

    cstl_free(d);
    ck_assert_int_eq(0, cstl_leak_test());

    for (i = 0; i < 10; i++) {
        cstl_malloc(sizeof(int));
    }
    ck_assert_int_eq(10, cstl_leak_test());
}
END_TEST

START_DEFINE_SUITE(debug)
    TEST(test_cstl_malloc)
    TEST(test_cstl_free)
    TEST(test_cstl_leak_test)
END_DEFINE_SUITE()
