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

int const MAXN = 500 + 3;
int const MAXM = 10000 + 3;
ll const INF = LLONG_MAX;

int n, m, s, t;
int d[MAXN];
ll p[MAXN];
ll in[MAXN];
ll out[MAXN];
ll stuck[MAXN];
edge e[2 * MAXM];
unordered_set<int> g[MAXN];
int st[MAXN];
int stsz;
int qh;
int qt;
int q[MAXN];
bool deleted[MAXN];
int ptr[MAXN];

vector<vector<int>> ans;

void add_edge(int a, int b, int c, int i) {
    e[i] = edge(a, b, c, i);
    e[i + 1] = edge(b, a, 0, i + 1);
    g[a].insert(i);
    g[b].insert(i + 1);
}

void add_edge(int i) {
    int a = read_uint() - 1;
    int b = read_uint() - 1;
    int c = read_uint();
    add_edge(a, b, c, 2 * i);
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
    stsz = 0;
    vector<unordered_set<int>> g1(n);

    for (int i = 0; i < n; ++i) {
        g1[i] = unordered_set<int>(g[i]);
    }

    for (int i = 0; i < n; ++i) deleted[i] = false;
    memset(in, 0, sizeof(ll) * n);
    in[s] = INF;
    memset(out, 0, sizeof(ll) * n);
    out[t] = INF;
    // memset(p, 0, sizeof(ll) * n);
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
        for (int i = 0; i < n; ++i) {
            if (!deleted[i] && getp(i) == 0) {
                st[stsz++] = i;
                deleted[i] = true;
            }
        }
        while (stsz != 0) {
            int const i = st[--stsz];
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
                if (!deleted[to] && getp(to) == 0) {
                    st[stsz++] = to;
                    deleted[to] = true;
                }
            }
            g1[i].clear();
        }
        int v = -1;
        for (int i = 0; i < n; ++i) {
            if (p[i] != 0 && (v == -1 || p[i] < p[v])) {
                v = i;
            }
        }
        if (v == -1)
            break;
        qh = 0;
        qt = 1;
        q[0] = v;
        stuck[v] = p[v];
        while (qh != qt) {
            int const i = q[qh++];
            for (int const& j : g1[i]) {
                int const r = (int) min(stuck[i], (ll) e[j].residual());
                int const to = e[j].b;
                if (r != 0 && d[i] + 1 == d[to]) {
                    push_flow(j, r);
                    if (stuck[to] == 0)
                        q[qt++] = to;
                    stuck[to] += r;
                    stuck[i] -= r;
                    if (stuck[i] == 0) {
                        break;
                    }
                }
            }
        }
        stuck[s] = stuck[t] = 0;
        qh = 0;
        qt = 1;
        q[0] = v;
        stuck[v] = p[v];
        while (qh != qt) {
            int const i = q[qh++];
            for (int const& j : g1[i]) {
                int const r = (int) min(stuck[i], (ll) e[j ^ 1].residual());
                int const from = e[j ^ 1].a;
                if (r != 0 && d[from] + 1 == d[i]) {
                    push_flow(j ^ 1, r);
                    if (stuck[from] == 0)
                        q[qt++] = from;
                    stuck[from] += r;
                    stuck[i] -= r;
                    if (stuck[i] == 0) {
                        break;
                    }
                }
            }
        }
        stuck[s] = stuck[t] = 0;
        result += p[v];
    }
    return result;
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

int dfs(int const v, int const flow = 1'000'000'001) {
    if (flow == 0)
        return 0;
    if (v == t) {
        ans.emplace_back(vector<int>({ flow }));
        return flow;
    }
    for (int const& j : g[v]) {
        int const to = e[j].b;
        if (j & 1)
            continue;
        int pushed = dfs(to, min(flow, e[j].f));
        if (pushed != 0) {
            ans.back().emplace_back(j / 2 + 1);
            e[j].f -= pushed;
            e[j ^ 1].f += pushed;
            return pushed;
        }
    }
    return 0;
}

void decompose() {
    while (dfs(s)) {
        reverse(ans.back().begin() + 1, ans.back().end());
    }
}

ll dinitz() {
    ll flow = 0;
    while (bfs()) {
        flow += Ma_Ku_Ma();
    }
    return flow;
}

int main() {
    // file_raii pomello("data");

    n = read_uint();
    m = read_uint();
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        add_edge(i);
    }

    dinitz();

    decompose();

    printf("%zd\n", ans.size());
    for (auto const& path : ans) {
        printf("%d %zd", path[0], path.size() - 1);
        for (size_t i = 1; i < path.size(); ++i) {
            printf(" %d", path[i]);
        } printf("\n");
    }

    return 0;
}
