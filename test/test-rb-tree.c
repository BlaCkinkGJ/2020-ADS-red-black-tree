#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "rb-tree.h"
#include "unity.h"

#define INSERT_SIZE (1000)
#define STR_BUF_SIZE (256)

struct rb_tree *tree;
key_t *key_arr;
char **data_arr;

void setUp(void)
{
        tree = rb_tree_alloc();
        key_arr = (key_t *)malloc(sizeof(key_t) * INSERT_SIZE);
        data_arr = (char **)malloc(sizeof(char *) * INSERT_SIZE);
        TEST_ASSERT_NOT_NULL(tree);
}

void tearDown(void)
{
        rb_tree_dealloc(tree);

        free(key_arr);
        free(data_arr);
}

void test_rb_insert(void)
{
        for (int i = 0; i < INSERT_SIZE; i++) {
                key_arr[i] = rand() % INSERT_SIZE;
        }

        for (int i = 0; i < INSERT_SIZE; i++) {
                char *data = (char *)malloc(sizeof(char) * STR_BUF_SIZE);
                sprintf(data, "%d", i);
                data_arr[key_arr[i]] = data;
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, key_arr[i], data));
        }
}

void test_rb_valid_search(void)
{
        test_rb_insert(); /**< Generate Test Cases */
        for (int i = 0; i < INSERT_SIZE; i++) {
                struct rb_node *node = rb_tree_search(tree, key_arr[i]);
                TEST_ASSERT_NOT_NULL(node);
                TEST_ASSERT_EQUAL(key_arr[i], node->key);
                TEST_ASSERT_EQUAL_STRING(data_arr[key_arr[i]], node->data);
        }
}

void test_rb_invalid_search(void)
{
        test_rb_insert(); /**< Generate Test Cases */
        for (key_t key = INSERT_SIZE; key < 2 * INSERT_SIZE; key++) {
                TEST_ASSERT_NULL(rb_tree_search(tree, key));
        }
        TEST_ASSERT_NOT_NULL(rb_tree_search(tree, key_arr[INSERT_SIZE - 1]));
}

void test_rb_minimum(void)
{
        key_t values[] = { 10, 35, 5, 22 };
        const int nr_values = (int)(sizeof(values) / sizeof(key_t));
        for (int i = 0; i < nr_values; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, values[i], NULL));
        }
        TEST_ASSERT_EQUAL(5, rb_tree_minimum(tree, tree->root)->key);
}

void test_rb_maximum(void)
{
        key_t values[] = { 10, 35, 5, 22 };
        const int nr_values = (int)(sizeof(values) / sizeof(key_t));
        for (int i = 0; i < nr_values; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, values[i], NULL));
        }
        TEST_ASSERT_EQUAL(35, rb_tree_maximum(tree, tree->root)->key);
}

void test_rb_successor_and_predecessor(void)
{
        struct rb_node *succ, *pred, *cur;
        int i = 0;
        key_t values[] = { 10, 35, 5, 22 };
        key_t expects[] = { -1, 5, 10, 22, 35, -1 };
        const int nr_expects = (int)(sizeof(expects) / sizeof(key_t));
        const int nr_values = (int)(sizeof(values) / sizeof(key_t));
        for (int i = 0; i < nr_values; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, values[i], NULL));
        }

        cur = rb_tree_minimum(tree, tree->root);
        for (i = 0; i < (nr_expects - 1) && cur != tree->nil; i++) {
                succ = rb_tree_successor(tree, cur);
                pred = rb_tree_predecessor(tree, cur);

                TEST_ASSERT_NOT_NULL(cur);
                TEST_ASSERT_NOT_NULL(succ);
                TEST_ASSERT_NOT_NULL(pred);

                cur = succ;

                TEST_ASSERT_EQUAL(expects[i], pred->key);
                TEST_ASSERT_EQUAL(expects[i + 2], succ->key);
        }
        TEST_ASSERT_EQUAL(tree->nil, cur);
        TEST_ASSERT_EQUAL(i, nr_expects - 2);
}

void test_rb_delete(void)
{
        struct rb_node *node;
        key_t values[] = { 10, 35, 5, 22 };
        const int nr_values = (int)(sizeof(values) / sizeof(key_t));
        for (int i = 0; i < nr_values; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, values[i], NULL));
        }
        for (int i = 0; i < nr_values; i++) {
                node = rb_tree_search(tree, values[i]);
                TEST_ASSERT_NOT_NULL(node);
                TEST_ASSERT_EQUAL(0, rb_tree_delete(tree, values[i]));
                node = rb_tree_search(tree, values[i]);
                TEST_ASSERT_NULL(node);
                TEST_ASSERT_EQUAL(-ENODATA, rb_tree_delete(tree, values[i]));
        }

        TEST_ASSERT_EQUAL_PTR(tree->nil, tree->root);
}

void test_rb_bh(void)
{
        key_t insert_seq[] = { 10, 20, 5,  7,  6,  19, 18,
                               17, 16, 15, 21, 22, 14, 13 };
        key_t delete_seq[] = { 10, 6,  5,  16, 7,  13, 15,
                               14, 21, 20, 22, 18, 19, 17 };
        size_t insert_bh[] = { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 };
        size_t delete_bh[] = { 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 0 };
        const int nr_inserts = (int)(sizeof(insert_seq) / sizeof(key_t));
        const int nr_deletes = (int)(sizeof(delete_seq) / sizeof(key_t));
        for (int i = 0; i < nr_inserts; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, insert_seq[i], NULL));
                TEST_ASSERT_EQUAL(insert_bh[i], tree->bh);
        }

        for (int i = 0; i < nr_deletes; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_delete(tree, delete_seq[i]));
#ifdef RB_TREE_DEBUG
                rb_tree_dump(tree, tree->root, 0);
                pr_info("=============================\n");
#endif
                TEST_ASSERT_EQUAL(delete_bh[i], tree->bh);
        }

        for (int i = 0; i < nr_inserts; i++) {
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, insert_seq[i], NULL));
                TEST_ASSERT_EQUAL(insert_bh[i], tree->bh);
        }
}

int main(void)
{
        UNITY_BEGIN();

        RUN_TEST(test_rb_insert);
        RUN_TEST(test_rb_valid_search);
        RUN_TEST(test_rb_invalid_search);
        RUN_TEST(test_rb_minimum);
        RUN_TEST(test_rb_maximum);
        RUN_TEST(test_rb_successor_and_predecessor);
        RUN_TEST(test_rb_delete);
        RUN_TEST(test_rb_bh);

        return UNITY_END();
}