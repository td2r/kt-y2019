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
set<int> ap;
vector<int> g[MAXN];
pair<int, int> edges[MAXM];

void dfs(int, int);

void dfs(int v) {
    int visited = 0;
    used[v] = true;
    tup[v] = tin[v] = timer++;
    for (int const& e : g[v]) {
        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (!used[to]) {
            ++visited;
            dfs(to, v);
        }
    }
    if (visited > 1) {
        ap.insert(v);
    }
}

void dfs(int v, int be) {
    used[v] = true;
    tup[v] = tin[v] = timer++;
    if (DEBUG) {
        printf("tup[%d] = %d\n", v + 1, tup[v]);
    }
    for (int const& e : g[v]) {
        if (e == be) continue;

        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (!used[to]) {
            dfs(to, e);
            if (tin[v] <= tup[to]) {
                ap.insert(v);
            }
            tup[v] = min(tup[v], tup[to]);
        } else {
            tup[v] = min(tup[v], tin[to]);
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
        edges[i] = make_pair(v, u);
        g[v].emplace_back(i);
        g[u].emplace_back(i);
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }

    printf("%zd\n", ap.size());
    for (int const& it : ap) {
        printf("%d ", it + 1);
    } printf("\n");

    if (DEBUG) {
        printf("tin: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", tin[i]);
        } printf("\n");

        printf("tup: ");
        for (int i = 0; i < n; ++i) {
            printf("%d ", tup[i]);
        } printf("\n");
    }

    return 0;
}
