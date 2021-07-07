#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000 + 3;

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

int n, m;
char banned[MAXN];
vector<int> g[2 * MAXN];
vector<int> gt[2 * MAXN];
bool used[2 * MAXN];
int components = 0;
int component[2 * MAXN];

void add_edge(int u, int v) {
    // printf("added edge %s%d -> %s%d\n", u >= n ? "-" : "", u % n + 1,
    //         v >= n ? "-" : "", v % n + 1);
    g[u].emplace_back(v);
    gt[v].emplace_back(u);
}

void dfs_ts(int v, vector<int>& topsort) {
    used[v] = true;
    for (int const& to : g[v]) {
        if (!used[to]) {
            dfs_ts(to, topsort);
        }
    }
    topsort.emplace_back(v);
}

void dfs_css(int v) {
    component[v] = components;
    for (int const& to : gt[v]) {
        if (component[to] == -1) {
            dfs_css(to);
        }
    }
}

bool solve_2sat() {
    vector<int> topsort;
    for (int i = 0; i < 2 * n; ++i) {
        if (!used[i]) {
            dfs_ts(i, topsort);
        }
    }
    memset(component, 255, sizeof(int) * 2 * n);
    reverse(topsort.begin(), topsort.end());
    for (int const& v : topsort) {
        if (component[v] == -1) {
            dfs_css(v);
            ++components;
        }
    }
    for (int v = 0; v < n; ++v) {
        if (component[v] == component[v + n]) {
            return false;
        }
    }
    return true;
}

int main() {
    scanf("%d %d\n", &n, &m);
    scanf("%s\n", banned);
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d\n", &u, &v);
        --u, --v;
        if (banned[u] == banned[v]) {
            // R G B |  R G B |  R G B
            // # 0 1 |  0 # 1 |  0 1 #
            // # 0 1 |  0 # 1 |  0 1 #
            add_edge(u + n, v);
            add_edge(u, v + n);
            add_edge(v, u + n);
            add_edge(v + n, u);
        } else if ((banned[u] == 'R' && banned[v] == 'G')
                || (banned[u] == 'G' && banned[v] == 'R')) {
            // R G B | R G B
            // # 0 1 | 0 # 1
            // 0 # 1 | # 0 1
            add_edge(u, v + n);
            add_edge(v, u + n);
        } else if (banned[u] == 'R' && banned[v] == 'B') {
            // R G B
            // # 0 1
            // 0 1 #
            add_edge(u + n, v + n);
            add_edge(v, u);
        } else if ((banned[u] == 'G' && banned[v] == 'B')
                || (banned[u] == 'B' && banned[v] == 'G')) {
            // R G B | R G B
            // 0 # 1 | 0 1 #
            // 0 1 # | 0 # 1
            add_edge(u + n, v);
            add_edge(v + n, u);
        } else if (banned[u] == 'B' && banned[v] == 'R') {
            // R G B
            // 0 1 #
            // # 0 1
            add_edge(u, v);
            add_edge(v + n, u + n);
        } else {
            assert(false);
        }
    }

    if (!solve_2sat()) {
        printf("Impossible\n");
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        bool value = (component[i] > component[i + n]);
        switch (banned[i]) {
            case 'R':
                printf("%c", !value ? 'G' : 'B');
                break;
            case 'G':
                printf("%c", !value ? 'R' : 'B');
                break;
            case 'B':
                printf("%c", !value ? 'R' : 'G');
                break;
            default:
                assert(false);
                break;
        }
    } printf("\n");

    return 0;
}
