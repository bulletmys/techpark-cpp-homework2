#include <gtest/gtest.h>
#include <random>
#include <cstddef>


#define ARRAY_SIZE 100*1024*1024

extern "C" {
#include "logic_pro.h"
#include "logic_easy.h"
#include "input_helpers.h"
}

TEST(EasyTest, start) {
    EXPECT_EQ(10, 10);
}

TEST(EasyTest, size_10) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (int i = 0; i < 5; ++i) {
        array[i] = 1024 + i;
    }
    unsigned long long int res = sum_easy(array, 5);
    free(array);
    EXPECT_EQ(res, 10);
}

TEST(EasyTest, size_0) {
    int *array = (int *) malloc(ARRAY_SIZE);

    unsigned long long int res = sum_easy(array, 0);
    free(array);
    EXPECT_EQ(res, 0);
}

TEST(ProTest, fork_1) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 5; ++i) {
        array[i] = 1024 + i;
    }
    unsigned long long int res = sum_pro(array, 5, 1);
    free(array);
    EXPECT_EQ(res, 10);
}

TEST(ProTest, fork_5) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 5; ++i) {
        array[i] = 1024 + i;
    }
    unsigned long long int res = sum_pro(array, 5, 5);
    free(array);
    EXPECT_EQ(res, 10);
}

TEST(CompareAnswers, size_10_fork_1) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 10; ++i) {
        array[i] = rand();
    }
    unsigned long long int res_pro = sum_pro(array, 10, 1);
    unsigned long long int res_easy = sum_easy(array, 10);
    free(array);
    EXPECT_EQ(res_easy, res_pro);
}

TEST(CompareAnswers, size_10_fork_5) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 10; ++i) {
        array[i] = rand();
    }
    unsigned long long int res_pro = sum_pro(array, 10, 5);
    unsigned long long int res_easy = sum_easy(array, 10);
    free(array);
    EXPECT_EQ(res_easy, res_pro);
}

TEST(CompareAnswers, size_big_fork_1) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 100000; ++i) {
        array[i] = rand();
    }
    unsigned long long int res_pro = sum_pro(array, 100000, 1);
    unsigned long long int res_easy = sum_easy(array, 100000);
    free(array);
    EXPECT_EQ(res_easy, res_pro);
}

TEST(CompareAnswers, size_big_fork_100) {
    int *array = (int *) malloc(ARRAY_SIZE);
    for (size_t i = 0; i < 100000; ++i) {
        array[i] = rand();
    }
    unsigned long long int res_pro = sum_pro(array, 100000, 100);
    unsigned long long int res_easy = sum_easy(array, 100000);
    free(array);
    EXPECT_EQ(res_easy, res_pro);
}



int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
