//
// Created by ivanln on 07.11.22.
//

#pragma once

#include <random>
#include "sort_utils.h"

namespace sequential {
    namespace {
        template<class RandomIt>
        inline void save_iter_swap(RandomIt it1, RandomIt it2, RandomIt& save) {
            if (it1 == it2)
                return;
            std::iter_swap(it1, it2);
            if (save == it1)
                save = it2;
            else if (save == it2)
                save = it1;
        }
    }

    __attribute__((unused))
    struct {
        template<class RandomIt>
        std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, RandomIt piv) const {
            auto pivot = *piv;
            auto middle1 = std::partition(first, last, [&pivot](auto const& x) {return x < pivot;});
            auto middle2 = std::partition(middle1, last, [&pivot](auto const& x) {return !(pivot < x);});
            return std::make_pair(middle1, middle2);
        }
    } const std_partition;

    __attribute__((unused))
    struct {
        template<class RandomIt>
        std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, RandomIt piv) const {
            --last;
            while (first <= last) {
                while (*first < *piv) {
                    ++first;
                }
                while (*last > *piv) {
                    --last;
                }
                if (first >= last) {
                    break;
                }
                if (first != last) {
                    save_iter_swap(first, last, piv);
                    ++first;
                    --last;
                }
            }
            return std::make_pair(last + 1, last + 1);
        }
    } const hoare_partition;

    struct {
        template<class RandomIt>
        std::pair<RandomIt, RandomIt> operator()(RandomIt first, RandomIt last, RandomIt piv) const {
            RandomIt it = first;
            while (it < last) {
                if (*it < *piv) {
                    save_iter_swap(first, it, piv);
                    ++first;
                    ++it;
                } else if (*it == *piv) {
                    ++it;
                } else {
                    --last;
                    save_iter_swap(it, last, piv);
                }
            }
            return std::make_pair(first, last);
        }
    } const fat_partition;

    /*
     * Returns a pair of iterators (i1, i2) such that
     * elements in [first, i1) are not greater than piv
     * elements in [i1, i2)    are equals to piv
     * elements in [i2, last)  are not less than piv
     */
    auto constexpr partition = fat_partition;

    template<class RandomIt>
    void sort(RandomIt first, RandomIt last) {
        if (last - first < 2)
            return;
        std::pair<RandomIt, RandomIt> prt = sequential::partition(first, last, sort_utils::pivot_element(first, last));
        sequential::sort(first, prt.first);
        sequential::sort(prt.second, last);
    }

    template<class RandomIt>
    void bubble_sort(RandomIt first, RandomIt last) {
        RandomIt i = last;
        RandomIt j;
        while (i > first) {
            for (j = first + 1; j < i; ++j) {
                if (*j < *(j - 1))
                    std::iter_swap(j - 1, j);
            }
            --i;
        }
    }
}
