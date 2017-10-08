#ifndef INCLUDE_DEBUG_H_H
#define INCLUDE_DEBUG_H_H

#include "cstl_stddef.h"

CSTL_LIB void *__cstl_malloc(unsigned int size, const char *fname
        , int line_number, const char *expression);
CSTL_LIB void __cstl_free(void *mem);
CSTL_LIB int __cstl_leak_test(int verbose);
CSTL_LIB int __cstl_no_leak(int verbose);

#ifndef NDEBUG

#   define cstl_malloc(size) __cstl_malloc(size, __FILE__, __LINE__, #size)
#   define cstl_free(mem) __cstl_free(mem)
#   define cstl_leak_test(verbose) __cstl_leak_test(verbose) 

#else

#   define cstl_malloc(size) malloc(size)
#   define cstl_free(mem) free(mem)
#   define cstl_leak_test(verbose)  __cstl_no_leak(verbose)

#endif

#endif //INCLUDE/DEBUG_H_H
