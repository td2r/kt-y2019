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

int n, m, k = 0;
int timer = 0;
int tin[MAXN];
int tup[MAXN];
bool used[MAXN];
stack<int> st;
vector<int> g[MAXN];
pair<int, int> edges[MAXM];
int color[MAXN];

void clear_stack(int stop_vert = -1) {
    if (st.empty()) return;
    while (!st.empty()) {
        int v = st.top();
        color[v] = k;
        st.pop();
        if (v == stop_vert) break;
    }
    ++k;
}

void dfs(int v, int be = -1) {
    st.push(v);
    used[v] = true;
    tup[v] = tin[v] = timer++;
    for (int const& e : g[v]) {
        if (e == be) continue;

        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (!used[to]) {
            dfs(to, e);
            if (tin[v] < tup[to]) {
                clear_stack(to);
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
            clear_stack();
        }
    }

    printf("%d\n", k);
    for (int i = 0; i < n; ++i) {
        printf("%d ", color[i] + 1);
    } printf("\n");

    return 0;
}
