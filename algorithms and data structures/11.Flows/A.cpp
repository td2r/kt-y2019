#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        // freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        // fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first,
                 forwardIterator last,
                 string const& name = "",
                 string const& separator = ", ") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << separator << *first++;
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

struct edge {
    int a;
    int b;
    int c;
    int f;
    int i;

    edge()
    : i(-1)
    {}

    edge(int a, int b)
    : a(a)
    , b(b)
    , c(0)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int c)
    : a(a)
    , b(b)
    , c(c)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int c, int i)
    : a(a)
    , b(b)
    , c(c)
    , f(0)
    , i(i)
    {}

    void scan() {
        scanf("%d %d %d", &a, &b, &c);
        --a, --b;
        f = 0;
        i = -1;
    }

    void print() const {
        printf("{ a=%d, b=%d, c=%d, f=%d residual=%d}\n", a, b, c, f, residual());
    }

    int residual() const {
        return c - f;
    }
};

int const MAXN = 20'000 + 3;
int const MAXM = 200'000 + 3;
int const inf = INT_MAX;
ll const INF = LLONG_MAX;

int n, n1, n2, m, s, t;
int d[MAXN];
edge e[2 * MAXM];
int ptr[MAXN];
int mt[MAXN];
vector<int> g[MAXN];
vector<int> g1[MAXN];
set<int> A;
bool used[MAXN];
int qh;
int qt;
int q[MAXN];

void add_edge(int a, int b, int c, int i) {
    e[i] = edge(a, b, c, i);
    e[i + 1] = edge(b, a, 0, i + 1);
    g[a].emplace_back(i);
    g[b].emplace_back(i + 1);
}

void add_edge(int i) {
    // int a = read_uint();
    // int b = read_uint();
    int a, b;
    scanf("%d %d", &a, &b);
    add_edge(a, b + n1, 1, 2 * i);
    g1[b + n1].emplace_back(a);
}

bool bfs() {
    q[0] = s;
    int qh = 0;
    int qt = 1;
    memset(d, 255, sizeof(int) * n);
    d[s] = 0;
    while (qh != qt && d[t] == -1) {
        int const i = q[qh++];
        for (int const& j : g[i]) {
            int const to = e[j].b;
            if (d[to] == -1 && e[j].f < e[j].c) {
                d[to] = d[i] + 1;
                q[qt++] = to;
            }
        }
    }
    return d[t] != -1;
}

int dfs(int v, int flow) {
    if (flow == 0)
        return 0;
    if (v == t) {
        return flow;
    }
    int const ptr_max = (int) g[v].size();
    for (; ptr[v] < ptr_max; ++ptr[v]) {
        int const j = g[v][ptr[v]];
        int const to = e[j].b;
        if (d[v] + 1 != d[to]) {
            continue;
        }
        int pushed = dfs(to, min(flow, e[j].residual()));
        if (pushed) {
            e[j].f += pushed;
            e[j ^ 1].f -= pushed;
            return pushed;
        }
    }
    return 0;
}

ll dinitz() {
    ll flow = 0;
    while (bfs()) {
        memset(ptr, 0, sizeof(int) * n);
        while (int pushed = dfs(s, inf)) {
            flow += pushed;
        }
    }
    return flow;
}

void dfs1(int v) {
    used[v] = true;
    A.insert(v);
    for (int const& to : g1[v]) {
        if (mt[to] != -1 && !used[mt[to]]) {
            dfs1(mt[to]);
        }
    }
}

void solve() {
    n = n1 + n2 + 2;
    s = 0;
    t = n - 1;
    scanf("%d", &m);
    for (int i = 0; i < m; ++i) {
        add_edge(i);
    }
    for (int i = 1; i <= n1; ++i) {
        add_edge(s, i, 1, 2 * m++);
    }
    for (int i = n1 + 1; i <= n1 + n2; ++i) {
        add_edge(i, t, 1, 2 * m++);
    }
    if (dinitz() == n2) {
        printf("0\n\n");
        return;
    }
    for (int i = 1; i <= n1; ++i) {
        for (int const& j : g[i]) {
            if (e[j].f) {
                mt[i] = e[j].b;
                mt[e[j].b] = i;
                break;
            }
        }
    }
    for (int i = n1 + 1; i <= n1 + n2; ++i) {
        if (mt[i] == -1) {
            dfs1(i);
            break;
        }
    }
    printf("%zd\n", A.size());
    for (int const& i : A) {
        printf("%d ", i - n1);
    } printf("\n\n");
}

int main() {
    // file_raii pomello("data");

    n = MAXN;
    while (scanf("%d %d", &n1, &n2) != -1) {
        memset(used, 0, sizeof(bool) * n);
        memset(mt, 255, sizeof(int) * n);
        A.clear();
        for (int i = 0; i < n; ++i) {
            g[i].clear();
            g1[i].clear();
        }
        solve();
    }

    return 0;
}
