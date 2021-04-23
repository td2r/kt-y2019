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

int const MAXN =  20'000 + 3;
int const MAXM = 200'000 + 3;

int n, m;
int timer = 0;
int tin[MAXN];
int tup[MAXN];
bool used[MAXN];
vector<int> g[MAXN];
vector<int> bridges;
pair<int, int> edges[MAXM];

void dfs(int v, int be = -1) {
    tup[v] = tin[v] = timer++;
    used[v] = true;
    for (int const& e : g[v]) {
        if (e == be) continue;

        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (!used[to]) {
            dfs(to, e);
            if (tin[v] < tup[to]) {
                bridges.emplace_back(e);
            }
        }

        tup[v] = min(tup[v], tup[to]);
    }
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        edges[i] = make_pair(v, u);
        g[v].emplace_back(i);
        g[u].emplace_back(i);
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }

    sort(bridges.begin(), bridges.end());
    printf("%zd\n", bridges.size());
    for (int const& it : bridges) {
        printf("%d ", it + 1);
    } printf("\n");

    return 0;
}
