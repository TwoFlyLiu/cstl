/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 08 17:06:07
*/
#include <limits.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include "leak.h"

#define MAX_BUCKETS 1024
#define EXPRESSION_MAX 200

// 都是int的整数倍，不会有对齐问题
typedef struct memory_record_node {
    void *memory;           //保存分配的内容
    int len;                //分配内存的长度
    char fname[PATH_MAX];   //调用CSTL_MALLOC的文件名称
    char expression[EXPRESSION_MAX]; //存放返回整形值的表达式
    int line_number;        //调用CSTL_MALLOC的所在文件的行号
    struct memory_record_node *next; 
} memory_record_node_t;

// 因为是静态的，所以初始化内部成员为NULL
static memory_record_node_t * memory_record_lists[MAX_BUCKETS];
static pthread_mutex_t lists_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool auto_test_first_called = true;
static bool auto_test_enabled = false;

static inline memory_record_node_t *
__malloc_memory_record_node(unsigned int size, const char *fname, int line, const char *expression);

static inline void memory_record_lists_insert(memory_record_node_t *record_node);
static inline void memory_record_lists_remove(void *me);
static inline int memory_record_lists_leak_test(bool verbose);


// 以分配内存值的地址来作为key计算hash值
static inline unsigned int ptr_hash_value(void *memory);

static void on_auto_test()
{
    int leak_count;

    if (auto_test_enabled) {
        leak_count = __cstl_leak_test(true);
        if (leak_count == 0) {
            printf("no leak!\n");
        }
    }
}

CSTL_EXPORT void __cstl_leak_auto_test(bool enable)
{
    auto_test_enabled = enable;

    if (auto_test_first_called) {
        auto_test_first_called = false;
        atexit(on_auto_test);
    }
}

CSTL_EXPORT void *
__cstl_malloc(unsigned int size, const char *fname, int line, const char *expression)
{
    memory_record_node_t *node = __malloc_memory_record_node(size, fname, line, expression);
    assert(node && node->memory && "malloc memory record node failed!");
    memory_record_lists_insert(node);
    return node->memory;
}

CSTL_EXPORT void 
__cstl_free(void *mem)
{
    if (mem == NULL) return;
    memory_record_lists_remove(mem);
}

CSTL_EXPORT int __cstl_leak_test(bool verbose)
{
    return memory_record_lists_leak_test(verbose);
}

static inline memory_record_node_t *
__malloc_memory_record_node(unsigned int size, const char *fname, int line, const char *expression)
{
    memory_record_node_t *node = (memory_record_node_t*)malloc(sizeof(memory_record_node_t));
    node->memory = malloc(size);
    node->len = size;
    strcpy(node->fname, fname);
    strcpy(node->expression, expression);
    node->line_number = line;
    node->next = NULL;
    return node;
}

// 支持多线程
static inline void memory_record_lists_insert(memory_record_node_t *record_node)
{
    unsigned int hash_val;
    memory_record_node_t *last_node;

    pthread_mutex_lock(&lists_mutex);

    // 获取hash值
    hash_val = ptr_hash_value(record_node->memory) % MAX_BUCKETS;


    // 寻找插入点
    last_node = memory_record_lists[hash_val];

    while (last_node && last_node->next != NULL) {
        last_node = last_node->next;
    }

    // 进行插入
    if (last_node != NULL) {
        last_node->next = record_node;
    } else {
        memory_record_lists[hash_val] = record_node;
    }

    pthread_mutex_unlock(&lists_mutex);
}

static inline void memory_record_lists_remove(void *mem)
{
    unsigned int hash_val;
    memory_record_node_t *node;
    memory_record_node_t *prev_node = NULL;

    pthread_mutex_lock(&lists_mutex);

    // 获取hash值
    hash_val = ptr_hash_value(mem) % MAX_BUCKETS;


    // 查找节点
    node = memory_record_lists[hash_val];

    while (node && mem != node->memory) {
        prev_node = node;
        node = node->next;
    }

    assert(node && "remove invalid memory or not be allocated by cstl_malloc");

    // 进行删除操作
    if (NULL == prev_node) { //节点是头节点
        memory_record_lists[hash_val] = node->next;
    } else {
        prev_node->next = node->next;
    }

    // 释放内存
    free(node->memory);
    free(node);

    pthread_mutex_unlock(&lists_mutex);
}

static inline void memory_record_node_report_leak_info(memory_record_node_t *node);
static inline int memory_record_lists_leak_test(bool verbose)
{
    int i;
    memory_record_node_t *node;
    int leak_count = 0;
    int total_leak_memory = 0;

    pthread_mutex_lock(&lists_mutex);

    // verbose是全局变量
    for (i = 0; i < MAX_BUCKETS; i++) {
        node = memory_record_lists[i];
        while (node) {
            if (verbose) {
                memory_record_node_report_leak_info(node); //值报告错误，不会释放掉泄露的内存
            }
            total_leak_memory += node->len;
            node = node->next;
            ++ leak_count;
        }
    }
    
    if (verbose && leak_count > 0) {
        printf("total leak count: %d, total leak memory: %d bytes\n", leak_count, total_leak_memory);
    }

    pthread_mutex_unlock(&lists_mutex);
    return leak_count;
}


// 以分配内存值的地址来作为key计算hash值
static inline unsigned int ptr_hash_value(void *memory)
{
    return (unsigned int)((uintptr_t)memory);
}

static inline void memory_record_node_report_leak_info(memory_record_node_t *node)
{
    fprintf(stderr, "%s:%d:cstl_malloc(%s):missing matched cstl_free!\n"
            , node->fname, node->line_number, node->expression);
}
