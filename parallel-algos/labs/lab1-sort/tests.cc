#include <gtest/gtest.h>
#include <algorithm>
#include "sequential.h"
#include "parallel.h"

namespace {
    std::mt19937 rand_gen = std::mt19937(std::random_device()());

    size_t const int_array_size = 3;
    int int_array[int_array_size] = {2, 3, 1};
    int int_array_sorted[int_array_size] = {1, 2, 3};

    size_t const string_array_size = 5;
    std::string string_array[string_array_size] = {
            "abacaba",
            "abc",
            "ding-ding-ding",
            "aboba",
            "abobaa"
    };
    std::string string_array_sorted[string_array_size] = {
            "abacaba",
            "abc",
            "aboba",
            "abobaa",
            "ding-ding-ding",
    };

    std::vector<int> int_vector({5, 3, 2, 3, 1, 4});
    std::vector<std::string> string_vector({
        "aba",
        "abc",
        "dceff",
        "dcefa",
        "babbfde",
        "cde",
        "cdf",
        "aaa",
        "aba",
        "aaaa",
        "abc",
    });
}

TEST(SequentialSortTests, IntArray) {
    int a[int_array_size];
    for (size_t i = 0; i < int_array_size; ++i)
        a[i] = int_array[i];
    sequential::sort(a, a + int_array_size);
    for (size_t i = 0; i < int_array_size; ++i)
        EXPECT_EQ(a[i], int_array_sorted[i]);
}

TEST(SequentialSortTests, StringArray) {
    std::string a[string_array_size];
    for (size_t i = 0; i < string_array_size; ++i)
        a[i] = string_array[i];
    sequential::sort(a, a + string_array_size);
    for (size_t i = 0; i < string_array_size; ++i)
        EXPECT_EQ(a[i], string_array_sorted[i]);
}

TEST(SequentialSortTests, IntVector) {
    std::vector<int> a(int_vector.begin(), int_vector.end());
    std::vector<int> b(a.begin(), a.end());
    sequential::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    EXPECT_EQ(a, b);
}

TEST(SequentialSortTests, StringVector) {
    std::vector<std::string> a(string_vector.begin(), string_vector.end());
    std::vector<std::string> b(a.begin(), a.end());
    sequential::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    EXPECT_EQ(a, b);
}

template<decltype(sequential::sort<std::vector<int>::iterator>) sort_function>
void random_vector_int_test(size_t const sz, int const max_value) {
    std::vector<int> a(sz);
    for (size_t i = 0; i < sz; ++i) {
        a[i] = (int) (rand_gen() % max_value);
    }
    std::vector<int> b(a.begin(), a.end());
    sort_function(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    EXPECT_EQ(a, b);
}

#define REPEAT_NTIMES(n, action...) \
    for (int i = 0; i < n; ++i) {   \
        action;                     \
    }

int const REPEATS = 5;

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define MACRO_CONCAT3(x, y, z) MACRO_CONCAT(MACRO_CONCAT(x, y), z)
#define MACRO_CONCAT4(x, y, z) MACRO_CONCAT(MACRO_CONCAT(x, y), z)

#define RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, VECTOR_SIZE, MAX_VALUE)                                   \
TEST(GROUP_NAME, MACRO_CONCAT(MACRO_CONCAT(MACRO_CONCAT(RandomIntVector, VECTOR_SIZE), _), MAX_VALUE)) { \
    REPEAT_NTIMES(REPEATS, random_vector_int_test<SORT_FUNCTION>(VECTOR_SIZE, MAX_VALUE))                \
}

#define RANDOM_TESTS_GROUP(GROUP_NAME, SORT_FUNCTION)    \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 10, 10)           \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 100, 10)          \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 100, 100)         \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000, 10)         \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000, 100)        \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000, 1000)       \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000000, 10)      \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000000, 1000)    \
RANDOM_TEST(GROUP_NAME, SORT_FUNCTION, 1000000, 1000000)

RANDOM_TESTS_GROUP(SequentialSortTests, sequential::sort)
RANDOM_TESTS_GROUP(ParallelSortTests, parallel::sort)
