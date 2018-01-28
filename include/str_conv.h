/*!
 * \file str_conv.h
 * \author twoflyliu
 * \version v0.0.6
 * \date 2017年10月22日08:58:37
 * \copyright GNU Public License V3.0
 * \brief 此文件中声明了对于常用字符集转换的api函数
 * 
 * 比如：
 * 1. gbk和utf8指向的相互转换
 * 2. wchar和gbk之间的相互转换
 * 3. native和wchar之间的相互转换
 * 
 * 本地字符串，在windows上，很有可能是ascii, 也有可能是gbk,在linux上则是utf8。
 * 宽字符串在windows上使用的是utf16-lb, 在linux则是utf32-lb。
 * utf8编码的字符串在不同的系统上使用的都是相同的格式。
 */

#ifndef INCLUDE_STR_CONV_H_H
#define INCLUDE_STR_CONV_H_H

#include "cstl_stddef.h"

struct CSTLString;
typedef struct CSTLString str_t;
struct CSTLWString;
typedef struct CSTLWString wstr_t;
struct CSTLUtf8String;
typedef struct CSTLUtf8String u8_str_t;

//str_t 可以用来保存任何数据类型,可以说是和系统相关的
//可能是utf8，可能是gbk,依赖于系统
//
//wstr_t u8_str_t编码类型都是确定，前者使用utf16编码，后者使用utf8编码
//

/*!
 *  \brief wstr_to_str 将宽字符串转换为本地字符串
 *  \param [in] wstr 原来的宽字符串
 *  \retval 返回本地字符串，对于windows， 返回的可能是ascii字符，也可能是gbk,字符，要根据你的系统语言设置，对于linux,则
 *  可能是utf8字符集，总之这个返回的字符串是可以直接在控制台上显示，不会出现乱码问题。
 *  \note wstr不能为NULL，否则会断言失败
 */
CSTL_LIB str_t *wstr_to_str(const wstr_t *wstr);

/*!
 *  \brief wstr_to_u8_str 将款字符串转换为utf8字符串
 *  \param [in] wstr 原来的宽字符串
 *  \retval 返回utf8编码的字符串
 *  \note wstr不能为NULL，否则会断言失败
 */
CSTL_LIB u8_str_t *wstr_to_u8_str(const wstr_t *wstr); 

/*!
 *  \brief str_to_wstr 将本地的字符串转换为宽字符串
 *  \param [in] str 原来的本地字符串
 *  \retval 返回宽字符串
 *  \note str不能为NULL，否则会断言失败 
 */
CSTL_LIB wstr_t *str_to_wstr(const str_t *str);

/*!
 *  \brief str_to_u8_str 将本地字符串转换为utf8字符串
 *  \param [in] str 原来的本地字符串
 *  \retval 返回utf8编码的字符串
 *  \note str不能为NULl，否则会断言失败 
 */
CSTL_LIB u8_str_t *str_to_u8_str(const str_t *str);

/*!
 *  \brief u8_str_to_wstr 将utf8编码的字符串转换为宽字符串
 *  \param [in] u8_str utf8编码的字符串
 *  \retval 返回宽字符
 */
CSTL_LIB wstr_t *u8_str_to_wstr(const u8_str_t *u8_str);

/*!
 *  \brief u8_str_to_str 将utf8编码的字符串转换为本地字符串
 *  \param [in] u8_str 原来utf8编码的字符串
 *  \retval 返回本地的字符串 
 */
CSTL_LIB str_t *u8_str_to_str(const u8_str_t *u8_str);

/*!
 *  \brief wstr_to_gbk 将宽字符串转换为gbk编码的字符串
 *  \param [in] str 原来的宽字符串
 *  \retval 返回gbk编码的字符串 
 */
CSTL_LIB str_t *wstr_to_gbk(const wstr_t *str);

/*!
 *  \brief u8_str_to_gbk 将utf8编码的字符串转换为gbk编码的字符串
 *  \param [in] wstr 原来的utf8编码的字符串
 *  \retval 返回转换后的gbk编码的字符串 
 */
CSTL_LIB str_t *u8_str_to_gbk(const u8_str_t *str);

/*!
 *  \brief str_to_gbk 将本地的字符串转换为gbk编码的字符串
 *  \param [in] str 原来本地编码的字符串
 *  \retval 返回转换后的gbk编码的字符串 
 */
CSTL_LIB str_t  *str_to_gbk(const str_t *str);

/*!
 *  \brief gbk_to_wstr 将gbk编码的字符串转换为宽字符串
 *  \param [in] gbk 原来gbk编码的字符串
 *  \retval 返回转换后的宽字符串 
 */
CSTL_LIB wstr_t *gbk_to_wstr(const str_t *gbk);

/*!
 *  \brief gbk_to_u8_str 将gbk编码的字符串转换为utf8编码的字符串
 *  \param [in] gbk 原来的gbk编码的字符串
 *  \retval 返回转换后的utf8编码的字符串 
 */
CSTL_LIB u8_str_t *gbk_to_u8_str(const str_t *gbk);

/*!
 *  \brief gbk_to_str 将gbk编码的字符串转换为本地的字符串
 *  \param [in] gbk 原来gbk编码的字符串
 *  \retval 返回本地的字符串 
 */
CSTL_LIB str_t  *gbk_to_str(const str_t *gbk);

#endif //INCLUDE_STR_CONV_H_H
