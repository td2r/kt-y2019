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

template <class forwardIterator>
void print_array(string const& name, forwardIterator first, forwardIterator last) {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

int const MAXN = 2'000 + 3;
int const MAXM = 5'000 + 3;
ll const INF = 8'000'000'000'000'000'001;

int n, m, s;
ll d[MAXN];
bool changed[MAXN];
bool used[MAXN];
struct edge { int v; int u; ll w; } e[MAXM];
ll g[MAXN][MAXN];

void dfs(int v) {
    used[v] = true;
    changed[v] = true;
    for (int to = 0; to < n; ++to) {
        if (g[v][to] < INF && !used[to]) {
            dfs(to);
        }
    }
}

int main() {
    // file_raii pomello("data");

    fill(d, d + MAXN, INF);
    for (int i = 0; i < MAXN; ++i) {
        fill(g[i], g[i] + MAXN, INF);
    }

    scanf("%d %d %d", &n, &m, &s);
    d[--s] = 0;
    while (m--) {
        int v, u;
        ll w;
        scanf("%d %d %lld", &v, &u, &w);
        --v, --u;
        g[v][u] = min(g[v][u], w);
    }

    m = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g[i][j] < INF) {
                e[m].v = i;
                e[m].u = j;
                e[m].w = g[i][j];
                ++m;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (d[e[j].v] < INF && d[e[j].v] + e[j].w < d[e[j].u]) {
                d[e[j].u] = d[e[j].v] + e[j].w;
                if (i == n - 1)
                    changed[e[j].u] = true;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (changed[i] && !used[i]) {
            dfs(i);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (changed[i]) {
            printf("-\n");
        } else if (d[i] == INF) {
            printf("*\n");
        } else {
            printf("%lld\n", d[i]);
        }
    }

    return 0;
}
