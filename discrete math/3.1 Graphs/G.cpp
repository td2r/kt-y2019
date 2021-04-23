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
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
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

int const MAXN = 9999 + 3;

int n, m, k;
int color[MAXN];
vector<int> g[MAXN];
bool used[MAXN];
int timer = 1;
int color_used[MAXN];

void dfs(int v) {
    used[v] = true;
    for (int const& to : g[v]) {
        if (!used[to]) {
            dfs(to);
        }
    }
    for (int const& to : g[v]) {
        if (color[to] != -1) {
            color_used[color[to]] = timer;
        }
    }
    color[v] = 0;
    while (color_used[color[v]] == timer) {
        ++color[v];
    }
    ++timer;
}

int main() {
    // file_raii pomello("data");

    memset(color, 255, sizeof(int) * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    k = 0;
    for (int i = 0; i < n; ++i) {
        k = max(k, (int) g[i].size());
    }
    k |= 1;

    int root = 0;
    while (g[root].size() == k) {
        ++root;
    }
    dfs(root);

    printf("%d\n", k);
    for (int i = 0; i < n; ++i) {
        printf("%d\n", color[i] + 1);
    }

    return 0;
}
