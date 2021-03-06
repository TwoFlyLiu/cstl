/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 09 30 16:28:31
*/
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "vec.h"
#include "leak.h"

static inline size_t
__calc_new_capacity(VEC *vec, size_t elem_count)
{
    return vec_size(vec) + CSTL_MAX((size_t)vec_size(vec), elem_count);
}

static inline void 
__free(void *ptr)
{
    if (ptr != NULL) {
        cstl_free(ptr);
    }
}

VEC *vec_new(int unit_size, destroy_func_t destroy_func)
{
    return vec_new_with_capacity(unit_size, destroy_func, 10);
}

// capacity - 单位是元素的数目
VEC *vec_new_with_capacity(int unit_size, destroy_func_t destroy_func,  int capacity)
{
    int total_size = unit_size * capacity;
    VEC *new_vec = (VEC *)cstl_malloc(sizeof(VEC));

    new_vec->capacity = capacity;
    new_vec->len = 0;
    new_vec->unit_size = unit_size;
    new_vec->destroy_func = destroy_func;

    new_vec->beg = cstl_malloc(total_size);
    return new_vec;
}

inline size_t vec_capacity(const VEC *vec)
{
    assert(vec);
    return vec->capacity;
}

inline size_t vec_size(const VEC *vec)
{
    assert(vec);
    return vec->len;
}

// 重新分配内部的保存数据的数组
static void
__vec_realloc(VEC *vec, int new_capacity)
{
    vec->capacity = new_capacity;
    vec->beg = cstl_realloc(vec->beg, vec->capacity * vec->unit_size);
}

void vec_resize(VEC *vec, size_t new_size, const void *value)
{
    assert(vec && new_size >= 0);
    
    if (new_size <= vec_size(vec)) {
        if (vec->destroy_func) {
            for (int i = new_size; i < (int)vec_size(vec); i++) {
                (*vec->destroy_func)(vec_get(vec, i));
            }
        }
        vec->len = new_size;
    } else {
        __vec_realloc(vec, new_size);
        for (int i = vec_size(vec); i < (int)new_size; i++) {
            vec_push_back(vec, value);
        }
    }
}

static inline void
__vec_put(VEC *vec, int index, const void *elem)
{
    memmove((char*)vec->beg + (index * vec->unit_size), elem, vec->unit_size);
}

void vec_push_back(VEC *vec, const void *elem)
{
    assert(vec && elem != NULL);

    if (vec_size(vec) >= vec_capacity(vec)) {
        __vec_realloc(vec, __calc_new_capacity(vec, 1));
    }

    __vec_put(vec, vec->len++, elem);
}

void vec_push_front(VEC *vec, const void *elem)
{
    assert(vec && elem != NULL);

    if (vec_size(vec) >= vec_capacity(vec)) {
        __vec_realloc(vec, __calc_new_capacity(vec, 1));
    }

    // 后面位置要挪移一位，这个时候不能够在调用memcpy, 因为他是从前往后复制，这样就会前面会覆盖掉后面的内容
    for (int i = vec->len - 1; i >= 0; --i) {
        __vec_put(vec, i + 1, vec_get(vec, i));
    }

    __vec_put(vec, 0, elem);
    ++ vec->len;
}

void vec_pop_front(VEC *vec)
{
    assert(vec);
    vec_erase(vec, 0);
}

void vec_pop_back(VEC *vec)
{
    assert(vec);
    vec_erase(vec, vec_size(vec) - 1);
}

void vec_erase(VEC *vec, size_t index)
{
    assert(vec);
    if (index >= vec_size(vec)) return;

    if (vec->destroy_func) {
        (*vec->destroy_func)(vec_get(vec, index));
    }

    -- vec->len;
    memmove((char*)vec->beg + (index * vec->unit_size), (char*)vec->beg + ((index + 1) * vec->unit_size), 
            (vec->len - index) * vec->unit_size);
}

// 移除第一个出现的值
void vec_remove(VEC *vec, const void *value, cmp_func_t compare)
{
    assert(compare && "Must sepcify compare func in the remove method of vec!");

    for (size_t i = 0; i < vec_size(vec); i++) {
        if (0 == (*compare)(value, vec_get(vec, i))) {
            vec_erase(vec, i);
            return;
        }
    }
}

