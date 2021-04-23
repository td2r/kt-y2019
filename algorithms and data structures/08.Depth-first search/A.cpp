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

int n, m;
vector<int> g[MAXN];
int color[MAXN];
vector<int> topsort;

bool dfs(int v) {
    color[v] = 1;
    for (int to : g[v]) {
        if (color[to] == 1 || (color[to] == 0 && !dfs(to))) {
            return false;
        }
    }
    color[v] = 2;
    topsort.emplace_back(v + 1);
    return true;
}

int main() {
    // file_raii immolate_improved("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u); --v, --u;
        g[v].emplace_back(u);
    }

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            if (!dfs(i)) {
                printf("-1\n");
                return 0;
            }
        }
    }

    reverse(topsort.begin(), topsort.end());
    for (auto const& i : topsort) {
        printf("%d ", i);
    } printf("\n");

    return 0;
}
