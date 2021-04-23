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
int used[MAXN], d[MAXN];
vector<int> g[MAXN];
stack<int> cycle;

int dfs(int v) {
    used[v] = 1;
    for (auto const& to : g[v]) {
        if (used[to] == 1) {
            cycle.push(v);
            return to;
        } else if (used[to] == 0) {
            int r = dfs(to);
            if (r == -1) {
                return -1;
            } else if (r >= 0) {
                cycle.push(v);
                return r == v ? -1 : r;
            }
        }
    }
    used[v] = 2;
    return -2;
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            if (dfs(i) == -1) {
                printf("YES\n");
                while (!cycle.empty()) {
                    printf("%d ", cycle.top() + 1);
                    cycle.pop();
                } printf("\n");
                return 0;
            }
        }
    }

    printf("NO\n");

    return 0;
}
