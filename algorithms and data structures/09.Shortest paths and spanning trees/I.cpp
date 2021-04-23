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

struct edge {
    int v;
    int u;
    int w;

    // edge(int v, int u, int w)
    // : v(v)
    // , u(u)
    // , w(w)
    // {}

    void scan() {
        scanf("%d %d %d", &v, &u, &w);
        --v, --u;
    }

    bool operator<(edge const& other) {
        if (other.w != w) {
            return w < other.w;
        } else if (other.v != v) {
            return v < other.v;
        } else {
            return u < other.u;
        }
    }
};

int const MAXN = 1'000 + 3;
int const MAXM = 10'000 + 3;
int const INF = 2'000'000'001;

int n, m;
int parent[MAXN];
int sz[MAXN];
edge e[MAXM];

void make_sets() {
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        sz[i] = 1;
    }
}

int find_set(int v) {
    return (parent[v] == v) ? v : parent[v] = find_set(parent[v]);
}

int union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (sz[a] < sz[b])
            swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
    }
    return sz[a];
}


int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    if (n == 1) {
        printf("YES\n0\n");
        return 0;
    }
    for (int i = 0; i < m; ++i) {
        e[i].scan();
    }

    sort(e, e + m);

    int ans = INF;

    for (int i = 0; i <= m - n + 1; ++i) {
        make_sets();
        int max_e = INF;
        for (int j = i; j < m; ++j) {
            if (union_sets(e[j].v, e[j].u) == n) {
                max_e = e[j].w;
                break;
            }
        }
        if (max_e != INF)
            ans = min(ans, max_e - e[i].w);
    }

    if (ans != INF) {
        printf("YES\n%d\n", ans);
    } else {
        printf("NO\n");
    }

    return 0;
}
