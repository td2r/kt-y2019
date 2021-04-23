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

int const MAXN = 100'000 + 3;
int const INF = 1'000'000'001;

int n, m;
int used[MAXN];
set<int> g[MAXN];
vector<int> topsort;

void dfs(int v) {
    used[v] = 1;
    for (int const& to : g[v]) {
        if (used[to] == 0) {
            dfs(to);
        }
    }
    topsort.emplace_back(v);
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].insert(u);
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            dfs(i);
        }
    }

    for (int i = 1; i < n; ++i) {
        int u = topsort[i - 1];
        int v = topsort[i];
        if (g[v].find(u) == g[v].end()) {
            printf("NO\n");
            return 0;
        }
    }

    printf("YES\n");

    return 0;
}
