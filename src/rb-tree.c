/**
 * @file rb-tree.c
 * @author BlaCkinkGJ (ss5kijun@gmail.com)
 * @brief red black tree implementation
 * @version 0.1
 * @date 2020-05-29
 * 
 * @ref Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to algorithms. MIT press.
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
struct rb_tree *rb_tree_alloc(void)
{
        struct rb_tree *tree = (struct rb_tree *)malloc(sizeof(struct rb_tree));
        if (!tree) {
                pr_info("Memory shortage detected! Allocation failed...");
                goto exception;
        }

        tree->root = NULL;

        tree->nil = (struct rb_node *)malloc(sizeof(struct rb_node));
        if (!tree->nil) {
                pr_info("Memory shortage detected! Allocation failed...");
                goto exception;
        }
        rb_set_nil_leaf_node(tree->nil);

        tree->root = tree->nil;

        return tree;
exception:
        if (tree->nil) {
                free(tree->nil);
                tree->nil = NULL;
        }

        if (tree) {
                free(tree);
        }
        return NULL;
}

/**
 * @brief Red-black tree left rotation
 *     (x)                    (y)
 *    ／  ＼                 ／  ＼
 *   α     (y)     ==>     (x)    γ
 *        ／  ＼          ／  ＼
 *        β    γ         α     β
 * 
 * @param tree red-black tree structure
 * @param x subtree's root node
 */
