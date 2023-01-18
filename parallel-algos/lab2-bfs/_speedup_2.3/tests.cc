#include <gtest/gtest.h>
#include <random>
#include "bfs.h"
#include "test_utils.h"

static std::mt19937 rand_gen = std::mt19937(std::random_device()());

template<size_t N>
void scan_test() {
    std::vector<int> a(N);
    for (size_t i = 0; i < N; ++i) {
        a[i] = (int) rand_gen() % 100;
    }
    std::vector<int> b(a.begin(), a.end());
    parallel_scan_inclusive(a.begin(), a.end());
    sequential_scan_inclusive(b.begin(), b.end());
    for (size_t i = 0; i < N; ++i) {
        if (a[i] != b[i]) {
            std::cout << "test " << N << ", pos = " << i + 1 << std::endl;
            break;
        }
    }
    EXPECT_EQ(a, b);
}

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)

#define SCAN_TEST(n) TEST(ScanTests, MACRO_CONCAT(Scan, n)) { \
    scan_test<n>();                                           \
}

#define CUBE_TEST(TEST_GROUP, BFS_FUN, N)                                            \
TEST(TEST_GROUP, MACRO_CONCAT(Cube, N)) {                                            \
    auto d = BFS_FUN(N * N * N,                                                      \
                     {0},                                                            \
                     (std::function<size_t(size_t)>) degree_of<N>,                   \
                     (std::function<std::vector<size_t>(size_t)>) adj_of<N>);        \
    for (size_t i = 0; i < d.size(); ++i) {                                          \
        size_t z = i % N;                                                            \
        size_t y = (i / N) % N;                                                      \
        size_t x = (i / N) / N;                                                      \
        EXPECT_EQ(d[i], x + y + z);                                                  \
    }                                                                                \
}

std::vector<std::vector<size_t>> adj_list({
      {1, 2},
      {0, 2},
      {0, 1, 3, 4},
      {2, 5},
      {2, 5},
      {3, 4}
});

#define ADJ_LIST_GRAPH_TEST(TEST_GROUP, BFS_FUN)                                                               \
TEST(TEST_GROUP, AdjacentListGraph) {                                                                          \
    auto d = BFS_FUN(adj_list.size(),                                                                          \
                     {2},                                                                                      \
                     std::function<size_t(size_t)>([&](size_t v) { return adj_list[v].size(); }),              \
                     std::function<std::vector<size_t> const&(size_t)>(                                        \
                         [&](size_t v) -> std::vector<size_t> const& {                                         \
                             return adj_list[v];                                                               \
                         }                                                                                     \
                     ));                                                                                       \
    EXPECT_EQ(d[0], 1);                                                                                        \
    EXPECT_EQ(d[1], 1);                                                                                        \
    EXPECT_EQ(d[2], 0);                                                                                        \
    EXPECT_EQ(d[3], 1);                                                                                        \
    EXPECT_EQ(d[4], 1);                                                                                        \
    EXPECT_EQ(d[5], 2);                                                                                        \
}

SCAN_TEST(1)
SCAN_TEST(2)
SCAN_TEST(3)
SCAN_TEST(4)
SCAN_TEST(5)
SCAN_TEST(6)
SCAN_TEST(7)
SCAN_TEST(8)
SCAN_TEST(512)
SCAN_TEST(513)
SCAN_TEST(514)
SCAN_TEST(515)
SCAN_TEST(516)
SCAN_TEST(517)
SCAN_TEST(518)
SCAN_TEST(519)
SCAN_TEST(520)
SCAN_TEST(1024)
SCAN_TEST(1025)
SCAN_TEST(1026)
SCAN_TEST(1094)
SCAN_TEST(2048)
SCAN_TEST(4096)
SCAN_TEST(4097)
SCAN_TEST(4098)
SCAN_TEST(4099)
SCAN_TEST(4100)
SCAN_TEST(6183)
SCAN_TEST(1000000)

CUBE_TEST(SequentialBfsTests, sequential_bfs, 25)
CUBE_TEST(SequentialBfsTests, sequential_bfs, 50)
CUBE_TEST(SequentialBfsTests, sequential_bfs, 100)
CUBE_TEST(SequentialBfsTests, sequential_bfs, 300)
ADJ_LIST_GRAPH_TEST(SequentialBfsTests, sequential_bfs)

CUBE_TEST(ParallelBfsTests, parallel_bfs, 25)
CUBE_TEST(ParallelBfsTests, parallel_bfs, 50)
CUBE_TEST(ParallelBfsTests, parallel_bfs, 100)
CUBE_TEST(ParallelBfsTests, parallel_bfs, 300)
ADJ_LIST_GRAPH_TEST(ParallelBfsTests, parallel_bfs)
