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

int const MAXN = 30'000 + 3;

int n, m;
int d[MAXN];
vector<int> g[MAXN];

int main() {
    // file_raii immolate_improved("data");
    memset(d, 255, sizeof(int) * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    queue<int> q;
    q.push(0);
    d[0] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int const& to : g[v]) {
            if (d[to] == -1) {
                d[to] = d[v] + 1;
                q.push(to);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", d[i]);
    } printf("\n");

    return 0;
}
