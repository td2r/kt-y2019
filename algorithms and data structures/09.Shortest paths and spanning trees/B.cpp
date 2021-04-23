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

ll const INF = 1'000'000'000'000'000'000;
int const MAXN = 30'000 + 3;

int n, m, s, f;
ll d[MAXN];
set<pair<ll, int>> q;
vector<pair<int, int>> g[MAXN];

int main() {
    // file_raii immolate_improved("data");
    fill(d, d + MAXN, INF);

    scanf("%d %d %d", &n, &s, &f);
    --s, --f;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int w;
            scanf("%d", &w);
            if (i == j) continue;
            if (w != -1) {
                g[i].emplace_back(make_pair(j, w));
            }
        }
    }

    d[s] = 0;
    q.insert(make_pair(0, s));
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());

        if (d[v] == INF) break;

        for (size_t j = 0; j < g[v].size(); ++j) {
            int to = g[v][j].first;
            int w = g[v][j].second;
            if (d[v] + w < d[to]) {
                q.erase(make_pair(d[to], to));
                d[to] = d[v] + w;
                q.insert(make_pair(d[to], to));
            }
        }
    }

    printf("%lld\n", d[f] == INF ? -1 : d[f]);
    return 0;
}
