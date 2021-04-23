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
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
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

int const MAXN = 100 + 3;
int const MAXM = 1'000 + 3;
int const INF = INT_MAX;

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

int n, m, s, t;
int d[MAXN];
int p[MAXN];
int in[MAXN];
int out[MAXN];
edge e[2 * MAXM];
unordered_set<int> g[MAXN];

int getp(int i) {
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

int Ma_Ku_Ma() {
    int result = 0;
    vector<unordered_set<int>> g1(n);

    for (int i = 0; i < n; ++i) {
        g1[i] = unordered_set<int>(g[i]);
    }

    memset(in, 0, sizeof(int) * n);
    in[s] = INF;
    memset(out, 0, sizeof(int) * n);
    out[t] = INF;
    memset(p, 0, sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        for (int const& j : g1[i]) {
            int const to = e[j].b;
            if (d[i] + 1 == d[to]) {
                in[to] += e[j].residual();
                out[i] += e[j].residual();
            }
        }
    }
    // printf("\nMa_Ku_Ma\n");
    for (;;) {
        // printf("new iteration\n");
        deque<int> dq;
        for (int i = 0; i < n; ++i) {
            // printf("p[%d] = %d, in[%d] = %d, out[%d] = %d\n", i, getp(i), i, in[i], i, out[i]);
            if (getp(i) == 0) {
                dq.push_front(i);
            }
        }
        while (!dq.empty()) {
            int const i = dq.front();
            dq.pop_front();
            // printf("erasing vertex %d\n", i);
            for (int const& j : g1[i]) {
                int to = e[j].b;
                // e[j].print();
                // e[j ^ 1].print();
                if (d[i] + 1 == d[to]) {
                    in[to] -= e[j].residual();
                    out[i] -= e[j].residual();
                } else if (d[to] + 1 == d[i]) {
                    in[i] -= e[j ^ 1].residual();
                    out[to] -= e[j ^ 1].residual();
                }
                g1[to].erase(j ^ 1);
                if (getp(to) == 0) {
                    dq.push_front(to);
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
        int const pv = p[v];
        // printf("v = %d, pv = %d\n", v, pv);
        // printf("push from v to t\n");
        dq.push_back(v);
        dq.push_back(pv);
        while (!dq.empty()) {
            int const i = dq.front();
            dq.pop_front();
            int to_push = dq.front();
            dq.pop_front();
            for (int const& j : g1[i]) {
                int const r = min(to_push, e[j].residual());
                int const to = e[j].b;
                if (r != 0 && d[i] + 1 == d[to]) {
                    // printf("pushing flow %d for edge ", r); e[j].print();
                    // printf("pushing flow %d for edge ", -r); e[j ^ 1].print();
                    dq.push_back(to);
                    dq.push_back(r);
                    push_flow(j, r);
                    to_push -= r;
                    if (to_push == 0) {
                        break;
                    }
                }
            }
        }
        // printf("push from s to v\n");
        dq.push_back(v);
        dq.push_back(pv);
        while (!dq.empty()) {
            int const i = dq.front();
            dq.pop_front();
            int to_push = dq.front();
            dq.pop_front();
            for (int const& j : g1[i]) {
                int const r = min(to_push, e[j ^ 1].residual());
                int const from = e[j ^ 1].a;
                if (r != 0 && d[from] + 1 == d[i]) {
                    // printf("pushing flow %d for edge ", r); e[j ^ 1].print();
                    // printf("pushing flow %d for edge ", -r); e[j].print();
                    dq.push_back(from);
                    dq.push_back(r);
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
    // printf("result is %d\n", result);
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

int dinitz() {
    int flow = 0;
    for (;;) {
        if (!bfs()) return flow;
        // print_array(d, d + n, "d");
        flow += Ma_Ku_Ma();
        // for (int i = 0; i < m; ++i) {
        //     e[2 * i].print();
        //     e[2 * i + 1].print();
        // }
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    s = 0;
    t = n - 1;
    for (int i = 0; i < m; ++i) {
        e[2 * i].scan();
        e[2 * i].i = i;
        e[2 * i + 1] = edge(e[2 * i].b, e[2 * i].a, 0, i);
        g[e[2 * i].a].insert(2 * i);
        g[e[2 * i].b].insert(2 * i + 1);
    }

    printf("%d\n", dinitz());

    return 0;
}
