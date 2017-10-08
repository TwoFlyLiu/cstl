#ifndef __CSTL_STDDEF_H_H
#define __CSTL_STDDEF_H_H

// 该函数定义了三种函数指针类型
// 
// 几个辅助宏
// 
// 常用数值类型比较函数指针
// CSTL_NUM_CMP_FUNC(num_type)  不带符号的
// CSTL_UNSIGNED_CMP_FUNC(num_type) 带有符号的
// 
// c99
// CSTL_LLONG_CMP_FUNC 类型是long long
// CSTL_ULLONG_CMP_FUNC  类型是unsigned long long
// 
// 
// 常用的数值类型hash计算函数指针
// CSTL_NUM_HASH_FUNC
// CSTL_UNSIGNED_HASH_FUNC
// 
// c99
// CSTL_LLONG_HASH_FUNC
// CSTL_ULLONG_HASH_FUNC
// 
// 一个空的destroy函数指针
// CSTL_NULL_DESTROY_FUNC 

#include <math.h>
#include <stdlib.h>

// 比较lhv, rhv
// *lhv == *rhv -> 0
// *lhv > *rhv -> >0
// *lhv < *rhv -> <0
typedef int (*cmp_func_t)(void *lhv, void *rhv);

// 计算val的hash值
typedef unsigned int (*hash_func_t)(void *val);

// 销毁value的函数(按照c++说法， 此函数是value的析构函数)
typedef void (*destroy_func_t)(void *value);


// 定义函数的引用宏
#define CSTL_NUM_CMP_FUNC(type) (&((cmp_func_t)__cstl_##type##_cmp_func))
//#define CSTL_UNSIGNED_CMP_FUNC(type) (&((cmp_func_t)__cstl_unsigned_##type##_cmp_func))
#define CSTL_UNSIGNED_CMP_FUNC(type) CSTL_NUM_CMP_FUNC(type)


//DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC(char)
//DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC(short)
//DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC(int)
//DEFINE_UNSIGNED_INTEGER_NUM_CMP_FUNC(long)

//#define DEFINE_UNSIGNED_NUM_HASH_FUNC(type) \
    //static unsigned int __cstl_unsigned_##type##_hash_func(unsigned type *value) {\
        //return (unsigned int)*value;\
    //}        

#define CSTL_NUM_HASH_FUNC(type) (&((hash_func_t)__cstl_##type##_hash_func))
//#define CSTL_UNSIGNED_HASH_FUNC (&((hash_func_t)__cstl_unsigned_##type##_hash_func))
#define CSTL_UNSIGNED_HASH_FUNC(type) CSTL_NUM_HASH_FUNC(type)


//CSTL_UNSIGNED_HASH_FUNC(char)
//CSTL_UNSIGNED_HASH_FUNC(short)
//CSTL_UNSIGNED_HASH_FUNC(int)
//CSTL_UNSIGNED_HASH_FUNC(long)

#define CSTL_STR_HASH_FUNC (&__cstl_str_hash_func)

// 主要是为了兼容非c99的编译器
// 表示支持c99标准
#if defined(__STDC__) && defined(__STDC_VERSION__) &&  (__STDC_VERSION__ > 199901L)
#include <stdbool.h>

#define CSTL_LLONG_CMP_FUNC (&((cmp_func_t)__cstl_long_long_cmp_func))
#define CSTL_ULLONG_CMP_FUNC CSTL_LLONG_CMP_FUNC

#define CSTL_LLONG_HASH_FUNC (&((hash_func_t)__cstl_long_long_hash_func))
#define CSTL_ULLONG_HASH_FUNC CSTL_LLONG_HASH_FUNC
#else
#define bool int
#define true 1
#define false 0
#define inline
#endif

// 定义空的destroy函数
#define CSTL_NULL_DESTROY_FUNC (&__cstl_destroy_func)

// 为了兼容c++，定义前缀extern "c"
#if defined(__cplusplus)
#define CSTL_LIB extern "C"
#else
#define CSTL_LIB
#endif

// 先预留一个宏，用来以后支持gcc以外的编译器
// 这儿当且是非gcc编译器，动态库时候可能有用
#define CSTL_EXPORT

#endif //STDDEF_H_H
