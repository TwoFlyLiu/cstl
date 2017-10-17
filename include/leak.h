#ifndef INCLUDE_LEAK_H_H
#define INCLUDE_LEAK_H_H

#include "cstl_stddef.h"

CSTL_LIB void *__cstl_malloc(unsigned int size, const char *fname
        , int line_number, const char *expression);
CSTL_LIB void __cstl_free(void *mem);
CSTL_LIB int __cstl_leak_test(bool verbose);

// 启用/禁用测试
CSTL_LIB void __cstl_leak_auto_test(bool enable);

#ifndef NDEBUG

#   define cstl_malloc(size) __cstl_malloc(size, __FILE__, __LINE__, #size)
#   define cstl_free(mem) __cstl_free(mem)
#   define cstl_leak_test(verbose) __cstl_leak_test(verbose) 
#   define cstl_leak_enable_auto_test() __cstl_leak_auto_test(true) 
#   define cstl_leak_disable_auto_test() __cstl_leak_auto_test(false) 

#else

#   define cstl_malloc(size) malloc(size)
#   define cstl_free(mem) free(mem)

    // 返回没有意义的函数
    static inline  int cstl_leak_test(bool verbose) {
        return (int)(verbose = 0);
    }

#   define cstl_leak_enable_auto_test()
#   define cstl_leak_disable_auto_test()

#endif

#endif //INCLUDE/DEBUG_H_H
