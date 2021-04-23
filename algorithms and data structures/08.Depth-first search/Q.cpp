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

int const MAXN =  1'000 + 3;
int const MAXM = 10'000 + 3;

int a;
int n, m;
pair<int, int> edge[2 * MAXM];
vector<int> g[2 * MAXN];
int color[2 * MAXN];

void dfs(int v, int k) {
    color[v] = k;
    for (int const& e : g[v]) {
        if (e / 2 > a) continue;

        int to = edge[e].first;
        if (to == v) to = edge[e].second;

        if (color[to] == -1) {
            dfs(to, k);
        }
    }
}

bool correct() {
    memset(color, 255, sizeof(int) * 2 * MAXN);
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (color[i] == -1) {
            dfs(i, k);
            ++k;
        }
        if (color[i + n] == -1) {
            dfs(i + n, k);
            ++k;
        }
        if (color[i] == color[i + n]) {
            return false;
        }
    }
    return true;
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        char t;
        scanf("%d %d %c", &v, &u, &t);
        --v, --u;
        if (t == 'T') {
            edge[2 * i] = make_pair(v, u);
            edge[2 * i + 1] = make_pair(v + n, u + n);
            g[v].emplace_back(2 * i);
            g[u].emplace_back(2 * i);
            g[v + n].emplace_back(2 * i + 1);
            g[u + n].emplace_back(2 * i + 1);
        } else {
            edge[2 * i] = make_pair(v, u + n);
            edge[2 * i + 1] = make_pair(v + n, u);
            g[v].emplace_back(2 * i);
            g[u + n].emplace_back(2 * i);
            g[v + n].emplace_back(2 * i + 1);
            g[u].emplace_back(2 * i + 1);
        }
    }

    int l = 0;
    int r = m;
    a = r / 2;
    while (a != l) {
        if (correct()) {
            l = a;
        } else {
            r = a;
        }
        a = (l + r) / 2;
    }

    printf("%d\n", a + 1);

    return 0;
}
