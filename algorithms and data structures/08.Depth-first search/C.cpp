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

int n, m, s, t;
int used[MAXN], d[MAXN];
vector<int> topsort;
vector<pair<int, int>> g[MAXN], gt[MAXN];

void dfs(int v) {
    used[v] = 1;
    for (auto const& to : g[v]) {
        if (used[to.first] == 0) {
            dfs(to.first);
        }
    }
    topsort.emplace_back(v);
}

int main() {
    // file_raii immolate_improved("data");

    fill(d, d + MAXN, INF);
    scanf("%d %d %d %d", &n, &m, &s, &t);
    --s, --t;
    for (int i = 0; i < m; ++i) {
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        --v, --u;
        g[v].emplace_back(make_pair(u, w));
        gt[u].emplace_back(make_pair(v, w));
    }

    d[s] = 0;
    dfs(s);
    if (used[t] == 0) {
        printf("Unreachable\n");
        return 0;
    }

    reverse(topsort.begin(), topsort.end());

    for (int const& v : topsort) {
        for (auto const& e : gt[v]) {
            if (d[e.first] + e.second < d[v]) {
                d[v] = d[e.first] + e.second;
            }
        }
    }

    printf("%d\n", d[t]);

    return 0;
}
