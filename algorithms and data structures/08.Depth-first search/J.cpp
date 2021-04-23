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
int color[MAXN];
vector<int> g[MAXN];

bool dfs(int v, int c) {
    color[v] = c;
    for (int const& to : g[v]) {
        if (color[to] == color[v] || (color[to] == -1 && !dfs(to, c ^ 1))) {
            return false;
        }
    }
    return true;
}

int main() {
    // file_raii immolate_improved("data");
    memset(color, 255, sizeof(int) * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {
            if (!dfs(i, 0)) {
                printf("NO\n");
                return 0;
            }
        }
    }

    printf("YES\n");

    return 0;
}
