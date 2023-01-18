//
// Created by ivanln on 11.11.22.
//

#pragma once

template<size_t N>
size_t coord_to_id(size_t x, size_t y, size_t z) {
    return N * (N * x + y) + z;
}

template<size_t N>
size_t degree_of(size_t v) {
    auto const z = v % N;
    v /= N;
    auto const y = v % N;
    v /= N;
    auto const x = v;
    return (x > 0) +
           (y > 0) +
           (z > 0) +
           (x < N - 1) +
           (y < N - 1) +
           (z < N - 1);
}

template<size_t N>
std::vector<size_t> adj_of(size_t v) {
    auto const z = v % N;
    v /= N;
    auto const y = v % N;
    v /= N;
    auto const x = v;
    std::vector<size_t> adj;
    if (x > 0) adj.push_back(coord_to_id<N>(x - 1, y, z));
    if (y > 0) adj.push_back(coord_to_id<N>(x, y - 1, z));
    if (z > 0) adj.push_back(coord_to_id<N>(x, y, z - 1));
    if (x < N - 1) adj.push_back(coord_to_id<N>(x + 1, y, z));
    if (y < N - 1) adj.push_back(coord_to_id<N>(x, y + 1, z));
    if (z < N - 1) adj.push_back(coord_to_id<N>(x, y, z + 1));
    return std::move(adj);
}
