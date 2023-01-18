//
// Created by ivanln on 29.11.22.
//

#pragma once

#include <cstdint>
#include <vector>

#include <cilk/cilk.h>

size_t constexpr BLOCK = 32;
size_t constexpr SEQ_CASE_BLOCK = 64;

template<class RandIt>
void sequential_scan_inclusive(RandIt first, RandIt last) {
    for (auto it = first + 1; it < last; ++it) {
        *it += *(it - 1);
    }
}

template<class RandIt1, class RandIt2>
void parallel_scan_inclusive_impl(RandIt1 first, RandIt1 last, RandIt2 free_first, RandIt2 free_last) {
    size_t sz = last - first;
    if (sz <= 2 * BLOCK) {
        sequential_scan_inclusive(first, last);
        return;
    }

    *free_first = 0;
    size_t blocks_n = sz / BLOCK;
    cilk_for (size_t i = 0; i < blocks_n; ++i) {
        sequential_scan_inclusive(first + i * BLOCK, first + (i + 1) * BLOCK);
        *(free_first + i + 1) = *(first + (i + 1) * BLOCK - 1); // sum in i-th block
    }
    if (sz % BLOCK) { // last block
        sequential_scan_inclusive(first + blocks_n * BLOCK, last);
        ++blocks_n;
    }

    parallel_scan_inclusive_impl(free_first, free_first + blocks_n, free_first + blocks_n, free_last);

    cilk_for (size_t i = 1; i < blocks_n; ++i) { // starting from 1, cause 0-th block stays the same
        RandIt1 const end = (i + 1 == blocks_n ? last : first + (i + 1) * BLOCK);
        auto add = *(free_first + i);
        for (RandIt1 it = first + i * BLOCK; it != end; ++it) {
            *it += add;
        }
    }
}

template<class RandIt, typename T = typename std::iterator_traits<RandIt>::value_type>
void parallel_scan_inclusive(RandIt first, RandIt last) {
    size_t sz = last - first;
    if (sz <= SEQ_CASE_BLOCK) {
        sequential_scan_inclusive(first, last);
        return;
    }
    // calculating of addition memory size for scan
    size_t free_sz = 1;
    while (sz > 1) {
        sz = (sz + BLOCK - 1) / BLOCK;
        free_sz += sz;
    }
    std::vector<T> free_mem(free_sz);
    parallel_scan_inclusive_impl(first, last, free_mem.begin(), free_mem.end());
}
