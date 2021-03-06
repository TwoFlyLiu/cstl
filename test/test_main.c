/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 03 22:24:14
*/
#include <check_util.h>

DECLARE_SUITE(leak);
DECLARE_SUITE(vec);
DECLARE_SUITE(hmap);
DECLARE_SUITE(hset);
DECLARE_SUITE(str);
DECLARE_SUITE(wstr);
DECLARE_SUITE(str_conv);
DECLARE_SUITE(u8_str);
DECLARE_SUITE(list);
DECLARE_SUITE(rmap);


// 使用END_CHECK_MAIN_AFTER来当所有测试都结束的时候，执行检测操作
// 对于check, 每个test的执行环境都是隔离的，他们的运行环境是
// 单个进程
// 
// 设置CK_FORK=no 来使用单进程测试

START_CHECK_MAIN(cstl)
    SUITE(leak)
    SUITE(vec)
    SUITE(hmap)
    SUITE(hset)
    SUITE(str)
    SUITE(wstr)
    SUITE(str_conv)
    SUITE(u8_str)
    SUITE(list)
    SUITE(rmap)
END_CHECK_MAIN()

