/**
 * @file rb-tree.h
 * @author BlaCkinkGJ (ss5kijun@gmail.com)
 * @brief red black tree data structure's declaration part
 * @version 0.1
 * @date 2020-05-29
 * 
 * @copyright Copyright (c) 2020 BlaCkinkGJ
 * 
 */
#ifndef RB_TREE_H_
#define RB_TREE_H_

#include <stdio.h>
#include <time.h>
#include <stdint.h>

#ifndef key_t
#warning "already key_t is defined"
typedef uint64_t key_t;
#endif

#ifndef pr_info
#define pr_info(msg, ...)                                                      \
        fprintf(stderr, "[{%lfs} %s(%s):%d] " msg,                             \
                ((double)clock() / CLOCKS_PER_SEC), __FILE__, __func__,        \
                __LINE__, ##__VA_ARGS__)
#endif

/**
 * @brief rb_node's color type
 * 
 */
enum rb_node_color {
        RB_NODE_COLOR_RED,
        RB_NODE_COLOR_BLACK,
};

/**
 * @brief red black tree's node
 * 
 */
struct rb_node {
        enum rb_node_color color;
        key_t key;
        void *data;
        struct rb_node *left, *right;
        struct rb_node *parent; /**< same as P in CLRS books */
};

/**
 * @brief red black tree structure
 * 
 */
struct rb_tree {
        struct rb_node *root;
        struct rb_node *leaf; /**< same as Nil in CLRS books */
};

/**
 * @brief red-black tree color test
 * 
 * @param node color test target node
 * @return true node color is black
 * @return false node color is not black
 */
static inline bool rb_node_is_black(struct rb_node *node)
{
        return node->color == RB_NODE_COLOR_BLACK;
}

/**
 * @brief Node check if the node is equal to tree->leaf
 * 
 * @param tree red-black tree whole
 * @param node leaf test target node
 * @return true node is leaf
 * @return false node is not leaf
 */
static inline bool rb_node_is_leaf(struct rb_tree *tree, struct rb_node *node)
{
        return tree->leaf == node;
}

/**
 * @brief set specific node to leaf
 * 
 * @param leaf the node which wants to make a leaf.
 */
static inline void rb_set_nil_leaf_node(struct rb_node *leaf)
{
        leaf->color = RB_NODE_COLOR_BLACK;
        leaf->parent = leaf->left = leaf->right = NULL;
        leaf->data = NULL;
        leaf->key = 0;
}

#endif