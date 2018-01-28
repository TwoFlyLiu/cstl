/********************************************************
* Description: @description@
* Author: twoflyliu
* Mail: twoflyliu@163.com
* Create time: 2017 11 12 11:35:24
*/
#include "rmap.h"
#include "leak.h"

#include <assert.h>
#include <string.h>

typedef void* any_t; // 任何用户数据类型

// 表示红黑树的颜色
typedef enum rb_color_t{
    RED = 0,
    BLACK
} rb_color_t;


// 表示红黑树的一个节点
typedef struct rb_node_t {
    // 下面四个字段构成红黑树的基本结构
    struct rb_node_t *left;
    struct rb_node_t *right;
    struct rb_node_t *parent;
    rb_color_t color;

    any_t data; // 保存的真正用户需要的数据
} rb_node_t;

// 提供辅助函数
static inline rb_node_t* parent(rb_node_t *node)
{
    if (node == NULL) return NULL;
    return node->parent;
}

static inline rb_node_t* grandparent(rb_node_t *node)
{
    rb_node_t *p  = parent(node);
    if (p != NULL) return parent(p);
    return NULL;
}

static inline rb_node_t* uncle(rb_node_t *node)
{
    rb_node_t *p = parent(node);
    if (p != NULL) {
        if (node == p->left) {
            return p->right;
        } else {
            return p->left;
        }
    }
    return NULL;
}

static inline rb_node_t *sibling(rb_node_t *node)
{
    if (parent(node)->left == node) {
        return parent(node)->right;
    }
    return parent(node)->left;
}


// 表示红黑树，内部维护了根节点
// 和比较算法
typedef struct rb_tree_t {
    rb_node_t *root;
    rb_node_t *NIL;

    size_t len;

    size_t key_size;        //表示key占用的尺寸
    size_t value_size;      //value占用的尺寸
    cmp_func_t cmp;         //比较算法
    key_destroy_func_t key_destroy;
    value_destroy_func_t value_destroy;
} rb_tree_t;

static rb_node_t *rb_tree_smallest(rb_tree_t *tree, rb_node_t *node)
{
    while (node->left != tree->NIL) {
        node = node->left;
    }
    return node;
}

// 提供基本的辅助函数

// 以x为支点进行左旋转
static void rb_tree_rotate_left(rb_tree_t *tree, rb_node_t *x)
{
    assert(x->right && "left rotate && the right son of x can't be null");
    // y节点的左子节点设置为x节点的右子节点 
    rb_node_t *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;

    // 建立y节点x原来父节点的连接
    y->parent = x->parent;
    if (x->parent == NULL) { // 建立x原来父节点到y节点之间的连接
        tree->root = y;
    } else if (x->parent->left == x) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    // 设置x和y之间的相互连接关系
    y->left = x;
    x->parent = y;
}

// 以y为支点进行右旋转
static void rb_tree_rotate_right(rb_tree_t *tree, rb_node_t *y)
{
    assert(y->left && "right rotate && the left son of y can't be null");
    // 建立y左子树和x右子树之间的连接关系
    rb_node_t *x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;

    // 建立x父亲和y原来的父亲之间的链接关系
    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y->parent->left == y) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    // 建立x和y之间的连接关系
    x->right = y;
    y->parent = x;
}

static inline void* rb_tree_get_key(rb_tree_t UNUSED *tree, rb_node_t *node)
{
    return node->data;
}

static inline void* rb_tree_get_value(rb_tree_t *tree, rb_node_t *node)
{
    return (char*)node->data + tree->key_size;
}

static inline int rb_tree_cmp_node(rb_tree_t *tree, rb_node_t *left, rb_node_t *right)
{
    return tree->cmp(rb_tree_get_key(tree, left),
            rb_tree_get_key(tree, right));
}


