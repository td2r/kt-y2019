#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 150'000 + 1;

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

void build_sufmas(char const* s, int* p) {
    int n = (int) strlen(s) + 1;
    int* pn = (int*) malloc(sizeof(int) * n);
    int* c = (int*) malloc(sizeof(int) * n);
    int* cn = (int*) malloc(sizeof(int) * n);
    int* cnt = (int*) malloc(sizeof(int) * max(256, n));

    memset(cnt, 0, sizeof(int) * 256);
    for (int i = 0; i < n; ++i) {
        ++cnt[(int) s[i]];
    }
    for (int i = 1; i < 256; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; --i) {
        p[--cnt[(int) s[i]]] = i;
    }

    c[p[0]] = 0;
    for (int i = 1; i < n; ++i) {
        c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);
    }

    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; ++i) {
            pn[i] = (p[i] - k + n) % n;
        }

        memset(cnt, 0, sizeof(int) * n);
        for (int i = 0; i < n; ++i) {
            ++cnt[c[pn[i]]];
        }
        for (int i = 1; i <= c[p[n - 1]]; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            p[--cnt[c[pn[i]]]] = pn[i];
        }

        cn[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            int jp = p[i - 1];
            int j = p[i];
            cn[j] = cn[jp] + (c[j] != c[jp] || c[(j + k) % n] != c[(jp + k) % n]);
        }
        swap(c, cn);
    }

    free(cnt);
    free(cn);
    free(c);
    free(pn);
}

void build_lcp(char const* s, int const* p, int* lcp) {
    int n = (int) strlen(s) + 1;
    int* r = (int*) malloc(sizeof(int) * n);

    for (int i = 0; i < n; ++i) {
        r[p[i]] = i;
    }

    for (int i = 0; i < n; ++i) {
        if (r[i] == n - 1) continue;
        int j = p[r[i] + 1];
        int len = i == 0 ? 0 : max(0, lcp[r[i - 1]] - 1);
        while (s[i + len] == s[j + len]) {
            ++len;
        }
        lcp[r[i]] = len;
    }

    free(r);
}

struct edge_t {
    int from;
    int to;
    int l;
    int r;

    edge_t()
    : from(0)
    , to(0)
    , l(0)
    , r(0)
    {}

    edge_t(int from, int to, int l, int r)
    : from(from)
    , to(to)
    , l(l)
    , r(r)
    {}

    int len() const {
        return r - l + 1;
    }

    void print() const {
        printf("%d %d %d %d\n", from + 1, to + 1, l + 1, r + 1);
    }
};

int k = 0; // edges number
edge_t edge[2 * MAXN];

struct node_t {
    vector<int> adj;
};

int m = 1; // verteces number
node_t node[2 * MAXN];
int up[2 * MAXN] = { -1 };
int depth[2 * MAXN];

int n, l;
char s[MAXN];
int p[MAXN];
int lcp[MAXN];
int dp[2 * MAXN];

int split_edge(int i, int x) {
    edge_t& e = edge[i];
    // add new vertex
    up[m] = i;
    depth[m] = depth[e.to] - x;
    node[m].adj.emplace_back(k);
    // add new edge
    edge[k] = edge_t(m, e.to, e.r - x + 1, e.r);
    // chagne edge[i].to
    up[e.to] = k;
    // change edge[i]
    e.to = m;
    e.r -= x;

    ++m, ++k;
    return m - 1;
}

void build_suftree() {
    int v = 0;
    for (int i = 1; i <= n; ++i) {
        int depth_lca = lcp[i - 1];
        while (v != 0 && depth_lca <= depth[edge[up[v]].from]) {
            v = edge[up[v]].from;
        }
        if (depth_lca < depth[v]) {
            v = split_edge(up[v], depth[v] - depth_lca);
        }
        node[v].adj.emplace_back(k);
        edge[k] = edge_t(v, m, p[i] + depth_lca, n - 1);
        up[m] = k;
        depth[m] = n - p[i];
        dp[m] = 1;
        v = m;
        ++m, ++k;
    }
}

ll value = 0;
int refren = 0;

int dfs(int v) {
    for (int i : node[v].adj) {
        dp[v] += dfs(edge[i].to);
    }
    if ((ll) dp[v] * depth[v] > value) {
        refren = v;
        value = (ll) dp[v] * depth[v];
    }
    return dp[v];
}

int main() {
    scanf("%d %d", &n, &l);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &l);
        s[i] = (char) l;
    }

    build_sufmas(s, p);
    build_lcp(s, p, lcp);
    build_suftree();

    assert(dfs(0) == n);

    printf("%lld\n%d\n", value, depth[refren]);
    stack<int> st;
    while (refren != 0) {
        st.push(refren);
        refren = edge[up[refren]].from;
    }

    while (!st.empty()) {
        int u = st.top();
        st.pop();
        for (int i = edge[up[u]].l; i <= edge[up[u]].r; ++i) {
            printf("%d ", (int) s[i]);
        }
    }
    printf("\n");

    return 0;
}
