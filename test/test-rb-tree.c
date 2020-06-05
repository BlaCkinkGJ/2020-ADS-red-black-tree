#include <stdlib.h>
#include <time.h>

#include "rb-tree.h"
#include "unity.h"

#define INSERT_SIZE (100000)

struct rb_tree *tree;
key_t key_arr[INSERT_SIZE];
int data_arr[INSERT_SIZE];

void setUp(void)
{
        tree = rb_tree_alloc();
        TEST_ASSERT_NOT_NULL(tree);
}

void tearDown(void)
{
        rb_tree_dealloc(tree);
}

void test_rb_insert(void)
{
        for (int i = 0; i < INSERT_SIZE; i++) {
                key_arr[i] = rand() % INSERT_SIZE;
        }

        for (int i = 0; i < INSERT_SIZE; i++) {
                struct rb_node *z = rb_node_alloc(key_arr[i]);
                z->data = malloc(sizeof(int));
                *((int *)z->data) = i;
                data_arr[key_arr[i]] = i;
                TEST_ASSERT_EQUAL(0, rb_tree_insert(tree, z));
        }
}

void test_rb_valid_search(void)
{
        test_rb_insert(); /**< Generate Test Cases */
        for (int i = 0; i < INSERT_SIZE; i++) {
                struct rb_node *node = rb_tree_search(tree, key_arr[i]);
                TEST_ASSERT_NOT_NULL(node);
                TEST_ASSERT_EQUAL(key_arr[i], node->key);
                TEST_ASSERT_EQUAL(data_arr[key_arr[i]], *((int *)node->data));
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

int main(void)
{
        UNITY_BEGIN();
        RUN_TEST(test_rb_insert);
        RUN_TEST(test_rb_valid_search);
        RUN_TEST(test_rb_invalid_search);
        return UNITY_END();
}