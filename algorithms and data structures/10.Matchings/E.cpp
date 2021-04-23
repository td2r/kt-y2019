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

int const MAXN = 100 + 3;

int n, m, k;
char board[MAXN][MAXN];

struct action {
    int type;
    int row;
    int col;
    char c;

    action(int type, int row, int col, int code)
    : type(type)
    , row(row)
    , col(col)
    , c(code == 0 ? 'W' : 'B')
    {}

    void print() {
        printf("%d %d %d %c\n", type, row + 1, col + 1, c);
    }
};

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
    for (int u = k; u < 2 * k; ++u) {
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

void solve(char c, vector<action> &ans) {
    int k = n + m - 1;
    vector<set<int>> g(2 * k);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if ((i + j + c) % 2 != board[i][j]) {
                int v = i + j;
                int u = i - j + m - 1 + k;
                g[v].insert(u);
                g[u].insert(v);
            }
        }
    }
    vector<int> mt(2 * k, -1);
    vector<bool> used(2 * k, false);
    for (int i = 0; i < k; ++i) {
        used.assign(used.size(), false);
        kuhn(i, g, mt, used);
    }
    orientate(g, mt);
    used.assign(used.size(), false);
    for (int i = 0; i < k; ++i) {
        if (used[i] == false && mt[i] == -1) {
            dfs(i, g, used);
        }
    }
    for (int i = 0; i < k; ++i) {
        if (!used[i]) {
            if (i > m - 1) {
                ans.emplace_back(action(1, i - m + 1, m - 1, (i + c) % 2));
            } else {
                ans.emplace_back(action(1, 0, i, (i + c) % 2));
            }
        }
    }
    for (int i = k; i < 2 * k; ++i) {
        if (used[i]) {
            int diff = i - k - m + 1;
            if (diff >= 0) {
                ans.emplace_back(action(2, diff, 0, (diff + c) % 2));
            } else {
                ans.emplace_back(action(2, 0, -diff, (c - diff) % 2));
            }
        }
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d\n", &n, &m);
    k = n + m - 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%c", &board[i][j]);
            board[i][j] = (board[i][j] == 'W') ? 0 : 1;
        } scanf("\n");
    }

    vector<action> ans1;
    vector<action> ans2;

    solve(0, ans1);
    solve(1, ans2);

    if (ans1.size() <= ans2.size()) {
        printf("%zd\n", ans1.size());
        for (size_t i = 0; i < ans1.size(); ++i) {
            ans1[i].print();
        }
    } else {
        printf("%zd\n", ans2.size());
        for (size_t i = 0; i < ans2.size(); ++i) {
            ans2[i].print();
        }
    }

    return 0;
}