void vec_remove_all(VEC *vec, const void *value, cmp_func_t compare)
{
    assert(compare && "Must sepcify compare func in the remove method of vec!");
    assert(vec && "VEC can't be null!");

    for (size_t i = 0; i < vec_size(vec);) {
        if (0 == (*compare)(value, vec_get(vec, i))) {
            vec_erase(vec, i);
        } else {
            i++;
        }
    }
}

void vec_clear(VEC *vec)
{
    assert(vec);
     
    if (vec->destroy_func != NULL) {
        for (size_t i = 0; i < vec_size(vec); i++) {
            (*vec->destroy_func)(vec_get(vec, i));
        }
    }

    __free(vec->beg); //释放掉原来的内存
    vec->beg = NULL;
    vec->len = 0;
    vec->capacity = 0;
}

inline bool vec_empty(const VEC *vec)
{
    assert(vec);

    return 0 == vec_size(vec);
}

void *vec_get(VEC *vec, size_t index)
{
    assert(vec);

    if (index >= vec_size(vec)) {
        return NULL;
    } else {
        return (char*)vec->beg + (index * vec->unit_size);
    }
}

void *vec_at(VEC *vec, int index)
{
    return vec_get(vec, index);
}

inline void vec_set(VEC *vec, size_t index, const void *value)
{
    assert(vec != NULL && index >= 0 && index < vec_size(vec) && value != NULL);
    if (vec->destroy_func) {
        (*vec->destroy_func)(vec_get(vec, index));
    }
    __vec_put(vec, index, value);
}

void *vec_front(VEC *vec)
{
    assert(vec);

    if (vec_empty(vec)) {
        return NULL;
    }
    return vec_get(vec, 0);
}

void *vec_back(VEC *vec)
{
    assert(vec);

    if (vec_empty(vec)) {
        return NULL;
    }
    return vec_get(vec, vec->len - 1);
}


void *vec_find(VEC *vec, const void *val, cmp_func_t compare)
{
    assert(vec && compare);

    for (size_t i = 0; i < vec_size(vec); i++) {
        if (0 == (*compare)(val, vec_get(vec, i))) {
            return vec_get(vec, i);
        }
    }

    return NULL;
}

