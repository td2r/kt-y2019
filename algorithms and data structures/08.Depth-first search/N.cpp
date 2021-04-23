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

int n, maxw;
bool used[MAXN];
int w[MAXN][MAXN];

void dfs1(int v) {
    used[v] = true;
    for (int to = 0; to < n; ++to) {
        if (!used[to] && w[v][to] <= maxw) {
            dfs1(to);
        }
    }
}

int dfs2(int v) {
    int ret = 1;
    used[v] = true;
    for (int to = 0; to < n; ++to) {
        if (!used[to] && w[to][v] <= maxw) {
            ret += dfs2(to);
        }
    }
    return ret;
}

bool is_strong_connected() {
    memset(used, 0, sizeof(bool) * MAXN);
    int source = 0;
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            source = i;
            dfs1(i);
        }
    }
    memset(used, 0, sizeof(bool) * MAXN);
    return dfs2(source) == n;
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &w[i][j]);
        }
    }

    int l = -1;
    int r = 1'000'000'000;
    maxw = r / 2;
    while (maxw != r) {
        if (is_strong_connected()) {
            r = maxw;
        } else {
            l = maxw;
        }
        maxw = (l + r + 1) / 2;
    }

    printf("%d\n", maxw);

    return 0;
}
