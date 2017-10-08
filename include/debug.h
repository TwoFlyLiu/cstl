#ifndef INCLUDE_DEBUG_H_H
#define INCLUDE_DEBUG_H_H

#include "cstl_stddef.h"

CSTL_LIB void *__cstl_malloc(unsigned int size, const char *fname
        , int line_number, const char *expression);
CSTL_LIB void __cstl_free(void *mem);
CSTL_LIB int __cstl_leak_test(void);
CSTL_LIB int __cstl_no_leak(void);

#ifndef NDEBUG

#   define cstl_malloc(size) __cstl_malloc(size, __FILE__, __LINE__, #size)
#   define cstl_free(mem) __cstl_free(mem)
#   define cstl_leak_test() __cstl_leak_test() 

#else

#   define cstl_malloc(size) malloc(size)
#   define cstl_free(mem) free(mem)
#   define cstl_leak_test()  __cstl_no_leak()

#endif

#endif //INCLUDE/DEBUG_H_H
