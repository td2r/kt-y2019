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

int n, m, s;
int used[MAXN], d[MAXN];
vector<int> topsort;
vector<int> g[MAXN];

void dfs(int v) {
    used[v] = 1;
    for (auto const& to : g[v]) {
        if (used[to] == 0) {
            dfs(to);
        }
    }
    topsort.emplace_back(v);
}

int main() {
    // file_raii immolate_improved("data");
    memset(d, 255, sizeof(int) * MAXN);

    scanf("%d %d %d", &n, &m, &s);
    --s;
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
    }

    dfs(s);

    for (int const& v : topsort) {
        assert(d[v] == -1);
        d[v] = 1;
        for (auto const& to : g[v]) {
            assert(d[to] != -1);
            if (d[to] == 1) {
                d[v] = 0;
            }
        }
    }

    printf("%s player wins\n", d[s] == 0 ? "First" : "Second");

    return 0;
}
