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

int timer = 0;
int n, m, k = 0;
int tin[MAXN];
int tup[MAXN];
int used[MAXN];  //  0 - white, 1 -grey, 2 - black

stack<int> st;

vector<int> g[MAXN];
pair<int, int> edges[MAXM];
int color[MAXM];

void clear_stack(int stop_edge = -1) {
    if (st.empty()) return;
    while (!st.empty()) {
        int e = st.top();
        color[e] = k;
        st.pop();
        if (e == stop_edge) break;
    }
    ++k;
}

void dfs(int, int);

void dfs(int v) {
    int visited = 0;
    used[v] = 1;
    tup[v] = tin[v] = timer++;
    for (int const& e : g[v]) {
        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (used[to] == 0) {
            ++visited;
            if (visited > 1) {
                clear_stack();
            }
            st.push(e);
            dfs(to, v);
        } else if (used[to] == 1) {
            st.push(e);
        }
    }
    used[v] = 2;
}

void dfs(int v, int be) {
    used[v] = 1;
    tup[v] = tin[v] = timer++;
    if (DEBUG) {
        printf("tup[%d] = %d\n", v + 1, tup[v]);
    }
    for (int const& e : g[v]) {
        if (e == be) continue;

        int to = edges[e].first;
        if (to == v) to = edges[e].second;

        if (used[to] == 0) {
            st.push(e);
            dfs(to, e);
            if (tin[v] <= tup[to]) {
                clear_stack(e);
            }
            tup[v] = min(tup[v], tup[to]);
        } else {
            tup[v] = min(tup[v], tin[to]);
            if (used[to] == 1) {
                st.push(e);
            }
        }
    }
    used[v] = 2;
}

int main() {
    // file_raii immolate_improved("data");

    memset(color, 255, sizeof(int) * MAXM);

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
        if (used[i] == 0) {
            dfs(i);
            clear_stack();
        }
    }

    printf("%d\n", k);
    for (int i = 0; i < m; ++i) {
        if (color[i] == -1) {
            throw "ball";
        }
        printf("%d ", color[i] + 1);
    } printf("\n");

    return 0;
}
