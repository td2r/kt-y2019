//
// Created by ivanln on 11.11.22.
//

#pragma once

#include "llvm/ADT/SmallVector.h"

typedef uint32_t CubeVertex_;
typedef llvm::SmallVector<CubeVertex_, 3> CubeAdjacent_;

template<CubeVertex_ N>
CubeVertex_ coord_to_id(CubeVertex_ x, CubeVertex_ y, CubeVertex_ z) {
    return N * (N * x + y) + z;
}

template<CubeVertex_ N>
size_t degree_of(CubeVertex_ v) {
    auto const z = v % N;
    v /= N;
    auto const y = v % N;
    v /= N;
    auto const x = v;
    return
           (x < N - 1) +
           (y < N - 1) +
           (z < N - 1);
}

template<CubeVertex_ N>
CubeAdjacent_ adj_of(CubeVertex_ v) {
    auto const z = v % N;
    v /= N;
    auto const y = v % N;
    v /= N;
    auto const x = v;
    CubeAdjacent_ adj;
    if (x < N - 1) adj.push_back(coord_to_id<N>(x + 1, y, z));
    if (y < N - 1) adj.push_back(coord_to_id<N>(x, y + 1, z));
    if (z < N - 1) adj.push_back(coord_to_id<N>(x, y, z + 1));
    return std::move(adj);
}
