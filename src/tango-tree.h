/**
 * @file tango-tree.h
 * @author BlaCkinkGJ (ss5kijun@gmail.com)
 * @brief tango tree data structure's declaration part
 * @version 0.1
 * @date 2020-06-08
 * 
 * @copyright Copyright (c) 2020 BlaCkinkGJ
 * 
 */
#ifndef TANGO_TREE_H_
#define TANGO_TREE_H_

#include "rb-tree.h"

struct tango_node {
        struct rb_tree *aux_tree;
        struct tango_node *left;
        struct tango_node *right;
};

struct tango_tree {
        struct tango_node *root;
};

struct tango_node *tango_tree_search(struct tango_tree *tree, key_t key);
void tango_tree_cut(struct tango_tree *tree, key_t key);
struct tang_tree *tango_tree_join(struct tango_tree *tree,
                                  struct tango_node *aux1,
                                  struct tango_node *aux2);

#endif