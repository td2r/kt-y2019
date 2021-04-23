#include <bits/stdc++.h>
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
vector<int> g[MAXN];

void dfs_parents(vector<int> &parent, int v) {
    for (int const& to : g[v]) {
        if (to != parent[v]) {
            parent[to] = v;
            dfs_parents(parent, to);
        }
    }
}

vector<int> to_prufer() {
    vector<int> prufer(n - 2);
    vector<int> parent(n, -1);
    vector<int> deg(n);
    int ptr = -1;
    int leaf = -1;
    dfs_parents(parent, n - 1);
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

int main() {
    // file_raii pomello("data");

    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    vector<int> prufer = to_prufer();

    for (int const& i : prufer) {
        printf("%d ", i + 1);
    } printf("\n");

    return 0;
}
