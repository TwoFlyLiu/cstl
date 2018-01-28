#ifndef TEST_TEST_COMMON_H_H
#define TEST_TEST_COMMON_H_H

#ifdef NDEBUG
#   undef NDEBUG
#endif

#include <wchar.h>
#include <locale.h>
#include "leak.h"


#define ck_assert_no_leak() \
    ck_assert_int_eq(0, cstl_leak_test(false))

#define ck_assert_wstr_eq(lhv, rhv) do {\
        int lhv_len = wcslen(lhv) * 3;\
        char lhv_buf[lhv_len + 1];\
        int rhv_len = wcslen(rhv) * 3;\
        char rhv_buf[rhv_len + 1];\
        \
        char *old_locale = setlocale(LC_ALL, NULL);\
        setlocale(LC_ALL, "");\
        int end = wcstombs(lhv_buf, lhv, lhv_len);\
        *(lhv_buf + end) = '\0';\
        end = wcstombs(rhv_buf, rhv, rhv_len);\
        *(rhv_buf + end) = '\0';\
        ck_assert_msg(0 == wcscmp(lhv, rhv), "%s!=%s, (%s)=(%s), (%s)=(%s)", #lhv, #rhv, #lhv, lhv_buf, #rhv, rhv_buf);\
        setlocale(LC_ALL, old_locale);\
    } while(0)


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array, type) (int)(sizeof(array) / sizeof(type))
#endif

#endif //TEST/TEST_COMMON_H_H