static void rb_tree_left_rotate(struct rb_tree *tree, struct rb_node *x)
{
        struct rb_node *y = NULL;

        y = x->right; /**< set right node */

        x->right = y->left; /**< move subtree */
        if (y->left != tree->nil) {
                y->left->parent = x;
        }
        y->parent = x->parent; /**< change parents */

        if (x->parent == tree->nil) {
                tree->root = y;
        } else if (x == x->parent->left) {
                x->parent->left = y;
        } else {
                x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
}

/**
 * @brief Red-black tree left rotation
 *      (y)              (x)
 *     ／  ＼           ／  ＼
 *   (x)     γ  ==>    α     (y)
 *  ／  ＼                  ／  ＼
 * α     β                 β     γ
 * 
 * @param tree red-black tree structure
 * @param x subtree's root node
 */
static void rb_tree_right_rotate(struct rb_tree *tree, struct rb_node *y)
{
        struct rb_node *x = NULL;

        x = y->left; /**< set left node */

        y->left = x->right; /**< move subtree */
        if (x->right != tree->nil) {
                x->right->parent = y;
        }
        x->parent = y->parent; /**< change parents */

        if (y->parent == tree->nil) {
                tree->root = x;
        } else if (y == y->parent->right) {
                y->parent->right = x;
        } else {
                y->parent->left = x;
        }

        x->right = y;
        y->parent = x;
}

/**
 * @brief Search red-black tree's node which the same value of key.
 * Based on binary search method
 * 
 * @param root red-black tree's root
 * @param key the key which I want to search
 * @return struct rb_node* if find success then return specific node pointer.
 * but if find failed then return NULL pointer
 * 
 * @ref Horowitz, E., Sahni, S., & Anderson-Freed, S. (1992). Fundamentals of data structures in C. WH Freeman & Co..
 */
static struct rb_node *__rb_tree_search(struct rb_node *root, key_t key)
{
        struct rb_node *node = root;
        while (node) {
                if (key == node->key) { /**< Find the specific values */
                        break;
                }

                if (key < node->key) {
                        node = node->left;
                } else {
                        node = node->right;
                }
        }

        return node;
}

/**
 * @brief Wrapping function of `__rb_tree_search` function
 * 
 * @param tree 
 * @param key 
 * @return struct rb_node* 
 */
struct rb_node *rb_tree_search(struct rb_tree *tree, key_t key)
{
        return __rb_tree_search(tree->root, key);
}

/**
 * @brief Re-color nodes and perform rotations
 * @details
 * case 1: z's uncle is y is red
 * case 2: z's uncle is y is black and z is a right child
 * case 3: z's uncle y is black and z is a left child
 * 
 * @param tree red-black tree structure
 * @param z new node which insert into red-black tree
 */
static void rb_insert_fixup(struct rb_tree *tree, struct rb_node *z)
{
        struct rb_node *y = NULL;

        while (z->parent->color == RB_NODE_COLOR_RED) {
                if (z->parent == z->parent->parent->left) {
                        y = z->parent->parent->right;
                        if (y->color == RB_NODE_COLOR_RED) { /**< case 1 */
                                z->parent->color = RB_NODE_COLOR_BLACK;
                                y->color = RB_NODE_COLOR_BLACK;
                                z->parent->parent->color = RB_NODE_COLOR_RED;
                                z = z->parent->parent;
                        } else {
                                if (z == z->parent->right) { /**< case 2 */
                                        z = z->parent;
                                        rb_tree_left_rotate(tree, z);
                                }
                                z->parent->color =
                                        RB_NODE_COLOR_BLACK; /**< case 3 */
                                z->parent->parent->color = RB_NODE_COLOR_RED;
                                rb_tree_right_rotate(tree, z->parent->parent);
                        }
                } else { /**< only different part is left and right */
                        y = z->parent->parent->left;
                        if (y->color == RB_NODE_COLOR_RED) {
                                z->parent->color = RB_NODE_COLOR_BLACK;
                                y->color = RB_NODE_COLOR_BLACK;
                                z->parent->parent->color = RB_NODE_COLOR_RED;
                                z = z->parent->parent;
                        } else {
                                if (z == z->parent->left) {
                                        z = z->parent;
                                        rb_tree_right_rotate(tree, z);
                                }
                                z->parent->color = RB_NODE_COLOR_BLACK;
                                z->parent->parent->color = RB_NODE_COLOR_RED;
                                rb_tree_left_rotate(tree, z->parent->parent);
                        }
                }
        }
        tree->root->color = RB_NODE_COLOR_BLACK;
}

/**
 * @brief Insert node to red-black tree
 * 
 * @param tree red-black tree structure
 * @param z new node which insert into red-black tree
 * @return int successfully insert status (0: success, else: fail)
 */
static int __rb_tree_insert(struct rb_tree *tree, struct rb_node *z)
{
        struct rb_node *y = NULL;
        struct rb_node *x = NULL;

        if (z == NULL) {
                pr_info("z must be not null");
                return -EINVAL;
        }

        y = tree->nil;
        x = tree->root;
        while (x != tree->nil) {
                if (x->key == z->key) { /**< update key's data */
                        rb_node_move(x, z);
                        return 0;
                }
                y = x;
                if (z->key < x->key) {
                        x = x->left;
                } else {
                        x = x->right;
                }
        } /**< traverse valid insert location */

        z->parent = y;
        if (y == tree->nil) { /**< set y state */
                tree->root = z;
        } else if (z->key < y->key) {
                y->left = z;
        } else {
                y->right = z;
        }

        z->left = tree->nil;
        z->right = tree->nil;
        z->color = RB_NODE_COLOR_RED;

        rb_insert_fixup(tree, z);

        return 0;
}

/**
 * @brief Wrapping function of `__rb_tree_insert`
 * 
 * @param tree red-black tree structure
 * @param key new node's key
 * @param data new node's data
 * @return int successfully insert status (0: success, else: fail)
 */
int rb_tree_insert(struct rb_tree *tree, const key_t key, void *data)
{
        struct rb_node *node = rb_node_alloc(key);
        if (!node) {
                pr_info("Allocate the node failed");
                return -ENOMEM;
        }

        node->data = data;

        return __rb_tree_insert(tree, node);
}

/**
 * @brief Translant previous root to next root
 * 
 * @param tree red-black tree whole
 * @param prev_root previous root node
 * @param next_root next root node which want to transplant
 */
static void rb_tree_transplant(struct rb_tree *tree, struct rb_node *prev_root,
                               struct rb_node *next_root)
{
        if (prev_root->parent == tree->nil) {
                tree->root = next_root;
        } else if (prev_root == prev_root->parent->left) {
                prev_root->parent->left = next_root;
        } else {
                prev_root->parent->right = next_root;
        }

        next_root->parent = prev_root->parent;
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

        __rb_tree_dealloc(tree, node->left);
        node->left = NULL;
        __rb_tree_dealloc(tree, node->right);
        node->right = NULL;

        rb_node_dealloc(node);
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

        rb_node_dealloc(tree->nil);
        tree->nil = NULL;

        free(tree);
}
