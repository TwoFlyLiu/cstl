#ifndef TEST_TEST_COMMON_H_H
#define TEST_TEST_COMMON_H_H

#ifdef NDEBUG
#   undef NDEBUG
#endif

#include "leak.h"


#define ck_assert_no_leak() \
    ck_assert_int_eq(0, cstl_leak_test(false))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array, type) (sizeof(array) / sizeof(type))
#endif

#endif //TEST/TEST_COMMON_H_H
