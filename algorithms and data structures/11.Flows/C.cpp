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

int const MAXN = 500 + 3;
int const MAXM = 20'000 + 3;
ll const INF = LLONG_MAX;

int n, m, s, t;
int d[MAXN];
ll p[MAXN];
ll in[MAXN];
ll out[MAXN];
edge e[2 * MAXM];
unordered_set<int> g[MAXN];

void add_edge(int a, int b, int c, int i) {
    e[i] = edge(a, b, c, i);
    e[i + 1] = edge(b, a, 0, i + 1);
    g[a].insert(i);
    g[b].insert(i + 1);
}

void add_edge(int i) {
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    add_edge(a - 1, b - 1, c, 4 * i);
    add_edge(b - 1, a - 1, c, 4 * i + 2);
}

ll getp(int i) {
    return p[i] = min(in[i], out[i]);
}

void push_flow(int ei, int flow) {
    int const from = e[ei].a;
    int const to = e[ei].b;
    assert(d[from] + 1 == d[to]);
    e[ei].f += flow;
    e[ei ^ 1].f -= flow;
    in[to] -= flow;
    out[from] -= flow;
}

ll Ma_Ku_Ma() {
    ll result = 0;
    vector<unordered_set<int>> g1(n);

    for (int i = 0; i < n; ++i) {
        g1[i] = unordered_set<int>(g[i]);
    }

    memset(in, 0, sizeof(ll) * n);
    in[s] = INF;
    memset(out, 0, sizeof(ll) * n);
    out[t] = INF;
    memset(p, 0, sizeof(ll) * n);
    for (int i = 0; i < n; ++i) {
        for (int const& j : g1[i]) {
            int const to = e[j].b;
            if (d[i] + 1 == d[to]) {
                in[to] += e[j].residual();
                out[i] += e[j].residual();
            }
        }
    }
    for (;;) {
        int v;
        deque<int> dq1;
        deque<ll> dq2;
        for (int i = 0; i < n; ++i) {
            if (getp(i) == 0) {
                dq1.push_front(i);
            }
        }
        while (!dq1.empty()) {
            int const i = dq1.front();
            dq1.pop_front();
            for (int const& j : g1[i]) {
                int to = e[j].b;
                if (d[i] + 1 == d[to]) {
                    in[to] -= e[j].residual();
                    out[i] -= e[j].residual();
                } else if (d[to] + 1 == d[i]) {
                    in[i] -= e[j ^ 1].residual();
                    out[to] -= e[j ^ 1].residual();
                }
                g1[to].erase(j ^ 1);
                if (getp(to) == 0) {
                    dq1.push_front(to);
                }
            }
            g1[i].clear();
        }
        v = -1;
        for (int i = 0; i < n; ++i) {
            if (p[i] != 0 && (v == -1 || p[i] < p[v])) {
                v = i;
            }
        }
        if (v == -1)
            break;
        ll const pv = p[v];
        dq1.push_back(v);
        dq2.push_back(pv);
        while (!dq1.empty()) {
            int const i = dq1.front();
            dq1.pop_front();
            ll to_push = dq2.front();
            dq2.pop_front();
            for (int const& j : g1[i]) {
                int const r = (int) min(to_push, (ll) e[j].residual());
                int const to = e[j].b;
                if (r != 0 && d[i] + 1 == d[to]) {
                    dq1.push_back(to);
                    dq2.push_back((ll) r);
                    push_flow(j, r);
                    to_push -= r;
                    if (to_push == 0) {
                        break;
                    }
                }
            }
        }
        dq1.push_back(v);
        dq2.push_back(pv);
        while (!dq1.empty()) {
            int const i = dq1.front();
            dq1.pop_front();
            ll to_push = dq2.front();
            dq2.pop_front();
            for (int const& j : g1[i]) {
                int const r = (int) min(to_push, (ll) e[j ^ 1].residual());
                int const from = e[j ^ 1].a;
                if (r != 0 && d[from] + 1 == d[i]) {
                    dq1.push_back(from);
                    dq2.push_back((ll) r);
                    push_flow(j ^ 1, r);
                    to_push -= r;
                    if (to_push == 0) {
                        break;
                    }
                }
            }
        }
        result += pv;
    }
    return result;
}

bool bfs() {
    queue<int> q;
    q.push(s);
    memset(d, 255, sizeof(int) * n);
    d[s] = 0;
    while (!q.empty() && d[t] == -1) {
        int const i = q.front();
        q.pop();
        for (int const& j : g[i]) {
            int const to = e[j].b;
            if (d[to] == -1 && e[j].f < e[j].c) {
                d[to] = d[i] + 1;
                q.push(to);
            }
        }
    }
    return d[t] != -1;
}

ll dinitz() {
    ll flow = 0;
    for (;;) {
        if (!bfs()) return flow;
        flow += Ma_Ku_Ma();
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        add_edge(i);
    }

    dinitz();

    vector<int> ans;
    for (int i = 0; i < n; ++i) {
        if (d[i] != -1) {
            ans.emplace_back(i);
        }
    }
    printf("%zd\n", ans.size());
    for (int const& i : ans) {
        printf("%d ", i + 1);
    } printf("\n");

    return 0;
}
