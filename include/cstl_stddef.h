/*!
 * \file cstl_stddef.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于utf8字符串的常见操作
 * 
 * 这个头文件定义了一些全局的宏，用来辅助函数的声明和定义。
 * 
 * 定义了三种函数指针类型，和一些默认的函数实现，以及引用这些函数的辅助宏。
 */

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


// 为了兼容c++，定义前缀extern "c"
#if defined(__cplusplus)
#define CSTL_LIB extern "C"
#else
#define CSTL_LIB
#endif

// 先预留一个宏，用来以后支持gcc以外的编译器
// 这儿当且是非gcc编译器，动态库时候可能有用
#define CSTL_EXPORT

#include <math.h>
#include <stdlib.h>

/*!
 *  \brief 一个比较函数类型
 *  \param [in] lhv 左值指针
 *  \param [in] rhv 右值指针
 *  \retval 如果*lhv == *rhv返回值==0, 如果*lhv < *rhv返回值<0, 如果*lhv > *rhv返回值>0     
 */
typedef int (*cmp_func_t)(const void *lhv, const void *rhv);

/*!
 *  \brief 计算某个值的hash值
 *  \param [in] val 某个值的指针
 *  \retval 返回*val的hash值
 */
typedef unsigned int (*hash_func_t)(const void *val);

// 销毁value的函数(按照c++说法， 此函数是value的析构函数)

/*!
 *  \brief 销毁某个地址内部所占用的所有内存资源
 *  \param [out] 要销毁的地址
 *  \retval none.     
 */
typedef void (*destroy_func_t)(void *value);


// 定义函数的引用宏

/*!
 *  \brief CSTL_NUM_CMP_FUNC 一个辅助宏, 用来引用数值比较函数
 *  
 *  比如：如果你想使用int类型的比较函数实现，那么你值需要写CSTL_NUM_CMP_FUNC(int)就可以了，你需要实现一个cmp_func_t的
 *  int类型实现，cstl已经为你实现好了。对于其他类型的无符号数值的类型的比较函数实现，一次类推
 *  
 *  \param [in] type 有符号数值类型关键字    
 */
#define CSTL_NUM_CMP_FUNC(type) ((cmp_func_t)(&__cstl_##type##_cmp_func))

/*!
 *  \brief CSTL_UNSIGNED_CMP_FUNC 一个辅助宏，用来引用无符号数值比较函数
 *  
 *  如果你想使用unsigned int的比较函数实现，你值需要些CSTL_UNSIGNED_CMP_FUNC(int), 如果你想使用unsigned char的比较函数，
 *  你只需要些CSTL_UNSIGNED_CMP_FUNC(char)就可以，一次类推。
 *  
 *  \param [in] type 去掉unsigned的数值类型关键字
 */
#define CSTL_UNSIGNED_CMP_FUNC(type) CSTL_NUM_CMP_FUNC(type)


#define __CSTL_DECLARE_TYPE_CMP_FUNC(type) \
    CSTL_LIB int __cstl_##type##_cmp_func(const type *lhv, const type *rhv)

// 声明常见的比较函数，定义位于cstl_stddef.c文件中
__CSTL_DECLARE_TYPE_CMP_FUNC(char);
__CSTL_DECLARE_TYPE_CMP_FUNC(short);
__CSTL_DECLARE_TYPE_CMP_FUNC(int);
__CSTL_DECLARE_TYPE_CMP_FUNC(long);
__CSTL_DECLARE_TYPE_CMP_FUNC(float);
__CSTL_DECLARE_TYPE_CMP_FUNC(double);

#undef __CSTL_DECLARE_TYPE_CMP_FUNC


/*!
 *  \brief CSTL_NUM_HASH_FUNC 使用有符号数值类型的hash函数指针，本质上就是引用cstl内部已经定义好的hash函数
 *  
 *  比如：如果你想获取计算int类型的hash函数，那么你可以使用CSTL_NUM_HASH_FUNC(int), 如果你想计算double类型的
 *  hash函数实现，你可以使用CSTL_NUM_HASH_FUNC(int) 以此类推。
 *  
 *  \param [in] type 无符号数值类型。 
 */
