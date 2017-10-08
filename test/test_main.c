/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 03 22:24:14
*/
#include <check_util.h>

#include "debug.h"

DECLARE_SUITE(debug);
DECLARE_SUITE(vec);
/*DECLARE_SUITE(hmap);*/
/*DECLARE_SUITE(hset);*/

// 使用END_CHECK_MAIN_AFTER来当所有测试都结束的时候，执行检测操作
// 不要使用atexit, 好像有问题
static int
__leak_test(int res);

START_CHECK_MAIN(cstl)

    SUITE(debug)
    SUITE(vec)
    /*SUITE(hmap)*/
    /*SUITE(hset)*/
END_CHECK_MAIN_AFTER(__leak_test)

static int
__leak_test(int res)
{
    int leak_res = 0;
   
    /*cstl_malloc(20);*/
    /*cstl_malloc(sizeof(double));*/
    leak_res =  cstl_leak_test(1);
    return res && leak_res;
}
