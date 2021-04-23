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

int const MAXN = 131072 + 3;
int const NEUTRAL = 0;

int n;
int timer;
int h[MAXN];
int sz[MAXN];
int ind[MAXN];
int tin[MAXN];
int tout[MAXN];
int prnt[MAXN];
int batya[MAXN];
vector<int> g[MAXN];
int t[2 * MAXN];

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

void add(int v, int tl, int tr, int i, int d) {
    if (tl + 1 == tr) {
        t[v] += d;
    } else {
        int tm = (tl + tr) / 2;
        if (i < tm) {
            add(2 * v + 1, tl, tm, i, d);
        } else {
            add(2 * v + 2, tm, tr, i, d);
        }
        t[v] = max(t[2 * v + 1], t[2 * v + 2]);
    }
}

int get(int v, int tl, int tr, int l, int r) {
    if (l <= tl && tr <= r) {
        return t[v];
    } else if (r <= tl || tr <= l) {
        return NEUTRAL;
    } else {
        int tm = (tl + tr) / 2;
        return max(get(2 * v + 1, tl, tm, l, r),
                   get(2 * v + 2, tm, tr, l, r));
    }
}

bool is_parent(int v, int u) {
    return tin[v] <= tin[u] && tout[u] <= tout[v];
}

int hld_max(int v, int u) {
    if (DEBUG) printf("hld_max, v = %d, u = %d\n", v, u);
    int result = NEUTRAL;
    while (!is_parent(v, u) && !is_parent(u, v)) {
        if (h[batya[v]] > h[batya[u]]) {
            result = max(result, get(0, 0, n, ind[batya[v]], ind[v] + 1));
            v = prnt[batya[v]];
        } else {
            result = max(result, get(0, 0, n, ind[batya[u]], ind[u] + 1));
            u = prnt[batya[u]];
        }
    }
    if (h[v] < h[u]) swap(v, u);
    while (h[batya[v]] > h[u]) {
        result = max(result, get(0, 0, n, ind[batya[v]], ind[v] + 1));
        v = prnt[batya[v]];
    }
    return max(result, get(0, 0, n, ind[u], ind[v] + 1));
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
    printf("vertex %d, [%d, %d), t = %d\n", v, tl, tr, t[v]);
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
        show_array(h, h + n, "h");
        show_array(sz, sz + n, "size");
        show_array(ind, ind + n, "ind");
        show_array(tin, tin + n, "tin");
        show_array(tout, tout + n, "tout");
        show_array(prnt, prnt + n, "prnt");
        show_array(batya, batya + n, "batya");
    }

    int q;
    scanf("%d\n", &q);
    while (q--) {
        char ch;
        int v, u;
        scanf("%c %d %d\n", &ch, &u, &v);
        if (ch == 'G') {
            printf("%d\n", hld_max(v - 1, u - 1));
        } else {
            add(0, 0, n, ind[u - 1], v);
        }
    }

	return 0;
}
