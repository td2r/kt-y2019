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
int const MAXM = 10'000 + 3;
int const INF = 2'000'000'001;

ll ans = 0;
int n, m, root = 0;
vector<pair<int, int>> g[MAXN];
vector<pair<int, int>> gt[MAXN];
vector<pair<int, int>> h[MAXN];
vector<pair<int, int>> ht[MAXN];
int timer_used = 1;
int used[MAXN];
int min_e[MAXN];
vector<int> topsort;
int timer_comp = 0;
int comp[MAXN];

int dfs(int v) {
    used[v] = timer_used;
    int ret = 1;
    for (auto const& e : g[v]) {
        if (used[e.first] != timer_used) {
            ret += dfs(e.first);
        }
    }
    return ret;
}

int dfs_zero(int v) {
    used[v] = timer_used;
    int ret = 1;
    for (auto const& e : h[v]) {
        if (used[e.first] != timer_used) {
            ret += dfs_zero(e.first);
        }
    }
    return ret;
}

void make_topsort(int v) {
    used[v] = timer_used;
    for (auto const& e : h[v]) {
        if (used[e.first] != timer_used) {
            make_topsort(e.first);
        }
    }
    topsort.emplace_back(v);
}

void dfs_cond(int v) {
    used[v] = timer_used;
    comp[v] = timer_comp;
    for (auto const& e : ht[v]) {
        if (used[e.first] != timer_used) {
            dfs_cond(e.first);
        }
    }
}

void print_data() {
    printf("n = %d\nroot = %d\n", n, root);
    printf("g:\n");
    for (int i = 0; i < n; ++i) {
        for (auto const& e : g[i]) {
            printf("%d %d %d\n", i + 1, e.first + 1, e.second);
        }
    }
    printf("gt:\n");
    for (int i = 0; i < n; ++i) {
        for (auto const& e : gt[i]) {
            printf("%d %d %d\n", e.first + 1, i + 1, e.second);
        }
    }
    printf("h:\n");
    for (int i = 0; i < n; ++i) {
        for (auto const& e : h[i]) {
            printf("%d %d %d\n", i + 1, e.first + 1, e.second);
        }
    }
    printf("ht:\n");
    for (int i = 0; i < n; ++i) {
        for (auto const& e : ht[i]) {
            printf("%d %d %d\n", e.first + 1, i + 1, e.second);
        }
    }
    printf("topsort:");
    for (int const& i : topsort) {
        printf(" %d", i + 1);
    } printf("\n");
    for (int i = 0; i < n; ++i) {
        printf("comp[%d] = %d\n", i + 1, comp[i] + 1);
    }
    printf("=====================================================================\n");
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        --v, --u;
        g[v].emplace_back(make_pair(u, w));
        gt[u].emplace_back(make_pair(v, w));
    }

    if (dfs(0) != n) {
        printf("NO\n");
        return 0;
    }
    timer_used++;

    while (true) {
        fill(min_e, min_e + n, INF);
        for (int i = 0; i < n; ++i) {
            for (auto const& e : g[i]) {
                min_e[e.first] = min(min_e[e.first], e.second);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (i == root) continue;
            ans += min_e[i];
        }
        for (int i = 0; i < n; ++i) {
            for (auto const& e : g[i]) {
                if (e.second == min_e[e.first]) {
                    h[i].emplace_back(e);
                    ht[e.first].emplace_back(make_pair(i, e.second));
                }
            }
        }
        // print_data();
        if (dfs_zero(root) == n) {
            printf("YES\n%lld\n", ans);
            return 0;
        }
        timer_used++;
        for (int i = 0; i < n; ++i) {
            if (used[i] != timer_used) {
                make_topsort(i);
            }
        }
        timer_used++;
        reverse(topsort.begin(), topsort.end());
        for (int const& i : topsort) {
            if (used[i] != timer_used) {
                dfs_cond(i);
                timer_comp++;
            }
        }
        // print_data();
        timer_used++;
        for (int i = 0; i < n; ++i) {
            h[i].clear();
            ht[i].clear();
        }
        for (int i = 0; i < n; ++i) {
            for (auto const& e : g[i]) {
                if (comp[i] != comp[e.first]) {
                    h[comp[i]].emplace_back(make_pair(comp[e.first], e.second - min_e[e.first]));
                    ht[comp[e.first]].emplace_back(make_pair(comp[i], e.second - min_e[e.first]));
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            swap(g[i], h[i]);
            swap(gt[i], ht[i]);
            h[i].clear();
            ht[i].clear();
        }
        topsort.clear();
        n = timer_comp;
        root = comp[root];
        timer_comp = 0;
        // print_data();
    }

    return 0;
}
