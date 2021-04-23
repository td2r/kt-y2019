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

int const MAXN = 150 + 3;

int n, m;

void to_complement(vector<set<int>> &g) {
    vector<set<int>> h(g.size());
    for (int i = 0; i < n; ++i) {
        for (int j = n; j < n + m; ++j) {
            if (g[i].find(j) == g[i].end()) {
                assert(g[j].find(i) == g[j].end());
                h[i].insert(j);
                h[j].insert(i);
            }
        }
    }
    swap(g, h);
}

bool kuhn(int v, vector<set<int>> &g, vector<int> &mt, vector<bool> &used) {
    if (used[v]) {
        return false;
    }
    used[v] = true;
    for (int const& to : g[v]) {
        if (mt[to] == -1 || kuhn(mt[to], g, mt, used)) {
            mt[to] = v;
            mt[v] = to;
            return true;
        }
    }
    return false;
}

void orientate(vector<set<int>> &g, vector<int> const& mt) {
    for (int u = n; u < n + m; ++u) {
        g[u].clear();
        if (mt[u] != -1) {
            g[u].insert(mt[u]);
            g[mt[u]].erase(u);
        }
    }
}

void dfs(int v, vector<set<int>> const& g, vector<bool> &used) {
    used[v] = true;
    for (int const& to : g[v]) {
        if (!used[to]) {
            dfs(to, g, used);
        }
    }
}

void solve() {
    scanf("%d %d", &n, &m);
    vector<set<int>> g(n + m);
    for (int i = 0; i < n; ++i) {
        int j;
        while (scanf("%d", &j) != -1 && j != 0) {
            j = j - 1 + n;
            g[i].insert(j);
            g[j].insert(i);
        }
    }
    to_complement(g);
    vector<int> mt(n + m, -1);
    vector<bool> used(n + m, false);
    for (int i = 0; i < n; ++i) {
        used.assign(used.size(), false);
        kuhn(i, g, mt, used);
    }
    orientate(g, mt);
    used.assign(used.size(), false);
    for (int i = 0; i < n; ++i) {
        if (used[i] == false && mt[i] == -1) {
            dfs(i, g, used);
        }
    }
    set<int> males;
    set<int> females;
    for (int i = 0; i < n; ++i) {
        if (used[i]) {
            males.insert(i);
        }
    }
    for (int i = n; i < n + m; ++i) {
        if (!used[i]) {
            females.insert(i);
        }
    }
    printf("%zd\n", males.size() + females.size());
    printf("%zd %zd\n", males.size(), females.size());
    for (int const& i : males) {
        printf("%d ", i + 1);
    } printf("\n");
    for (int const& i : females) {
        printf("%d ", i - n + 1);
    } printf("\n");
}

int main() {
    // file_raii pomello("data");

    int k;
    scanf("%d", &k);
    while (k--) {
        solve();
    }

    return 0;
}
