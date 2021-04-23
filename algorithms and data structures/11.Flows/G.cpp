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
    int cap;
    int cost;
    int f;
    int i;

    edge()
    : a(0)
    , b(0)
    , cap(0)
    , cost(0)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b)
    : a(a)
    , b(b)
    , cap(0)
    , cost(0)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap)
    : a(a)
    , b(b)
    , cap(cap)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap, int cost)
    : a(a)
    , b(b)
    , cap(cap)
    , cost(cost)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap, int cost, int i)
    : a(a)
    , b(b)
    , cap(cap)
    , cost(cost)
    , f(0)
    , i(i)
    {}

    void scan() {
        scanf("%d %d %d %d", &a, &b, &cap, &cost);
        --a, --b;
        f = 0;
        i = -1;
    }

    void print() const {
        printf("{ a=%d, b=%d, cap=%d, cost=%d, f=%d residual=%d}\n",
                a, b, cap, cost, f, residual());
    }

    int residual() const {
        return cap - f;
    }
};

int const MAXN = 600 + 3;
int const MAXM = MAXN * MAXN;
int const inf = INT_MAX;
ll const INF = LLONG_MAX;

int k, n, m, s, t;
int qh;
int qt;
int q[MAXN];
int d[MAXN];
int p[MAXN];
int pe[MAXN];
int m_id[MAXN];
edge e[2 * MAXM];
vector<int> g[MAXN];
bool used[MAXN];

void add_edge(int a, int b, int cap, int cost, int i) {
    e[i] = edge(a, b, cap, cost, i);
    e[i + 1] = edge(b, a, 0, -cost, i + 1);
    g[a].emplace_back(i);
    g[b].emplace_back(i + 1);
}

// void add_edge() {
// }

bool levit() {
    fill(m_id, m_id + n, 2);
    fill(d, d + n, inf);
    memset(p, 255, sizeof(int) * n);
    d[s] = 0;
    m_id[s] = 1;
    qh = qt = 0;
    q[qt++] = s;
    while (qh != qt) {
        int const v = q[qh];
        qh = (qh + 1) % n;
        m_id[v] = 0;
        for (int const& j : g[v]) {
            int const to = e[j].b;
            if (e[j].residual() > 0 && d[v] + e[j].cost < d[to]) {
                d[to] = d[v] + e[j].cost;
                p[to] = j;
                if (m_id[to] == 0) {
                    qh = (qh == 0) ? n - 1 : qh - 1;
                    q[qh] = to;
                    m_id[to] = 1;
                } else if (m_id[to] == 1) {
                    // no ops.
                } else {
                    q[qt] = to;
                    qt = (qt + 1) % n;
                    m_id[to] = 1;
                }
            }
        }
    }
    return d[t] != inf;
}

bool dijkstra() {
    fill(d, d + n, inf);
    memset(used, 0, sizeof(bool) * n);
    d[s] = 0;
    for (int kek = 0; kek < n; ++kek) {
        int v = -1;
        for (int i = 0; i < n; ++i) {
            if (!used[i] && (v == -1 || d[i] < d[v])) {
                v = i;
            }
        }
        if (d[v] == inf)
            break;
        used[v] = true;
        for (int const& j : g[v]) {
            int const to = e[j].b;
            if (e[j].residual() > 0) {
                int const w = e[j].cost + p[v] - p[to];
                if (d[v] + w < d[to]) {
                    d[to] = d[v] + w;
                    pe[to] = j;
                }
            }
        }
    }
    return d[t] != inf;
}

int min_cost_max_flow() {
    int res_cost = 0;
    while (dijkstra()) {
        int pushed = inf;
        for (int v = t; v != s; v = e[pe[v]].a) {
            pushed = min(pushed, e[pe[v]].residual());
        }
        for (int v = t; v != s; v = e[pe[v]].a) {
            res_cost += pushed * e[pe[v]].cost;
            e[pe[v]].f += pushed;
            e[pe[v] ^ 1].f -= pushed;
        }
        for (int i = 0; i < n; ++i) {
            p[i] += d[i];
        }
    }
    return res_cost;
}

int main() {
    // file_raii pomello("data");

    scanf("%d", &k);
    n = 2 * k + 2;
    s = 0;
    t = n - 1;
    for (int i = 1, cost; i <= k; ++i) {
        for (int j = k + 1; j <= 2 * k; ++j) {
            scanf("%d", &cost);
            add_edge(i, j, 1, cost, 2 * m++);
        }
    }
    for (int i = 1; i <= k; ++i) {
        add_edge(s, i, 1, 0, 2 * m++);
    }
    for (int i = k + 1; i <= 2 * k; ++i) {
        add_edge(i, t, 1, 0, 2 * m++);
    }

    printf("%d\n", min_cost_max_flow());
    for (int i = 1; i <= k; ++i) {
        for (int const& j : g[i]) {
            if (e[j].f == 1) {
                printf("%d %d\n", i, e[j].b - k);
                break;
            }
        }
    }

    return 0;
}
