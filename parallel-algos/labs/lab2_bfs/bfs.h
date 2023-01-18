//
// Created by ivanln on 11.11.22.
//

#pragma once

#include "scan.h"

#include <type_traits>
#include <vector>
#include <functional>
#include <queue>
#include <atomic>
#include <limits>

#include <cilk/cilk.h>

template<typename Vertex_, typename Adjacent_>
std::enable_if_t<std::is_integral_v<Vertex_>, std::vector<size_t>>
sequential_bfs(size_t n,
               std::vector<Vertex_> const& start,
               std::function<size_t(Vertex_)> degree_of,
               std::function<Adjacent_(Vertex_)> adj_of)
{
    std::vector<size_t> dist(n, SIZE_MAX);
    for (Vertex_ const& v : start)
        dist[v] = 0;
    std::queue<Vertex_> q;
    for (Vertex_ const& v : start) {
        q.push(v);
    }
    while (!q.empty()) {
        Vertex_ v = q.front();
        q.pop();
        auto dv = dist[v];
        Adjacent_ adj = adj_of(v);
        for (Vertex_ const& to : adj) {
            if (dist[to] == SIZE_MAX) {
                dist[to] = dv + 1;
                q.push(to);
            }
        }
    }
    return dist;
}

size_t constexpr SEQ_CASE_FILTER = 256;

template<typename Vertex_, typename Adjacent_>
std::enable_if_t<std::is_integral_v<Vertex_>, std::vector<size_t>>
parallel_bfs(size_t const n,
               std::vector<Vertex_> const& start,
               std::function<size_t(Vertex_)> degree_of,
               std::function<Adjacent_(Vertex_)> adj_of)
{
    Vertex_ constexpr INF_VERT = std::numeric_limits<Vertex_>::max();

    std::vector<Vertex_> frontier(start.begin(), start.end());
    std::vector<std::atomic<size_t>> dist(n);
    std::vector<size_t> dg(frontier.size());
    std::vector<Vertex_> reserve;
    std::vector<int> reserve_ind;

    cilk_for (size_t i = 0; i < n; ++i) {
        dist[i].store(SIZE_MAX);
    }
    cilk_for (size_t i = 0; i < frontier.size(); ++i) {
        dist[frontier[i]].store(0);
        dg[i] = degree_of(frontier[i]);
    }

    for (size_t d = 1; !frontier.empty(); ++d) {
        parallel_scan_inclusive(dg.begin(), dg.end());
        reserve.resize(dg.back(), INF_VERT);
        cilk_for (size_t i = 0; i < frontier.size(); ++i) {
            auto adj = adj_of(frontier[i]);
            for (size_t j = 0; j < adj.size(); ++j) {
                size_t expected = SIZE_MAX;
                if (dist[adj[j]].compare_exchange_strong(expected, d)) {
                    reserve[dg[i] - j - 1] = adj[j];
//                    reserve_ind[dg[i] - j - 1] = 1;
                }
            }
        }
        if (reserve.size() < SEQ_CASE_FILTER) { // sequential filter
            frontier.clear();
            dg.clear();
            for (auto& it : reserve) {
                if (it != INF_VERT) {
                    frontier.push_back(it);
                    dg.push_back(degree_of(it));
                    it = INF_VERT;
                }
            }
        } else { // parallel filter
            reserve_ind.resize(reserve.size());
            // parallel map
            cilk_for(size_t i = 0; i < reserve_ind.size(); ++i) {
                reserve_ind[i] = (reserve[i] != INF_VERT);
            }
            parallel_scan_inclusive(reserve_ind.begin(), reserve_ind.end());
            frontier.resize(reserve_ind.back());
            dg.resize(frontier.size());
            cilk_for(size_t i = 0; i < reserve_ind.size(); ++i) {
                if (reserve[i] != INF_VERT) {
                    frontier[reserve_ind[i] - 1] = reserve[i];
                    dg[reserve_ind[i] - 1] = degree_of(reserve[i]);
                    reserve[i] = INF_VERT;
                }
            }
        }
    }
    std::vector<size_t> result(n);
    cilk_for (size_t i = 0; i < n; ++i) {
        result[i] = dist[i].load();
    }
    return result;
}
