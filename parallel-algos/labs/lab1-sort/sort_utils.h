//
// Created by ivanln on 08.11.22.
//

#pragma once

#include <random>

namespace sort_utils {
    namespace {
        std::mt19937 rand_gen = std::mt19937(std::random_device()());
    }

    template<class RandIt>
    inline RandIt pivot_element(RandIt first, RandIt last) {
        return first + (rand_gen() % (last - first));
    }
}
