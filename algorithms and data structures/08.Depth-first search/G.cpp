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

int n, m, k = 0;
int used[MAXN], component[MAXN];
vector<int> g[MAXN];

void dfs(int v) {
    used[v] = 1;
    component[v] = k;
    for (int const& to : g[v]) {
        if (used[to] == 0) {
            dfs(to);
        }
    }
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            ++k;
            dfs(i);
        }
    }

    printf("%d\n", k);
    for (int i = 0; i < n; ++i) {
        printf("%d ", component[i]);
    } printf("\n");

    return 0;
}
