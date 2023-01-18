#include <iostream>
#include <vector>
#include <functional>
#include <chrono>

#include "bfs.h"
#include "test_utils.h"

size_t constexpr N = 500;

int const RUNS = 5;

std::function<size_t(size_t)> test_degree_f = degree_of<N>;
std::function<std::vector<size_t>(size_t)> test_adj_f = adj_of<N>;

void do_sequential_bfs() {
    auto d = sequential_bfs(N * N * N, {0}, test_degree_f, test_adj_f);
}

void do_parallel_bfs() {
    auto d = parallel_bfs(N * N * N, {0}, test_degree_f, test_adj_f);
}

void warm_up() {
    do_sequential_bfs();
    do_parallel_bfs();
}

double get_speedup() {
    auto t1 = std::chrono::high_resolution_clock::now();
    do_sequential_bfs();
    auto t2 = std::chrono::high_resolution_clock::now();
    do_parallel_bfs();
    auto t3 = std::chrono::high_resolution_clock::now();
    auto seq_duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
    auto par_duration = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
    std::cout << "seq: " << seq_duration.count() << std::endl;
    std::cout << "par: " << par_duration.count() << std::endl;
    double speedup = (double) seq_duration.count() / (double) par_duration.count();
    std::cout << "speedup: " << speedup << std::endl;
    return speedup;
}

int main() {
    std::cout << "Warming up..." << std::endl;
    warm_up();
    std::cout << "Done" << std::endl << std::endl;

    double ssum = 0.0;
    for (int i = 0; i < RUNS; ++i) {
        ssum += get_speedup();
    }

    std::cout << "average speedup: " << ssum / RUNS << std::endl;

    return 0;
}
