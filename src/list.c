/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 10 17 07:11:17
*/
#include <assert.h>
#include <string.h>

#include "list.h"
#include <leak.h>

static list_node_t *_list_node_new(const void *data, int data_size)
{
    list_node_t *node = (list_node_t*)cstl_malloc(sizeof(list_node_t));
    node->data = cstl_malloc(data_size);
    memmove(node->data, data, data_size);

    node->next = NULL;
    node->prev = NULL;
    return node;
}

static void _list_node_free(list_node_t *node)
{
    assert(node);

    if (node->data) {
        cstl_free(node->data);
    }
    cstl_free(node);
}


// 工厂方法
CSTL_LIB LIST *list_new(size_t unit_size, destroy_func_t destroy)
{
    list_t *list = (list_t*)cstl_malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->unit_size = unit_size;
    list->destroy_func = destroy;
    list->len = 0;
    return list;
}

CSTL_LIB void list_free(LIST *list)
{
    assert(list);

    list_node_t *next = list->head;
    list_node_t *cur = NULL;

    while (next) {
        cur = next;
        next = next->next;

        if (list->destroy_func) {
            (*list->destroy_func)(cur->data);
        }
        _list_node_free(cur);
    }

    cstl_free(list);
}

// 访问修改容量信息
CSTL_LIB size_t list_capacity(const UNUSED LIST *list)
{
    assert(list);
    return (size_t)-1;
}

CSTL_LIB bool list_empty(const LIST *list)
{
    assert(list);
    return (NULL == list->head);
}

CSTL_LIB inline size_t list_size(const LIST *list)
{
    assert(list);
    return list->len; //使用len来记录列表长度，可以加速size函数速度
}

// 增加
CSTL_LIB void list_push_front(LIST *list, const void *elem)
{
    assert(list && elem);
    list_node_t *node = _list_node_new(elem, list->unit_size);

    if (list->head) {
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    } else {
        list->tail= list->head = node;
    }
    list->len ++;
}

CSTL_LIB void list_push_back(LIST *list, const void *elem)
{
    assert(list && elem);
    list_node_t *node = _list_node_new(elem, list->unit_size);

    if (list->tail) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    } else {
        list->tail = list->head = node;
    }
    list->len ++;
}

CSTL_LIB void list_insert(LIST *list, int index, const void *elem)
{
    assert(list && elem);


    index = CSTL_MAX(0, index);
    index = CSTL_MIN((int)list->len, index);

    if (0 == index) {
        return list_push_front(list, elem);
    } 

    if ((int)list->len == index) {
        return list_push_back(list, elem);
    }

    list_node_t *new_node = _list_node_new(elem, list->unit_size);

    // 找到原来位置上的节点
    list_node_t *node = list->head;
    for(; index > 0; --index) {
        node = node->next;
    }

    // 进行插入
    node->prev->next = new_node;
    new_node->prev = node->prev;
    new_node->next = node;
    node->prev = new_node;

    list->len ++;
}

CSTL_LIB void list_extend(LIST *list, int index, const LIST *insert_list)
{
    assert(list && insert_list);

    index = CSTL_MAX(0, index);
    index = CSTL_MIN((int)list_size(list), index);

    const list_node_t *node = insert_list->head;
    for (; node; node = node->next) {
        list_insert(list, index, node->data);
        ++index;
    }
}

CSTL_LIB void list_extend_front(LIST *list, const LIST *insert_list)
{
    assert(list && insert_list);
    list_extend(list, 0, insert_list);
}

CSTL_LIB void list_extend_back(LIST *list, const LIST *insert_list)
{
    assert(list && insert_list);
    list_extend(list, list_size(list), insert_list);
}

//  访问/修改
CSTL_LIB void *list_get(LIST *list, int index)
{
    assert(list);
    list_node_t *node = list_get_node(list, index);
    return node != NULL ? node->data : NULL;
}

CSTL_LIB list_node_t *list_get_node(LIST *list, int index)
{
    assert(list);

    if (index < 0 || index >= (int)list_size(list)) {
        return NULL;
    }

    list_node_t *node = list->head;
    for (; index > 0; --index) {
        node = node->next;
    }

    return node;
}

CSTL_LIB void *list_at(LIST *list, int index)
{
    assert(list);
    return list_get(list, index);
}

CSTL_LIB void list_set(LIST *list, int index, const void *value)
{
    assert(list);
    void *data = list_get(list, index);

    if (NULL != data) {
        if (list->destroy_func) {
            (*list->destroy_func)(data);
        }
        memmove(data, value, list->unit_size);
    }
}

CSTL_LIB void *list_front(LIST *list)
{
    assert(list);
    if (list->head) {
        return list->head->data;
    }
    return NULL;
}

CSTL_LIB void *list_back(LIST *list)
{
    assert(list);

    if (list->tail) {
        return list->tail->data;
    }
    return NULL;
}

static void _list_destroy_node(LIST *list, list_node_t *node)
{
    if (list->destroy_func) {
        (*list->destroy_func)(node->data);
    }
    _list_node_free(node);
}

// 移除
CSTL_LIB void list_pop_back(LIST *list)
{
    assert(list);

    if (NULL == list->tail) {
        return;
    }

    list_node_t *node = list->tail;
    // 只有一个元素的情况
    if (list->tail == list->head) {
        list->tail = list->head = NULL;
    } else {
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
    }

    _list_destroy_node(list, node);
    list->len --;
}

CSTL_LIB void list_pop_front(LIST *list)
{
    assert(list);

    if (NULL == list->head) return;

    list_node_t *node = list->head;

    if (list->head == list->tail) {
        list->head = list->tail = NULL;
    } else {
        list->head->next->prev = NULL;
        list->head = list->head->next;
    }

    _list_destroy_node(list, node);
    list->len --;
}