#define CSTL_NUM_HASH_FUNC(type) ((hash_func_t)&__cstl_##type##_hash_func)

/*!
 *  \brief CSTL_UNSIGNED_HASH_FUNC 使用已有的无符号用来计算数值类型的hash函数指针，本质上就是引用cstl内部已经定义好的hash函数  
 *  
 *  比如：如果你想获取计算unsigned int类型数值的hash函数，你可以使用CSTL_UNSIGNED_HASH_FUNC(int) ...
 *  
 *  \param [in] type 去掉unsigned的数值类型 
 */
#define CSTL_UNSIGNED_HASH_FUNC(type) CSTL_NUM_HASH_FUNC(type)

#define CSTL_PTR_HASH_FUNC() ((hash_func_t)&__cstl_ptr_hash_func) //!< 使用计算原始指针的hash函数

#define __CSTL_DECLARE_TYPE_HASH_FUNC(type) \
    CSTL_LIB unsigned int __cstl_##type##_hash_func(const void *)

__CSTL_DECLARE_TYPE_HASH_FUNC(char);
__CSTL_DECLARE_TYPE_HASH_FUNC(short);
__CSTL_DECLARE_TYPE_HASH_FUNC(int);
__CSTL_DECLARE_TYPE_HASH_FUNC(long);
__CSTL_DECLARE_TYPE_HASH_FUNC(float);
__CSTL_DECLARE_TYPE_HASH_FUNC(double);

CSTL_LIB unsigned int __cstl_ptr_hash_func(const void *);

#undef __CSTL_DECLARE_TYPE_HASH_FUNC

CSTL_LIB unsigned int __cstl_str_hash_func(const void *);

#define CSTL_STR_HASH_FUNC (&__cstl_str_hash_func) //!< 使用字符串的hash函数指针

// 主要是为了兼容非c99的编译器
// 表示支持c99标准
#if defined(__STDC__) && defined(__STDC_VERSION__) &&  (__STDC_VERSION__ > 199901L)
#include <stdbool.h>

CSTL_LIB int __cstl_long_long_cmp_func(const long long *lhv, const long long *rhv);
CSTL_LIB int __cstl_long_long_hash_func(const long long *value);

#define CSTL_LLONG_CMP_FUNC (((cmp_func_t)&__cstl_long_long_cmp_func)) //!< 使用long long比较函数
#define CSTL_ULLONG_CMP_FUNC CSTL_LLONG_CMP_FUNC //!< 使用unsigned long long类型的比较函数

#define CSTL_LLONG_HASH_FUNC (((hash_func_t)&__cstl_long_long_hash_func)) //!<使用long long的hash函数
#define CSTL_ULLONG_HASH_FUNC CSTL_LLONG_HASH_FUNC //!<使用unsigned long long类型的hash函数
#else
// 如果不支持c99，那么就统一实现一个bool类型的定义
#define bool int
#define true 1
#define false 0
#define inline
#endif

// 定义空的destroy函数
#define CSTL_NULL_DESTROY_FUNC (&__cstl_destroy_func) //!< 一个宏的destroy函数，内部没有做任何东西

#define CSTL_MAX(a, b) ((a) > (b) ? (a) : (b)) //!< 一个便利宏，获取两个值之间的最大的值
#define CSTL_MIN(a, b) ((a) < (b) ? (a) : (b)) //!< 一个便利宏，获取两个值之间的最小的值
#define CSTL_ABS(a) ((a) >= 0 ? (a) : -(a))    //!< 一个便利宏，获取一个值的绝对值

#if defined(__GNUC__)
#   define UNUSED __attribute__((unused)) //!<一个辅助宏，用来镇压未使用变量的警告
#else
#   define UNUSED //!<一个空的宏
#endif

#include <stdint.h>
#include <stdlib.h>

#endif //STDDEF_H_H
