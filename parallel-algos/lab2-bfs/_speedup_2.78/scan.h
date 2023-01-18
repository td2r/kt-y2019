//
// Created by ivanln on 29.11.22.
//

#pragma once

#include <cstdint>
#include <vector>

#include <cilk/cilk.h>

size_t constexpr BLOCK = 32;
size_t constexpr SEQ_CASE_BLOCK = 64;

//static inline uint32_t uint32_log2(uint32_t const x) {
//    uint32_t y;
//    asm ( "\tbsr %1, %0\n"
//            : "=r"(y)
//            : "r" (x)
//            );
//    return y;
//}
//template<class RandIt, typename T>
//static T scan_exclusive_inplace(RandIt first, RandIt last) {
//    uint32_t const k = uint32_log2(last - first);
//    uint32_t const n = (1 << k);
//    T last_elem = *(last - 1);
//    for (uint32_t i = 1; i <= k; ++i) {
//        uint32_t const d = (1 << (i - 1));
//        uint32_t beb = (n >> i);
//        // using uint32_t causes compilation error for some reason
//        cilk_for (int32_t j = 1; j <= beb; ++j) {
//            *(first + d * 2 * j - 1) += *(first + d * (2 * j - 1) - 1);
//        }
//    }
//    *(last - 1) = 0;
//    for (uint32_t i = k; i > 0; --i) {
//        uint32_t const d = (1 << (i - 1));
//        // using uint32_t causes compilation error for some reason
//        cilk_for (int32_t j = 1; j <= (n >> i); ++j) {
//            RandIt it1 = first + d * (2 * j - 1) - 1;
//            RandIt it2 = first + d * 2 * j - 1;
//            auto const tmp = *it1;
//            *it1 = *it2;
//            *it2 += tmp;
//        }
//        std::cout << "iteration " << (k + 1 - i) << std::endl;
//        for (auto it = first; it != last; ++it) {
//            std::cout << *it << ' ';
//        }
//        std::cout << std::endl;
//    }
//    return *(last - 1) + last_elem;
//}

template<class RandIt, typename T = typename std::iterator_traits<RandIt>::value_type>
[[maybe_unused]]
T sequential_scan_exclusive(RandIt first, RandIt last) {
    T s = 0;
    for (auto it = first; it != last; ++it) {
        s += *it;
    }
    T sum = s;
    for (auto it = last; it != first; --it) {
        *(it - 1) = s - *(it - 1);
        s = *(it - 1);
    }
    return sum;
}

template<class RandIt>
void sequential_scan_inclusive(RandIt first, RandIt last) {
    for (auto it = first + 1; it != last; ++it) {
        *it += *(it - 1);
    }
}

template<class RandIt1, class RandIt2>
void parallel_scan_inclusive_impl(RandIt1 first, RandIt1 last, RandIt2 free_first, RandIt2 free_last) {
    size_t sz = last - first;
    if (sz <= BLOCK) {
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

    cilk_for (size_t i = 1; i < blocks_n; ++i) { // starting from BLOCK, cause 0-th block stays the same
        RandIt1 const end = (i + 1 == blocks_n ? last : first + (i + 1) * BLOCK);
        auto add = *(free_first + i);
        for (RandIt1 it = first + i * BLOCK; it != end; ++it) {
            *it += add;
        }
    }
}

template<class RandIt, typename T = typename std::iterator_traits<RandIt>::value_type>
[[maybe_unused]]
void parallel_scan_inclusive(RandIt first, RandIt last) {
    size_t sz = last - first;
    if (sz <= SEQ_CASE_BLOCK) {
        sequential_scan_inclusive(first, last);
        return;
    }
    // calculating of addition memory size for scan
    size_t free_sz = 1; // need
    while (sz > 1) {
        sz = (sz + BLOCK - 1) / BLOCK;
        free_sz += sz;
    }
    std::vector<T> free_mem(free_sz);
    parallel_scan_inclusive_impl(first, last, free_mem.begin(), free_mem.end());
}
