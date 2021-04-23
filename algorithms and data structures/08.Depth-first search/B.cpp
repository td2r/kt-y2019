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

int const MAXN = 20'000 + 3;

int n, m, k = 0;
int used[MAXN], color[MAXN];
vector<int> topsort;
vector<int> g[MAXN], gt[MAXN];

void dfs1(int v) {
    used[v] = 1;
    for (int to : g[v]) {
        if (used[to] == 0) {
            dfs1(to);
        }
    }
    topsort.emplace_back(v);
}

void dfs2(int v) {
    used[v] = 2;
    color[v] = k;
    for (int to : gt[v]) {
        if (used[to] == 1) {
            dfs2(to);
        }
    }
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u); --v, --u;
        g[v].emplace_back(u);
        gt[u].emplace_back(v);
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            dfs1(i);
        }
    }

    reverse(topsort.begin(), topsort.end());
    for (int const& i : topsort) {
        if (used[i] == 1) {
            ++k;
            dfs2(i);
        }
    }

    printf("%d\n", k);
    for (int i = 0; i < n; ++i) {
        printf("%d ", color[i]);
    } printf("\n");

    return 0;
}
