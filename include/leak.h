/*!
 * \file leak.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了内存泄露检测的所有设施。
 */

#ifndef INCLUDE_LEAK_H_H
#define INCLUDE_LEAK_H_H

#include "cstl_stddef.h"

CSTL_LIB void *__cstl_malloc(unsigned int size, const char *fname
        , int line_number, const char *expression);
CSTL_LIB void *__cstl_realloc(void *ptr, unsigned int size, const char *fname
        , int line_number, const char *expression);
CSTL_LIB void __cstl_free(void *mem);
CSTL_LIB int __cstl_leak_test(bool verbose);

// 启用/禁用测试
CSTL_LIB void __cstl_leak_auto_test(bool enable);

#ifndef NDEBUG

/*!
 * \breif c stdlib中realloc的包装函数，调用它，leak内部会记录调用realloc方法的文件位置和行号为吹，和此内存
 * 是否被释放的信息。此包装函数是线程安全的。
 * \param [in,out] ptr 原来的内存地址
 * \param [in] size 要充分分配的内存的大小
 * \retval 如果分配成功，则会返回新的地址, 并且会free掉ptr, 如果分配失败，ptr的值仍然有效，返回NULL
 */
#   define cstl_realloc(ptr, size) __cstl_realloc(ptr, size, __FILE__, __LINE__, #size)

/*!
 * \brief c stdlib中malloc的包装函数，调用它，leak内部会记录调用malloc函数的文件位置和行号位置, 和此内部
 * 是否被释放的信息。此包装函数是线程安全的。
 * \param [in] size 要分配内存的尺寸
 * \retval 返回新分配内存的地址，分配失败返回NULL 
 */
#   define cstl_malloc(size) __cstl_malloc(size, __FILE__, __LINE__, #size)

/*!
 * \brief c stdlib中free的包含函数，调用它，leak会释放调用它锁保存的使用cstl_malloc, cstl_realloc锁保存的信息，
 * 并且会真正释放掉，之前分配的内存。此包装函数是线程安全的。
 * \param [in,out] mem 要释放的内存地址
 * \note 当此函数执行后，mem将不再有效，使用mem的行为是未知的
 */
#   define cstl_free(mem) __cstl_free(mem)

/*!
 * \brief 会检测当前已经分配的内存是否为空，如果不为空则说明有内存泄露
 * \param [in] verbose 用来指定是否打印内存泄露信息，如果为true，并且有分配的内存块不为空的话，则会打印所有内存
 * 块的相关信息，格式是：file:line_number:expression
 * \retval 不为空，返回true, 为空返回false
 */
#   define cstl_leak_test(verbose) __cstl_leak_test(verbose) 

/*!
 * \brief 调用它，就会让在程序退出的时候，自动调用cstl_leak_test(true),即会自动检测内存泄露情况
 * \retval none.
 */
#   define cstl_leak_enable_auto_test() __cstl_leak_auto_test(true) 

/*!
 * \brief 是cstl_leak_enable_auto_test的逆操作，禁止在程序退出时候，自动进行内存泄露检测
 */
#   define cstl_leak_disable_auto_test() __cstl_leak_auto_test(false) 

#else

#   define cstl_realloc(ptr, size) realloc(ptr, size)
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
