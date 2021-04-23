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

int const MAXN = 100 + 3;
int const MAXM = 1000 + 3;
int const inf = INT_MAX;
ll const INF = LLONG_MAX;

int n, m, s, t;
int qh;
int qt;
int q[MAXN];
int d[MAXN];
int p[MAXN];
int m_id[MAXN];
edge e[2 * MAXM];
vector<int> g[MAXN];

void add_edge(int a, int b, int cap, int cost, int i) {
    e[i] = edge(a, b, cap, cost, i);
    e[i + 1] = edge(b, a, 0, -cost, i + 1);
    g[a].emplace_back(i);
    g[b].emplace_back(i + 1);
}

void add_edge(int i) {
    int a, b, cap, cost;
    scanf("%d %d %d %d", &a, &b, &cap, &cost);
    --a, --b;
    add_edge(a, b, cap, cost, 2 * i);
}

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

ll min_cost_max_flow() {
    ll res_cost = 0;
    while (levit()) {
        int pushed = inf;
        for (int v = t; v != s; v = e[p[v]].a) {
            pushed = min(pushed, e[p[v]].residual());
        }
        for (int v = t; v != s; v = e[p[v]].a) {
            res_cost += (ll) pushed * e[p[v]].cost;
            e[p[v]].f += pushed;
            e[p[v] ^ 1].f -= pushed;
        }
    }
    return res_cost;
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        add_edge(i);
    }

    printf("%lld\n", min_cost_max_flow());

    return 0;
}
