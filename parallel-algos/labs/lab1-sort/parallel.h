//
// Created by ivanln on 08.11.22.
//

#pragma once

#include "sequential.h"
#include <cilk/cilk.h>

namespace parallel {
    namespace {
        size_t const BLOCK = 32;
    }

    template<class RandomIt>
    void sort(RandomIt first, RandomIt last) {
        if (last - first <= BLOCK) {
            sequential::sort(first, last);
            return;
        }
        auto prt = sequential::partition(first, last, sort_utils::pivot_element(first, last));

        cilk_scope {
            cilk_spawn parallel::sort(first, prt.first);
            parallel::sort(prt.second, last);
        }
    }
}
