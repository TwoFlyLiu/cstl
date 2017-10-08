/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 08 07:45:38
*/
#include <math.h>



// 定义有符号整数的比较函数
#define __CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC(type) \
    int __cstl_##type##_cmp_func(type *lhv, type *rhv) {\
        return (*lhv - *rhv);\
    }

// 定义无符号整数的比较函数
/*#define __CSTL_DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC(type) \*/
    /*int __cstl_unsigned_##type##_cmp_func(unsigned type *lhv, unsigned type *rhv) {\*/
        /*return (*lhv - *rhv);\*/
    /*}*/

// 定义小数的比较函数
#define __CSTL_DEFINE_FLOAT_NUM_CMP_FUNC(type) \
    int __cstl_##type##_cmp_func(type *lhv, type *rhv) {\
        type diff = *lhv - *rhv;\
        return (fabs(diff) < 1e-6) ? 0 : (int)diff;\
    }


__CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC(char)
__CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC(short)
__CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC(int)
__CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC(long)

__CSTL_DEFINE_FLOAT_NUM_CMP_FUNC(float)
__CSTL_DEFINE_FLOAT_NUM_CMP_FUNC(double)


// 定义常见的hash函数

#define __CSTL_DEFINE_SIGNED_NUM_HASH_FUNC(type) \
    unsigned int __cstl_##type##_hash_func(type *value) {\
        return (unsigned int)*value;\
    }        


__CSTL_DEFINE_SIGNED_NUM_HASH_FUNC(char)
__CSTL_DEFINE_SIGNED_NUM_HASH_FUNC(short)
__CSTL_DEFINE_SIGNED_NUM_HASH_FUNC(int)
__CSTL_DEFINE_SIGNED_NUM_HASH_FUNC(long)

unsigned int
__cstl_str_hash_func(char *value) {
    int hash_val = 0;
    while (*value) {
        hash_val += *value++;
    }
    return hash_val;
}

#if defined(__STDC__) && defined(__STDC_VERSION__) &&  (__STDC_VERSION__ > 199901L)

int __cstl_long_long_cmp_func(long long *lhv, long long *rhv)
{
    return *lhv - *rhv;
}

int
__cstl_long_long_hash_func(long long *value)
{
    return (int)*value;
}

#endif

void __cstl_destroy_func(void *value)
{
    value = NULL; //镇压编译器警告
}


#undef __CSTL_DEFINE_SIGNED_INTEGER_NUM_CMP_FUNC
/*#undef __CSTL_DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC*/
#undef __CSTL_DEFINE_FLOAT_NUM_CMP_FUNC
#undef __CSTL_DEFINE_FLOAT_NUM_CMP_FUNC

#undef __CSTL_DEFINE_SIGNED_NUM_HASH_FUNC
/*#undef __CSTL_DEFINE_UNSIGNED_NUM_HASH_FUNC*/
