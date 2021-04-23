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

int const MAXN = 600 + 3;
int const MAXM = MAXN * MAXN + 2 * MAXN + 3;
int const inf = INT_MAX;
ll const INF = LLONG_MAX;

int minw;
int n, m, k, s, t;
int d[MAXN];
edge e[2 * MAXM];
int w[MAXN][MAXN];
int ptr[MAXN];
vector<int> g[MAXN];
int st[MAXN];
int stsz;
int qh;
int qt;
int q[MAXN];
bool deleted[MAXN];

void add_edge(int a, int b, int c, int i) {
    e[i] = edge(a, b, c, i);
    e[i + 1] = edge(b, a, 0, i + 1);
    g[a].emplace_back(i);
    g[b].emplace_back(i + 1);
}

// void add_edge(int i) {
//     throw 1;
// }

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
            if (d[to] == -1 && w[i][to] >= minw && e[j].f < e[j].c) {
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
        if (d[v] + 1 != d[to] || w[v][to] < minw) {
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

int main() {
    // file_raii pomello("data");

    k = read_uint();
    n = 2 * k + 2;
    m = 0;
    s = 0;
    t = n - 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            w[i][j] = inf;
        }
    }
    for (int i = 1; i <= k; ++i) {
        add_edge(s, i, 1, 2 * m++);
    }
    for (int i = 1; i <= k; ++i) {
        for (int j = k + 1; j <= 2 * k; ++j) {
            w[i][j] = read_uint();
            add_edge(i, j, 1, 2 * m++);
        }
    }
    for (int i = k + 1; i <= 2 * k; ++i) {
        add_edge(i, t, 1, 2 * m++);
    }

    int l = 0;
    int r = 1'000'001;
    minw = r / 2;
    while (l != minw) {
        if (dinitz() == k) {
            l = minw;
        } else {
            r = minw;
        }
        minw = (l + r) / 2;
        for (int i = 0; i < m; ++i) {
            e[2 * i].f = 0;
            e[2 * i + 1].f = 0;
        }
    }

    printf("%d\n", minw);

    return 0;
}