static void rb_tree_insert_rebalance(rb_tree_t *tree, rb_node_t *z);
static void rb_tree_insert_node(rb_tree_t *tree, rb_node_t *z)
{
    rb_node_t *y = NULL;
    rb_node_t *x = tree->root;

    // 查找插入点的位置（叶子结点）
    while (x != NULL && x != tree->NIL) {
        y = x;
        if (rb_tree_cmp_node(tree, z, x) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL) {
        tree->root = z;
    } else if (rb_tree_cmp_node(tree, z, y) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    // 初始化z的状态
    z->left = z->right = tree->NIL;
    z->color = RED;
    rb_tree_insert_rebalance(tree, z);
}

static inline void rb_tree_destroy_value(rb_tree_t *tree, void *value) {
    if (tree->value_destroy) {
        (*tree->value_destroy)(value);
    }
}

static bool rb_tree_set_value(rb_tree_t *tree, const void *key, const void *value);
static rb_node_t *rb_tree_find_node(rb_tree_t *tree, const void *key);
static void* rb_tree_find(rb_tree_t *tree, const void* key);

static void rb_tree_insert(rb_tree_t *tree, const void *key, const void *value)
{
    if (!rb_tree_set_value(tree, key, value)) { //创建新的
        ++ tree->len;
        rb_node_t *node = (rb_node_t*)cstl_malloc(sizeof(rb_node_t));
        node->data = cstl_malloc(tree->key_size + tree->value_size);
        memmove(node->data, key, tree->key_size);
        memmove((char*)node->data + tree->key_size, value, tree->value_size);
        rb_tree_insert_node(tree, node);
    }
}

// 只有当值存在的实话才会设置
static bool rb_tree_set_value(rb_tree_t *tree, const void *key, const void *value)
{
    void* val = rb_tree_find(tree, key);
    if (val != NULL) {
        rb_tree_destroy_value(tree, val); //先销毁原来的
        memmove(val, value, tree->value_size); // 设置新的
        return true;
    } 
    return false;
}

static void rb_tree_insert_rebalance(rb_tree_t *tree, rb_node_t *z)
{
    if (NULL == z->parent) {
        z->color = BLACK; // 根节点颜色设置为黑色就可以
    } else if (parent(z)->color == BLACK) {
        //nothing to dao
    } else if (uncle(z) && uncle(z)->color == RED) {
        uncle(z)->color = parent(z)->color = BLACK;
        grandparent(z)->color = RED;
        rb_tree_insert_rebalance(tree, grandparent(z));
    } else { //uncle节点要么不存在，要么为黑色
        // 处理后，s, p和p位于同一侧
        // 原来s和p有4种排序，现在经过转换只能能有2两
        // 要么都是位于左侧，要么都是位于右侧
        // (思想是一种：归一，将可变的情况，尽量变的单一化)
        if (z == parent(z)->right && parent(z) == grandparent(z)->left) {
            rb_tree_rotate_left(tree, parent(z));
            z = z->left;
        } else if (z == parent(z)->left && parent(z) == grandparent(z)->right) {
            rb_tree_rotate_right(tree, parent(z));
            z = z->right;
        }

        parent(z)->color = BLACK;
        grandparent(z)->color = RED;
        if (z == parent(z)->left && parent(z) == grandparent(z)->left) {
            rb_tree_rotate_right(tree, grandparent(z));
        } else {
            //z == parent(z)->right && parent(z) == grandparent(z)->right
            rb_tree_rotate_left(tree, grandparent(z));
        }
    }
}

static void rb_tree_swap(rb_tree_t *tree, rb_node_t *left, rb_node_t *right)
{
    size_t entry_size = tree->key_size + tree->value_size;
    char tmp[entry_size];
    memcpy(tmp, left->data, entry_size);
    memcpy(left->data, right->data, entry_size);
    memcpy(right->data, tmp, entry_size);
}

static void rb_tree_destroy_node(rb_tree_t *tree, rb_node_t *node);

static void rb_tree_erase_rebalance(rb_tree_t *tree, rb_node_t *z);
static void rb_tree_erase(rb_tree_t *tree, rb_node_t *z)
{
    rb_node_t *child;

    assert(z && "to removed node can't be null");
    if (z->left != tree->NIL && z->right != tree->NIL) {
        rb_node_t *smallest = rb_tree_smallest(tree, z->right);
        rb_tree_swap(tree, smallest, z);
        z = smallest;
    } 

    // 可能是NIL，可能 不是NIL
    child = (z->left == tree->NIL) ? z->right : z->left;

    // 要删除的节点是树上唯一一个节点
    if (z->parent == NULL && z->left == tree->NIL && z->right == tree->NIL) {
        tree->root = NULL;
        goto destroy_node;
    }

    // 要删除的节点是根节点，并且整棵树还有其他节点
    if (z->parent == NULL) {
        child->parent = NULL;
        child->color = BLACK;
        tree->root = child;
        goto destroy_node;
    }

    // p不是根节点，则肯定有父亲
    if (parent(z)->left == z) {
        parent(z)->left = child;
    } else {
        parent(z)->right = child;
    }
    child->parent = z->parent;

    // 只有删除颜色为黑色，才可能要调整，如果删除为红色，则不用调整
    if (z->color == BLACK) {
        if (child->color == RED) {
            child->color = BLACK;
        } else { //删除点是黑色，孩子也是黑色
            rb_tree_erase_rebalance(tree, child);
        }
    }
destroy_node:
    rb_tree_destroy_node(tree, z);
    -- tree->len;
}

static void rb_tree_erase_rebalance(rb_tree_t *tree, rb_node_t *n)
{
    rb_node_t *s = sibling(n);
    if (n->parent == NULL) return;

    if (s->color == RED) {
        n->parent->color = RED;
        s->color = BLACK;
        if (n == parent(n)->left) {
            rb_tree_rotate_left(tree, parent(n));
        } else {
            rb_tree_rotate_right(tree, parent(n));
        }
        s = sibling(n);
    }

    if (parent(n)->color == BLACK && s->color == BLACK &&
            s->left->color==BLACK && s->right->color==BLACK) {
        return rb_tree_erase_rebalance(tree, n);
    } else if (parent(n)->color == RED && s->color == BLACK
            && s->left->color == BLACK && s->right->color == BLACK) {
        s->color = RED;
        parent(n)->color = BLACK;
    } else if (s->color == BLACK) {
        if (n == parent(n)->left && BLACK == s->right->color && RED == s->left->color) {
            s->color = RED;
            s->left->color = BLACK;
            rb_tree_rotate_right(tree, s);
        } else if (n == parent(n)->right && BLACK == s->left->color &&
            RED == s->right->color) {
            s->color = RED;
            s->right->color = BLACK;
            rb_tree_rotate_left(tree, s);
        }
        s = sibling(n);
    }

    s->color = n->parent->color;
    n->parent->color = BLACK;

    if (n == parent(n)->left) {
        s->right->color = BLACK;
        rb_tree_rotate_left(tree, parent(n));
    } else {
        s->left->color = BLACK;
        rb_tree_rotate_right(tree, parent(n));
    }
}

static void rb_tree_destroy_node(rb_tree_t *tree, rb_node_t *node)
{
    assert(tree && node);
    if (tree->key_destroy) {
        (*tree->key_destroy)(rb_tree_get_key(tree, node));
    }
    if (tree->value_destroy) {
        (*tree->value_destroy)(rb_tree_get_value(tree, node));
    }
    if (node->data) cstl_free(node->data);
    cstl_free(node);
}

static inline void _rb_tree_free_all_node(rb_tree_t *tree, rb_node_t *node)
{
    if (node == tree->NIL || NULL == node) return;
    _rb_tree_free_all_node(tree, node->left);
    _rb_tree_free_all_node(tree, node->right);
    rb_tree_destroy_node(tree, node);
}

// 释放内部所有资源
static void rb_tree_free(rb_tree_t *tree)
{
    _rb_tree_free_all_node(tree, tree->root);
    tree->root = NULL;
    tree->len = 0;
}

static void* rb_tree_find(rb_tree_t *tree, const void* key)
{
    rb_node_t *node = rb_tree_find_node(tree, key);
    return (node == NULL) ? NULL : rb_tree_get_value(tree, node);
}

static rb_node_t *rb_tree_find_node(rb_tree_t *tree, const void *key)
{
    rb_node_t *node = tree->root;
    if (node == NULL) return NULL;

    while (node != tree->NIL) {
        int rs = (*tree->cmp)(key, rb_tree_get_key(tree, node));
        if (0 == rs) {
            return node;
        } else if (rs < 0 ) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    
    return NULL;
}

static void rb_tree_for_each_impl(rb_tree_t *tree, rb_node_t *node, RMAP_FOR_EACH each, void *user_data)
{
    if (NULL == node || tree->NIL == node) return;
    each(rb_tree_get_key(tree, node), rb_tree_get_value(tree, node), user_data);
    rb_tree_for_each_impl(tree, node->left, each, user_data);
    rb_tree_for_each_impl(tree, node->right, each, user_data);
}

static void rb_tree_for_each(rb_tree_t *tree, RMAP_FOR_EACH each, void *data)
{
    if (NULL == tree->root || tree->NIL == tree->root ||  NULL == each) return;
    rb_tree_for_each_impl(tree, tree->root, each, data);
}

// rmap_t
typedef struct rmap_t {
    rb_tree_t tree;
} rmap_t;

CSTL_LIB rmap_t *rmap_new(size_t key_size, size_t value_size, 
        cmp_func_t key_cmp_func)
{
    return rmap_new_with_destroy_func(key_size, value_size, key_cmp_func,
            NULL, NULL);
}

CSTL_LIB rmap_t *rmap_new_with_destroy_func(size_t key_size, size_t value_size, 
        key_cmp_func_t key_cmp_func,
        key_destroy_func_t key_destroy,
        value_destroy_func_t val_destroy)
{
    rmap_t *rmap = (rmap_t*)cstl_malloc(sizeof(rmap_t));

    rmap->tree.root = NULL;
    rmap->tree.NIL = (rb_node_t*)cstl_malloc(sizeof(rb_node_t));
    rmap->tree.NIL->color = BLACK;

    rmap->tree.len = 0;

    rmap->tree.key_size = key_size;
    rmap->tree.value_size = value_size;
    rmap->tree.cmp = key_cmp_func;
    rmap->tree.key_destroy = key_destroy;
    rmap->tree.value_destroy = val_destroy;

    return rmap;
}

CSTL_LIB void rmap_free(rmap_t *rmap)
{
    assert(rmap && "rmap cannot be null");
    rb_tree_free(&rmap->tree);
    cstl_free(rmap->tree.NIL);
    cstl_free(rmap);
}

CSTL_LIB void rmap_insert(rmap_t *rmap, const void *key, const void *value)
{
    assert(rmap && key && value && "rmap key value cannot be null");
    rb_tree_insert(&rmap->tree, key, value);
}

CSTL_LIB void rmap_erase(rmap_t *rmap, const void *key)
{
    assert(rmap && key && "rmap key cannot be null");
    rb_node_t *node = rb_tree_find_node(&rmap->tree, key);
    if (node != NULL) {
        rb_tree_erase(&rmap->tree, node);
    }
}

CSTL_LIB void *rmap_get(const rmap_t *rmap, const void *key)
{
    assert(rmap && key && "rmap key cannot be null");
    return rb_tree_find((rb_tree_t*)&rmap->tree, key);
}

CSTL_LIB bool rmap_has_key(const rmap_t *rmap, const void *key)
{
    assert(rmap && key && "rmap key cannot be null");
    return NULL != rmap_get(rmap, key);
}

CSTL_LIB void rmap_set(rmap_t *rmap, const void *key, const void *new_value)
{
    assert(rmap && key && new_value && "rmap key new_value cannot be null");
    rb_tree_set_value(&rmap->tree, key, new_value);
}

CSTL_LIB void rmap_clear(rmap_t *rmap)
{
    assert(rmap && "rmap cannot be null");
    rb_tree_free(&rmap->tree);
    rmap->tree.root = NULL;
}

CSTL_LIB size_t rmap_size(const rmap_t *rmap)
{
    assert(rmap && "rmap cannot be null");
    return rmap->tree.len;
}

CSTL_LIB bool rmap_empty(const rmap_t *rmap)
{
    return rmap_size(rmap) == 0;
}

CSTL_LIB void rmap_for_each(rmap_t* rmap, RMAP_FOR_EACH for_each_func, void *user_data)
{
    assert(rmap && "rmap cannot be null");
    rb_tree_for_each(&rmap->tree, for_each_func, user_data);
}