CSTL_LIB void list_remove(LIST *list, void *value, cmp_func_t compare)
{
    assert(list && compare);
    list_node_t *node = list_find_node(list, value, compare);
    list_remove_node(list, node);
}

CSTL_LIB void list_remove_node(LIST *list, list_node_t *node)
{
    if (node == NULL) return;
    if (node == list->head) return list_pop_front(list);
    if (node == list->tail) return list_pop_back(list);

    node->prev->next = node->next;
    node->next->prev = node->prev;
    _list_destroy_node(list, node);
    list->len --;
}

CSTL_LIB void list_erase(LIST *list, int index)
{
    assert(list);

    list_node_t *node = list_get_node(list, index);
    if (NULL == node) return;
    if (node == list->head) return list_pop_front(list);
    if (node == list->tail) return list_pop_back(list);

    node->prev->next = node->next;
    node->next->prev = node->prev;
    _list_destroy_node(list, node);
    list->len --;
}

CSTL_LIB void list_clear(LIST *list)
{
    assert(list);
    list_node_t *next = list->head;
    list_node_t *cur = NULL;

    while (next) {
        cur = next;
        next = next->next;
        _list_destroy_node(list, cur);
    }

    list->head = NULL;
    list->tail = NULL;
    list->len = 0;
} 

// 查找
CSTL_LIB list_node_t *list_find_node(LIST *list, void *val, cmp_func_t compare)
{
    assert(list && compare);
    list_node_t *node = list->head;

    while (node && (*compare)(node->data, val) != 0) {
        node = node->next;
    }
    return node;
}
CSTL_LIB void *list_find(LIST *list, void *val, cmp_func_t compare)
{
    assert(list && compare);
    list_node_t *node = list_find_node(list, val, compare);

    return (node != NULL ? node->data : NULL);
}

CSTL_LIB void *list_bin_find(LIST *list, void *val, cmp_func_t compare)
{
    int left, right, mid;
    int cmp_rs;
    void *cur_val;

    assert(list);

    if (list_empty(list)) {
        return NULL;
    }

    left = 0;
    right = list_size(list) - 1;

    while (left <= right) {
        mid = (left + right) / 2;
        cur_val = list_get(list, mid);
        cmp_rs = (*compare)(cur_val, val);

        if (0 == cmp_rs) {
            return cur_val;
        } else if (cmp_rs > 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return NULL;
}

// 返回的索引，可以保证索引左侧的值都比pivo_value小，右侧的值都是pivot_value大
static list_node_t *
__list_partition(list_node_t *left, list_node_t *right, cmp_func_t compare)
{
    void *pivot;
    pivot = left->data;

    while (left != right) {

        // 从右向左左，找第一个比pivot小或相等的数
        while (left != right && (*compare)(right->data, pivot) > 0)
            right = right->prev;
        if (left != right) {
            left->data = right->data;
            left = left->next;
        }


        // 从左向右找，第一个比pivot大或者相等的数
        while (left != right && (*compare)(left->data, pivot) < 0) {
            left = left->next;
        }

        if (left != right) {
            right->data = left->data;
            right = right->prev;
        }
    }

    left->data = pivot;
    return left;
}

//1 2 3 4 5
//    ^
static void
__list_quick_sort(list_node_t *left, list_node_t *right, cmp_func_t compare)
{
    list_node_t *pivot_pos;

    // 当left, 或者right == NULL, 说明分块分到只有一个元素的情况，说明这个位置肯定是有序
    if ((left == NULL) || (right == NULL) || (left == right)) return;

    pivot_pos = __list_partition(left, right, compare);

    // 因为链表是节点地址的大小关系可能不不是连续的，所以不适用left < right来作为边界
    // 但是要保证pivot_pos的之前的节点，和之后的节点不能够越界，就只有想下面这个写法
    if (pivot_pos != left) {
        __list_quick_sort(left, pivot_pos->prev, compare);
    }
    if (pivot_pos != right) {
        __list_quick_sort(pivot_pos->next, right, compare);
    }
}


// 排序（使用快速排序算法）
CSTL_LIB void list_sort(LIST *list, cmp_func_t compare)
{
    if (list_size(list) < 2) return;
    __list_quick_sort(list->head, list->tail, compare);
}

// 交换两个元素的值
CSTL_LIB void list_swap(LIST *list, int idx1, int idx2)
{
    assert(list && idx1 >= 0 && idx1 < (int)list_size(list) && idx2 >= 0 && idx2 < (int)list_size(list));

    list_node_t *node1 = list_get_node(list, idx1);
    list_node_t *node2 = list_get_node(list, idx2);

    void *tmp = node1->data;
    node1->data = node2->data;
    node2->data = tmp;
}

// 遍历函数
CSTL_LIB void list_foreach(LIST *list, list_foreach_func_t list_foreach_func, void *user_data)
{
    assert(list);

    for (list_node_t *node = list->head; node; node = node->next) {
        if (list_foreach_func) {
            (*list_foreach_func)(node->data, user_data);
        }
    }
}

CSTL_LIB void list_resize(LIST *list, size_t new_size, void *val)
{
    assert(list);
    size_t old_size = list_size(list);

    // 缩小
    if (new_size < old_size) {
        while (list_size(list) > new_size) {
            list_pop_back(list);
        }
    } else { //扩大
        int expand_size = new_size - old_size;
        for (int i = 0; i < expand_size; i++) {
            list_push_back(list, val);
        }
    }
}
