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

int const MAXN = 1'000 + 3;

int n, m;
vector<int> g[2 * MAXN];
int mt[2 * MAXN];
bool used[2 * MAXN];

bool dfs(int v) {
    if (used[v]) {
        return false;
    }
    used[v] = true;
    for (int to : g[v]) {
        if (mt[to] == -1 || dfs(mt[to])) {
            mt[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    // file_raii pomello("data");

    memset(mt, 255, sizeof(int) * 2 * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        g[--v].emplace_back(--u + n);
    }

    for (int i = 0; i < n; ++i) {
        memset(used, 0, sizeof(bool) * MAXN);
        dfs(i);
    }

    int ans = n;
    for (int i = n; i < 2 * n; ++i) {
        if (mt[i] != -1) {
            --ans;
        }
    }
    printf("%d\n", ans);

    return 0;
}
