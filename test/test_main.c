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

static void leak_test(void);

START_CHECK_MAIN(cstl)
    atexit(leak_test);

    SUITE(debug)
    SUITE(vec)
    /*SUITE(hmap)*/
    /*SUITE(hset)*/
END_CHECK_MAIN()

// 当整个应用退出的时候进行内存泄露检测
static void leak_test(void)
{
    cstl_leak_test();
}