// 二分法查找，要求vec内部数据是有序的，
// 所以在调用此函数之前，要先调用对应的vec_sort函数
void *vec_bin_find(VEC *vec, const void *val,  cmp_func_t compare)
{
    int left, right, mid;
    int cmp_rs;

    assert(vec);

    if (vec_empty(vec)) {
        return NULL;
    }

    left = 0;
    right = vec_size(vec) - 1;

    while (left <= right) {
        mid = (left + right) / 2;
        cmp_rs = (*compare)(vec_get(vec, mid), val);

        if (0 == cmp_rs) {
            return vec_get(vec, mid);
        } else if (cmp_rs > 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return NULL;
}

static inline void 
__vec_insert_middle(VEC *vec, int index, const void *elem)
{
    if (vec_size(vec) >= vec_capacity(vec)) {
        __vec_realloc(vec, __calc_new_capacity(vec, 1));
    }

    for (int i = vec_size(vec) - 1; i >= index; i--) {
        __vec_put(vec, i + 1, vec_get(vec, i));
    }
    __vec_put(vec, index, elem);
    ++ vec->len;
}

void vec_insert(VEC *vec, size_t index, const void *elem)
{
    assert(vec && elem != NULL);
    
    if (0 == index) {
        vec_push_front(vec, elem);
    } else if (index >= vec_size(vec)) {
        vec_push_back(vec, elem);
    } else {
        __vec_insert_middle(vec, index, elem);
    }
}

void vec_extend(VEC *vec, int index, const VEC *insert_vec)
{
    assert(vec && insert_vec && index >= 0);

    if (vec_size(vec) + vec_size(insert_vec) > vec_capacity(vec)) {
        __vec_realloc(vec, __calc_new_capacity(vec, vec_size(insert_vec)));
    }

    // 在行尾插入
    if (index >= (int)vec_size(vec)) {
        for (size_t i = 0; i < vec_size(insert_vec); i++) {
            vec_push_back(vec, vec_get((VEC*)insert_vec, i));
        }
    } else {
        // 先向后移动原来的元素
        for (int i = vec_size(vec) - 1; i >= index; i--) {
            __vec_put(vec, i + vec_size(insert_vec), vec_get(vec, i));
        } 

        // 插入新的元素
        for (size_t i = 0; i < vec_size(insert_vec); i++) {
            __vec_put(vec, i + index, vec_get((VEC*)insert_vec, i));
        }

        // 更新元素长度
        vec->len += vec_size(insert_vec);
    }
}

void vec_extend_front(VEC *vec, const VEC *insert_vec)
{
    vec_extend(vec, 0, insert_vec);
}

void vec_extend_back(VEC *vec, const VEC *insert_vec)
{
    vec_extend(vec, vec_size(vec), insert_vec);
}

static inline void
__vec_swap(VEC *vec, size_t idx1, size_t idx2)
{
    char tmp[vec->unit_size]; //c99 数组支持运行时尺寸 

    memmove(tmp, (char*)vec->beg + (idx1 * vec->unit_size), vec->unit_size);
    memmove((char*)vec->beg + (idx1 * vec->unit_size), (char*)vec->beg + (idx2 * vec->unit_size), vec->unit_size);
    memmove((char*)vec->beg + (idx2 * vec->unit_size), tmp, vec->unit_size);
}

void vec_swap(VEC *vec, size_t idx1, size_t idx2)
{
    assert(vec && "vec_t instance can't be null!");
    assert((idx1 >= 0 && idx1 < vec_size(vec)) && (idx2 >= 0 && idx2 < vec_size(vec)) 
            && "idx1 and idx2 must be between 0 and vec_size(vec) - 1!");

    __vec_swap(vec, idx1, idx2);
}

static void
__vec_get_copy(VEC *vec, int index, void *ret)
{
    void *value = vec_get(vec, index);
    memmove(ret, value, vec->unit_size);
}

// 返回的索引，可以保证索引左侧的值都比pivo_value小，右侧的值都是pivot_value大
static int
__vec_partition(VEC *vec, int left, int right, cmp_func_t compare)
{
    char pivot[vec->unit_size];
    int i = left;
    int j = right;
    __vec_get_copy(vec, left, pivot);

    while (i < j) {

        // 从右向左左，找第一个比pivot小或相等的数
        while (i < j && (*compare)(vec_get(vec, j), pivot) > 0)
            j--;
        if (i < j) {
            __vec_put(vec, i, vec_get(vec, j));
            i++;
        }


        // 从左向右找，第一个比pivot大或者相等的数
        while (i < j && (*compare)(vec_get(vec, i), pivot) < 0) {
            i++;
        }

        if (i < j) {
            __vec_put(vec, j, vec_get(vec, i));
            j--;
        }
    }

    __vec_put(vec, i, pivot);
    return i;
}

//1 2 3 4 5
//    ^
static void
__vec_quick_sort(VEC *vec, int left, int right, cmp_func_t compare)
{
    int pivot_index;

    if (left < right) {
        pivot_index = __vec_partition(vec, left, right, compare);
        __vec_quick_sort(vec, left, pivot_index - 1, compare);
        __vec_quick_sort(vec, pivot_index + 1, right, compare);
    }
}

void vec_sort(VEC *vec, cmp_func_t compare)
{
    __vec_quick_sort(vec, 0, vec_size(vec) - 1, compare);
}

void vec_foreach(VEC *vec, vec_foreach_func_t vec_foreach_func, void *user_data)
{
    assert(vec && "vec can't be null!");
    assert(vec_foreach_func && "foreach callback can't be null!");

    for (size_t i = 0; i < vec_size(vec); i++) {
        (*vec_foreach_func)(vec_get(vec, i), user_data);
    }
}

void vec_free(VEC *vec)
{
    assert(vec);

    // 调用析构函数
    if (vec->destroy_func != NULL) {
        for (size_t i = 0; i < vec_size(vec); i++) {
            (*vec->destroy_func)((char*)vec->beg + (i * vec->unit_size));
        }
    }

    __free(vec->beg);
    __free(vec);
}
