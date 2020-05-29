/**
 * @file rb-tree.c
 * @author BlaCkinkGJ (ss5kijun@gmail.com)
 * @brief red black tree implementation
 * @version 0.1
 * @date 2020-05-29
 * 
 * @copyright Copyright (c) 2020 BlaCkinkGJ
 * 
 */
#include <stdlib.h>
#include "rb-tree.h"

/**
 * @brief Allocation of red-black tree
 * 
 * @return struct rb_tree* allocated red-black tree
 */
struct rb_tree *rb_tree_alloc()
{
        struct rb_tree *tree = (struct rb_tree *)malloc(sizeof(struct rb_tree));
        if (!tree) {
                pr_info("Memory shortage detected! Allocation failed...");
                goto exception;
        }

        tree->root = NULL;

        tree->leaf = (struct rb_node *)malloc(sizeof(struct rb_node));
        if (!tree->leaf) {
                pr_info("Memory shortage detected! Allocation failed...");
                goto exception;
        }
        rb_set_nil_leaf_node(tree->leaf);

        return tree;
exception:
        if (tree->leaf) {
                free(tree->leaf);
                tree->leaf = NULL;
        }
        if (tree) {
                free(tree);
        }
        return NULL;
}

/**
 * @brief Count the number of black colored nodes that belong to the subtree.
 * However, you must not count the color of the root itself.
 * 
 * @param tree red-black tree whole
 * @param root the root of the subtree
 * @return int the number of black colored nodes in the subtree.
 */
static int rb_tree_get_bh(struct rb_tree *tree, struct rb_node *root)
{
        int left_bh, right_bh;
        if (!root) {
                return 1;
        }

        left_bh = rb_tree_get_bh(tree, root->left);
        if (left_bh == 0) {
                return left_bh;
        }

        right_bh = rb_tree_get_bh(tree, root->right);
        if (right_bh == 0) {
                return right_bh;
        }

        if (left_bh != right_bh) {
                return 0;
        }

        return left_bh + (rb_node_is_black(root) ? 1 : 0);
}

static void rb_tree_left_rotate(struct rb_tree *tree, struct rb_node *node)
{
        struct rb_node *right = node->right; /**< set right node */

        node->right = right->left; /**< move subtree */
        if (right->left != tree->leaf) {
                node->left->parent = right;
        }
        right->parent = node->parent; /**< change parents */

        if (node->parent == tree->leaf) {
                tree->root = right;
        } else if (node == node->parent->left) {
                node->parent->left = right;
        } else {
                node->parent->right = right;
        }

        right->left = node;
        node->parent = right;
}

/**
 * @brief Does deallocation of the red-black tree subtree
 * 
 * @param tree red-black tree whole
 * @param node the root of the subtree
 */
static void __rb_tree_dealloc(struct rb_tree *tree, struct rb_node *node)
{
        if (!node || rb_node_is_leaf(tree, node)) {
                return;
        }

        dealloc_rb_node(tree, node->left);
        node->left = NULL;
        dealloc_rb_node(tree, node->right);
        node->right = NULL;

        free(node);
}

/**
 * @brief Does deallocation fo the red-black tree
 * 
 * @param tree red-black tree whole
 */
void rb_tree_dealloc(struct rb_tree *tree)
{
        __rb_tree_dealloc(tree, tree->root);
        tree->root = NULL;

        free(tree->leaf);
        tree->leaf = NULL;

        free(tree);
}
