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

int read_uint() {
    char c;
    int res = 0;
    while ((c = (char) getchar()) != -1) {
        if (c == ' ' || c == '\n') {
            return res;
        }
        res = 10 * res + c - '0';
    }
    return res;
}

int const MAXN = 2'000 + 3;
int const MAXM = 200'000 + 3;

int n, m = 0;
int timer = 1;
pair<int, int> edge[MAXM];
int comp[2 * MAXN];
int sz[MAXN];
int groom[MAXN][MAXN];
bool used[2 * MAXN];
vector<int> g[2 * MAXN];
vector<int> gt[2 * MAXN];
vector<int> topsort;

void dfs(int v) {
    used[v] = true;
    for (int const& to : g[v]) {
        if (!used[to]) {
            dfs(to);
        }
    }
    topsort.emplace_back(v);
}

void color_comp(int v) {
    comp[v] = timer;
    for (int const& to : gt[v]) {
        if (comp[to] == -1) {
            color_comp(to);
        }
    }
}

int main() {
    // file_raii pomello("data");

    memset(comp, 255, sizeof(int) * 2 * MAXN);

    n = read_uint();
    for (int i = 0; i < n; ++i) {
        int ki = read_uint();
        while (ki--) {
            int j = read_uint() - 1 + n;
            edge[m++] = make_pair(i, j);
        }
    }
    for (int i = 0; i < n; ++i) {
        int j = read_uint() - 1 + n;
        g[j].emplace_back(i);
        gt[i].emplace_back(j);
    }
    for (int i = 0; i < m; ++i) {
        int v = edge[i].first;
        int u = edge[i].second;
        if (g[u][0] != v) {
            g[v].emplace_back(u);
            gt[u].emplace_back(v);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }
    reverse(topsort.begin(), topsort.end());
    for (int const& i : topsort) {
        if (comp[i] == -1) {
            color_comp(i);
            timer++;
        }
    }

    for (int i = 0; i < m; ++i) {
        int v = edge[i].first;
        int u = edge[i].second;
        if (comp[v] == comp[u] || g[u][0] == v) {
            groom[v][sz[v]++] = u;
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d", sz[i]);
        for (int j = 0; j < sz[i]; ++j) {
            printf(" %d", groom[i][j] - n + 1);
        } printf("\n");
    }

    return 0;
}
