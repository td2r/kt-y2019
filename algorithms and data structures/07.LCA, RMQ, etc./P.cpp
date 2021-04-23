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

	~file_raii() {
		fclose(stdout);
		fclose(stdin);
	}
};

int const MAXN = 524288 + 3;
int const MAXL = 19 + 3;

int n;
int m;
int timer;
int h[MAXN];
int sz[MAXN];
int ind[MAXN];
int tin[MAXN];
int tout[MAXN];
int prnt[MAXN];
int batya[MAXN];
vector<int> g[MAXN];
ll to_add[2 * MAXN];

void dfs1(int v, int p = -1) {
    tin[v] = timer++;
    sz[v] = 1;
    prnt[v] = p;
    for (int &to : g[v]) {
        if (to == p) continue;
        h[to] = h[v] + 1;
        dfs1(to, v);
        sz[v] += sz[to];
    }
    tout[v] = timer++;
}

void dfs2(int v, int p = -1) {
    ind[v] = timer++;
    int heavy_son = -1;
    for (int &son : g[v]) {
        if (son != p && (heavy_son == -1 || sz[son] > sz[heavy_son])) {
            heavy_son = son;
        }
    }
    if (heavy_son != -1) {
        batya[heavy_son] = batya[v];
        dfs2(heavy_son, v);
    }
    for (int &to : g[v]) {
        if (to != p && to != heavy_son) {
            batya[to] = to;
            dfs2(to, v);
        }
    }
}

void push(int v, int tl, int tr) {
    if (tl + 1 < tr && to_add[v] != 0) {
        to_add[2 * v + 1] += to_add[v];
        to_add[2 * v + 2] += to_add[v];
        to_add[v] = 0;
    }
}

void add(int v, int tl, int tr, int l, int r, ll d) {
    if (DEBUG) printf("add(%d, %d, %d, %d, %d, %lld\n", v, tl, tr, l, r, d);
    push(v, tl, tr);
    if (l <= tl && tr <= r) {
        to_add[v] += d;
    } else if (!(r <= tl || tr <= l)) {
        int tm = (tl + tr) / 2;
        add(2 * v + 1, tl, tm, l, r, d);
        add(2 * v + 2, tm, tr, l, r, d);
    }
}

ll get(int v, int tl, int tr, int i) {
    push(v, tl, tr);
    if (tl + 1 == tr) {
        return to_add[v];
    } else {
        int tm = (tl + tr) / 2;
        if (i < tm) {
            return get(2 * v + 1, tl, tm, i);
        } else {
            return get(2 * v + 2, tm, tr, i);
        }
    }
}

bool is_parent(int v, int u) {
    return tin[v] <= tin[u] && tout[u] <= tout[v];
}

void hld_add(int v, int u, ll d) {
    if (DEBUG) printf("hld_add, v = %d, u = %d\n", v, u);
    while (!is_parent(v, u) && !is_parent(u, v)) {
        if (DEBUG) printf("v = %d, u = %d\n", v, u);
        if (h[batya[v]] > h[batya[u]]) {
            add(0, 0, n, ind[batya[v]], ind[v] + 1, d);
            v = prnt[batya[v]];
        } else {
            add(0, 0, n, ind[batya[u]], ind[u] + 1, d);
            u = prnt[batya[u]];
        }
    }
    if (h[v] < h[u]) swap(v, u);
    while (h[batya[v]] > h[u]) {
        if (DEBUG) printf("v = %d, u = %d\n", v, u);
        add(0, 0, n, ind[batya[v]], ind[v] + 1, d);
        v = prnt[batya[v]];
    }
    if (DEBUG) printf("v = %d, u = %d\n", v, u);
    add(0, 0, n, ind[u], ind[v] + 1, d);
}

template <typename T>
void show_array(T *first, T *last, string s) {
    cout << s << " : ";
    if (first != last) {
        cout << *first++;
    }
    while (first != last) {
        cout << ' ' << *first++;
    }
    cout << '\n';
}

void show_tree(int v, int tl, int tr) {
    printf("vertex %d, [%d, %d), to_add = %lld\n", v, tl, tr, to_add[v]);
}

int main() {
	// file_raii author_ivan_naumov("data");

    scanf("%d\n", &n);
    for (int i = 1; i < n; ++i) {
        int v, u;
        scanf("%d %d\n", &v, &u);
        --v, --u;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    timer = 0;
    dfs1(0);
    timer = 0;
    dfs2(0);

    if (DEBUG) {
        show_array(ind, ind + n, "ind");
    }

    scanf("%d\n", &m);
    while (m--) {
        char ch;
        scanf("%c", &ch);
        if (ch == '+') {
            int v, u;
            ll d;
            scanf(" %d %d %lld\n", &v, &u, &d);
            hld_add(v - 1, u - 1, d);
        } else {
            int v;
            scanf(" %d\n", &v);
            printf("%lld\n", get(0, 0, n, ind[v - 1]));
        }
    }

	return 0;
}
