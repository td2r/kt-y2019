#include <bits/stdc++.h>
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

int const MAXL = 18 + 2;
int const MAXN = 200'000 + 3;

int n;
int m;
int k;
int timer = 0;
int croot = 0;
int h[MAXN];
int tin[MAXN];
int tout[MAXN];
int first[MAXN];
int st[MAXL][MAXN];
vector<int> g[MAXN];
int binlog[MAXN] = { INT_MIN, 0 };

void dfs(int v, int p = -1) {
    tin[v] = timer++;
    first[v] = k;
    st[0][k++] = v;
    for (int to : g[v]) {
        if (to == p) continue;
        h[to] = h[v] + 1;
        dfs(to, v);
        st[0][k++] = v;
    }
    tout[v] = timer++;
}

int lca(int v, int u) {
    int l = first[v];
    int r = first[u];
    if (l > r) swap(l, r);
    ++r;
    int p = binlog[r - l];
    int lval = st[p][l];
    int rval = st[p][r - (1 << p)];
    return h[lval] <= h[rval] ? lval : rval;
}

bool is_parent(int v, int u) {
    return tin[v] <= tin[u] && tout[u] <= tout[v];
}

int get_lca(int v, int u) {
    int w = lca(v, u);
    if (!is_parent(w, croot)) {
        return w;
    }
    if (lca(croot, v) == w) {
        return lca(croot, u);
    } else {
        return lca(croot, v);
    }
}

void solve() {
    for (int i = 1; i < n; ++i) {
        int v, u;
        scanf("%d %d\n", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    dfs(0);

    for (int i = 1; i <= binlog[k]; ++i) {
        for (int j = 0; j + (1 << i) <= k; ++j) {
            int lval = st[i - 1][j];
            int rval = st[i - 1][j + (1 << (i - 1))];
            if (h[lval] <= h[rval]) {
                st[i][j] = lval;
            } else {
                st[i][j] = rval;
            }
        }
    }

    scanf("%d\n", &m);
    while (m--) {
        char ch;
        int v, u;
        scanf("%c", &ch);
        if (ch == '?') {
            scanf(" %d %d\n", &v, &u);
            printf("%d\n", get_lca(v - 1, u - 1) + 1);
        } else {
            scanf(" %d\n", &v);
            croot = v - 1;
        }
    }

    timer = 0;
    croot = 0;
    k = 0;
    for (int i = 0; i < n; ++i) {
        g[i].clear();
        g[i].shrink_to_fit();
    }
}

int main() {
	// file_raii author_ivan_naumov("data");

    for (int i = 2; i < MAXN; i *= 2) {
        binlog[i] = binlog[i - 1] + 1;
        for (int j = i + 1; j < 2 * i && j < MAXN; ++j) {
            binlog[j] = binlog[i];
        }
    }

    while (scanf("%d\n", &n) != -1 && n != 0) {
        solve();
    }

	return 0;
}
