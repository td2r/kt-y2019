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

struct task_t {
    int s;
    int f;
    int c;

    void scan() {
        scanf("%d %d %d", &s, &f, &c);
        f += s;
    }
};

int const MAXN = 2'000 + 3;
int const MAXM = 3 * MAXN + 3;
int const inf = INT_MAX;
ll const INF = LLONG_MAX;

int nt, n, k, m, s, t;
ll d[MAXN];
ll p[MAXN];
int pe[MAXN];
bool used[MAXN];
edge e[2 * MAXM];
vector<int> g[MAXN];
task_t task[MAXN];
int coord[MAXN];

void add_edge(int a, int b, int cap, int cost, int i) {
    e[i] = edge(a, b, cap, cost, i);
    e[i + 1] = edge(b, a, 0, -cost, i + 1);
    g[a].emplace_back(i);
    g[b].emplace_back(i + 1);
}

// void add_edge() {
// }

void ford_bellman() {
    // it's not ford-bellman
    p[0] = 0;
    for (int i = 1; i < n; ++i) {
        p[i] = p[i - 1];
        for (int const& j : g[i]) {
            if (j & 1) {
                int const from = e[j ^ 1].a;
                if (p[from] + e[j ^ 1].cost < p[i]) {
                    p[i] = p[from] + e[j ^ 1].cost;
                }
            }
        }
    }
}

struct cmp {
    bool operator()(int v1, int v2) {
        return d[v1] == d[v2] ? v1 < v2 : d[v1] < d[v2];
    }
};

bool dijkstra() {
    memset(used, 0, sizeof(bool) * n);
    fill(d, d + n, INF);
    d[s] = 0;
    set<int, cmp> q;
    q.insert(s);
    while (!q.empty()) {
        int const v = *q.begin();
        q.erase(q.begin());
        used[v] = true;
        for (int const& j : g[v]) {
            int const to = e[j].b;
            if (e[j].residual() > 0) {
                ll const w = e[j].cost + p[v] - p[to];
                if (d[v] + w < d[to]) {
                    q.erase(to);
                    d[to] = d[v] + w;
                    pe[to] = j;
                    q.insert(to);
                }
            }
        }
    }
    return d[t] != INF;
}

int min_cost_max_flow() {
    int res_cost = 0;
    ford_bellman();
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

    scanf("%d %d", &nt, &k);
    for (int i = 0; i < nt; ++i) {
        task[i].scan();
        coord[2 * i] = task[i].s;
        coord[2 * i + 1] = task[i].f;
    }

    sort(coord, coord + 2 * nt);
    n = (unique(coord, coord + 2 * nt) - coord) + 2;
    s = 0;
    t = n - 1;

    for (int i = 0; i < nt; ++i) {
        add_edge(
                (lower_bound(coord, coord + n - 2, task[i].s) - coord) + 1,
                (lower_bound(coord, coord + n - 2, task[i].f) - coord) + 1,
                1,
                -task[i].c,
                2 * m++);
    }
    for (int i = 1; i < n; ++i) {
        add_edge(i - 1, i, k, 0, 2 * m++);
    }

    min_cost_max_flow();

    for (int i = 0; i < nt; ++i) {
        printf("%d ", e[2 * i].f);
    } printf("\n");

    return 0;
}
