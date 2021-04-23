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
int const MAXM = 50'000 + 3;

int n, m;
int timer = 0;
int comp[2 * MAXN];
int g[2 * MAXN][2 * MAXN];

int from_input(int x) {
    return (x > 0) ? 2 * x : -2 * x + 1;
}

void dfs(int v) {
    comp[v] = timer;
    for (int to = 2; to <= 2 * n + 1; ++to) {
        if (g[to][v] && comp[to] == -2) {
            dfs(to);
        }
    }
}

void dfs(int v, vector<int> &ts) {
    comp[v] = -2;
    for (int to = 2; to <= 2 * n + 1; ++to) {
        if (g[v][to] && comp[to] == -1) {
            dfs(to, ts);
        }
    }
    ts.emplace_back(v);
}

int main() {
    // file_raii pomello("data");

    memset(comp, 255, sizeof(int) * 2 * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        a = from_input(a);
        b = from_input(b);
        c = from_input(c);
        g[a ^ 1][b] = 1;
        g[b ^ 1][a] = 1;

        g[b ^ 1][c] = 1;
        g[c ^ 1][b] = 1;

        g[a ^ 1][c] = 1;
        g[c ^ 1][a] = 1;
    }

    vector<int> topsort;
    for (int i = 2; i <= 2 * n + 1; ++i) {
        if (comp[i] == -1) {
            dfs(i, topsort);
        }
    }
    reverse(topsort.begin(), topsort.end());
    for (int const& i : topsort) {
        if (comp[i] == -2) {
            dfs(i);
            timer++;
        }
    }

    // print_array("comp", comp + 2, comp + 2 * n + 2);

    for (int i = 1; i <= n; ++i) {
        if (comp[2 * i] == comp[2 * i + 1]) {
            printf("NO\n");
            return 0;
        }
    }

    printf("YES\n");
    for (int i = 1; i <= n; ++i) {
        printf("%d ", (comp[2 * i] > comp[2 * i + 1]) ? i : -i);
    } printf("\n");

    return 0;
}
