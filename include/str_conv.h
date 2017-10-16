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

CSTL_LIB str_t *wstr_to_str(const wstr_t *wstr);
CSTL_LIB u8_str_t *wstr_to_u8_str(const wstr_t *wstr); 

CSTL_LIB wstr_t *str_to_wstr(const str_t *str);
CSTL_LIB u8_str_t *str_to_u8_str(const str_t *str);

CSTL_LIB wstr_t *u8_str_to_wstr(const u8_str_t *u8_str);
CSTL_LIB str_t *u8_str_to_str(const u8_str_t *u8_str);

CSTL_LIB str_t *wstr_to_gbk(const wstr_t *str);
CSTL_LIB str_t *u8_str_to_gbk(const u8_str_t *wstr);
CSTL_LIB str_t  *str_to_gbk(const str_t *str);

CSTL_LIB wstr_t *gbk_to_wstr(const str_t *gbk);
CSTL_LIB u8_str_t *gbk_to_u8_str(const str_t *gbk);
CSTL_LIB str_t  *gbk_to_str(const str_t *gbk);

#endif //INCLUDE_STR_CONV_H_H
