﻿#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

int const MAXN = 100'000 + 3;

int n;

void dfs_parents(vector<vector<int>> const& g, vector<int> &parent, int v) {
    for (int const& to : g[v]) {
        if (to != parent[v]) {
            parent[to] = v;
            dfs_parents(g, parent, to);
        }
    }
}

vector<int> to_prufer(vector<vector<int>> const& g) {
    vector<int> prufer(n - 2);
    vector<int> parent(n, -1);
    vector<int> deg(n);
    int ptr = -1;
    int leaf = -1;
    dfs_parents(g, parent, n - 1);
    for (int i = 0; i < n; ++i) {
        deg[i] = g[i].size();
        if (deg[i] == 1 && ptr == -1) {
            leaf = ptr = i;
        }
    }
    for (int i = 0; i < n - 2; ++i) {
        prufer[i] = parent[leaf];
        --deg[leaf];
        --deg[parent[leaf]];
        if (deg[parent[leaf]] == 1 && parent[leaf] < ptr) {
            leaf = parent[leaf];
        } else {
            while (ptr < n && deg[ptr] != 1) {
                ++ptr;
            }
            leaf = ptr;
        }
    }
    return prufer;
}

vector<pair<int, int>> from_prufer(vector<int> const& prufer) {
    int ptr = 0;
    int leaf = 0;
    vector<int> deg(n, 1);
    vector<pair<int, int>> result(n - 1);
    for (int const& i : prufer) {
        ++deg[i];
    }
    while (ptr < n && deg[ptr] != 1) {
        ++ptr;
    }
    leaf = ptr;
    for (int i = 0; i < n - 2; ++i) {
        result[i] = make_pair(leaf, prufer[i]);
        --deg[leaf];
        --deg[prufer[i]];
        if (deg[prufer[i]] == 1 && prufer[i] < ptr) {
            leaf = prufer[i];
        } else {
            while (ptr < n && deg[ptr] != 1) {
                ++ptr;
            }
            leaf = ptr;
        }
    }
    result[n - 2] = make_pair(find(deg.begin(), deg.end(), 1) - deg.begin(),
                              n - 1);
    return result;
}

int main() {
    // file_raii pomello("data");

    scanf("%d", &n);
    vector<int> prufer(n - 2);
    for (int i = 0; i < n - 2; ++i) {
        scanf("%d", &prufer[i]);
        --prufer[i];
    }

    auto edges = from_prufer(prufer);

    for (auto const& e : edges) {
        printf("%d %d\n", e.first + 1, e.second + 1);
    }

    return 0;
}
