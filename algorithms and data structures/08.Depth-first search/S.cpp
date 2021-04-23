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

int const MAXN = 300 + 3;

int n, l;
int nextv[MAXN];
int order[2 * MAXN - 1];
vector<int> inst[MAXN];
vector<pair<int, int>> edges;

vector<int> dfs(int v) {
    vector<int> subtree({v});
    for (int const& i : inst[v]) {
        int to = order[i + 1];
        for (int const& u : dfs(to)) {
            subtree.emplace_back(u);
            if (to <= u)
                edges.emplace_back(make_pair(v, u));
        }
    }
    return subtree;
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &l);
    for (int i = 0; i < l; ++i) {
        scanf("%d", &order[i]);
        --order[i];
        inst[order[i]].emplace_back(i);
    }
    for (int v = 0; v < n; ++v) {
        nextv[v] = inst[v].back() + 1;
        inst[v].pop_back();
    }

    for (int i = 0; i < l; i = nextv[order[i]]) {
        dfs(order[i]);
    }

    printf("%zd\n", edges.size());
    for (auto const& edge : edges) {
        printf("%d %d\n", edge.first + 1, edge.second + 1);
    }

    return 0;
}
