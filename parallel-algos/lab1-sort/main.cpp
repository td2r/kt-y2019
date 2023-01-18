#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>

#include <cilk/cilkscale.h>

#include "sequential.h"
#include "parallel.h"

size_t const MAX_SIZE = 100000000;
unsigned long const MAX_ELEM = 100000000;
int const REPEATS = 5;

static std::mt19937 rand_gen = std::mt19937(std::random_device()());

int generate_element() {
    return (int) (rand_gen() % MAX_ELEM);
}

void warm_up() {
    std::vector<int> v(MAX_SIZE);
    for (int i = 0; i < 3; ++i) {
        std::generate(v.begin(), v.end(), generate_element);
        sequential::sort(v.begin(), v.end());
    }
    for (int i = 0; i < 3; ++i) {
        std::generate(v.begin(), v.end(), generate_element);
        parallel::sort(v.begin(), v.end());
    }
}

void init_vector_and_copies(std::vector<int>& v, std::vector<std::vector<int>>& vcopy) {
    v = std::vector<int>(MAX_SIZE);
    std::generate(v.begin(), v.end(), generate_element);

    vcopy = std::vector<std::vector<int>>(REPEATS);
    for (int i = 0; i < REPEATS; ++i)
        vcopy[i] = std::vector<int>(v.begin(), v.end());
}

void reinit_copy_by_vector(std::vector<int> const& v, std::vector<std::vector<int>>& vcopy) {
    for (int i = 0; i < REPEATS; ++i)
        std::copy(v.begin(), v.end(), vcopy[i].begin());
}

double calc_speedup() {
    std::vector<int> a(MAX_SIZE);
    std::generate(a.begin(), a.end(), generate_element);
    std::vector<int> b(a.begin(), a.end());

    auto start = std::chrono::high_resolution_clock::now();
    sequential::sort(a.begin(), a.end());
    auto seq_stop = std::chrono::high_resolution_clock::now();

    parallel::sort(b.begin(), b.end());
    auto par_stop = std::chrono::high_resolution_clock::now();


    auto seq_duration = std::chrono::duration_cast<std::chrono::microseconds>(seq_stop - start);
    auto par_duration = std::chrono::duration_cast<std::chrono::microseconds>(par_stop - seq_stop);
    std::cout << "sequential: " << seq_duration.count() << " ms" << std::endl;
    std::cout << "parallel  : " << par_duration.count() << " ms" << std::endl;
    std::cout << "speedup: " << (double) seq_duration.count() / (double) par_duration.count() << std::endl;
    return (double) seq_duration.count() / (double) par_duration.count();
}

int parallel_bench() {
    std::vector<int> v(MAX_SIZE);
    std::generate(v.begin(), v.end(), generate_element);
    wsp_t start = wsp_getworkspan();

    parallel::sort(v.begin(), v.end());

    wsp_t end = wsp_getworkspan();
    wsp_t elapsed = wsp_sub(end, start);
    wsp_dump(elapsed, "parallel_sort");
    return 0;
}

int const RUNS = 5;

int main(int argc, char* argv[]) {
    if (argc == 2 && !std::strcmp(argv[1], "bench")) {
        return parallel_bench();
    } else if (argc == 1) {
        double ssum = 0.0;
        for (int i = 0; i < RUNS; ++i) {
            ssum += calc_speedup();
        }
        std::cout << std::setprecision(10) << ssum / (double) RUNS << std::endl;
    } else {
        throw std::runtime_error("unknown main arguments");
    }
}
