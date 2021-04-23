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

int const MAXN = 50'000 + 3;

int n;
int c[MAXN];
int nest[2][MAXN];
int w[2 * MAXN + 1];
int ind[2 * MAXN + 1];

int timer = 0;
int comp[2 * MAXN];
vector<int> topsort;
vector<int> g[2 * MAXN];
vector<int> gt[2 * MAXN];

void dfs1(int v) {
    comp[v] = -2;
    for (int const& to : g[v]) {
        if (comp[to] == -1) {
            dfs1(to);
        }
    }
    topsort.emplace_back(v);
}

void dfs2(int v) {
    comp[v] = timer;
    for (int const& to : gt[v]) {
        if (comp[to] == -2) {
            dfs2(to);
        }
    }
}

int main() {
    // file_raii pomello("data");

    memset(nest, 255, sizeof(int) * 2 * MAXN);
    memset(comp, 255, sizeof(int) * 2 * MAXN);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &c[i]);
    }
    for (int i = 0; i < 2 * n; ++i) {
        scanf("%d", &w[i]);
        --w[i];
        ind[i] = (nest[0][w[i]] == -1) ? 0 : 1;
        nest[ind[i]][w[i]] = i;
    }
    ind[2 * n] = 0;
    w[2 * n] = w[0];

    for (int i = 0; i < 2 * n; ++i) {
        if (w[i] != w[i + 1] && c[w[i]] == c[w[i + 1]]) {
            // state "wire x goes to y nest" looks like 2 * x + y
            // negate statement = negate nest index(0/1)
            g[2 * w[i] + ind[i]].emplace_back(2 * w[i + 1] + (ind[i + 1] ^ 1));
            g[2 * w[i + 1] + ind[i + 1]].emplace_back(2 * w[i] + (ind[i] ^ 1));
            // transposed graph
            gt[2 * w[i + 1] + (ind[i + 1] ^ 1)].emplace_back(2 * w[i] + ind[i]);
            gt[2 * w[i] + (ind[i] ^ 1)].emplace_back(2 * w[i + 1] + ind[i + 1]);
        }
    }

    for (int i = 0; i < 2 * n; ++i) {
        if (comp[i] == -1) {
            dfs1(i);
        }
    }
    reverse(topsort.begin(), topsort.end());
    for (int const& i : topsort) {
        if (comp[i] == -2) {
            dfs2(i);
            timer++;
        }
    }

    if (DEBUG) {
        print_array("c", c, c + n);
        print_array("w", w, w + 2 * n + 1);
        print_array("ind", ind, ind + 2 * n + 1);
        printf("gt:\n");
        for (int i = 0; i < 2 * n; ++i) {
            for (int const& j : gt[i]) {
                printf("%d %d\n", i, j);
            }
        }
        print_array("topsort", topsort.begin(), topsort.end());
        print_array("comp", comp, comp + 2 * n);
    }

    for (int i = 0; i < n; ++i) {
        if (comp[2 * i] == comp[2 * i + 1]) {
            printf("NO\n");
            return 0;
        }
    }

    printf("YES\n");
    for (int i = 0; i < n; ++i) {
        if (comp[2 * i] > comp[2 * i + 1]) {
            printf("%d ", nest[0][i] + 1);
        } else {
            printf("%d ", nest[1][i] + 1);
        }
    } printf("\n");

    return 0;
}
