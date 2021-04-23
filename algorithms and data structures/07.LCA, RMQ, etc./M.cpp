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
int const MAXM = 20'000'000 + 3;

int n;
int m;
int k;
int a[MAXM];
int h[MAXN];
int first[MAXN];
int st[MAXL][MAXN];
vector<int> g[MAXN];
int binlog[MAXN] = { INT_MIN, 0 };

void dfs(int v, int p = -1) {
    first[v] = k;
    st[0][k++] = v;
    for (int to : g[v]) {
        if (to == p) continue;
        h[to] = h[v] + 1;
        dfs(to, v);
        st[0][k++] = v;
    }
}

int lca(int v, int u) {
    // printf("lca(%d, %d)\n", v, u);
    int l = first[v];
    int r = first[u];
    if (l > r) swap(l, r);
    ++r;
    int p = binlog[r - l];
    int lval = st[p][l];
    int rval = st[p][r - (1 << p)];
    return h[lval] <= h[rval] ? lval : rval;
}

void show_st() {
    for (int i = binlog[k]; i >= 0; --i) {
        for (int j = 0; j + (1 << i) <= k; ++j) {
            printf("%d ", st[i][j]);
        } printf("\n");
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

    ll x, y, z, answer = 0;
    scanf("%d %d", &n, &m);
    for (int i = 1; i < n; ++i) {
        int p;
        scanf("%d", &p);
        g[p].emplace_back(i);
    }
    scanf("%d %d", &a[1], &a[2]);
    scanf("%lld %lld %lld", &x, &y, &z);
    for (int i = 3; i <= 2 * m; ++i) {
        a[i] = (int)((x * a[i - 2] + y * a[i - 1] + z) % (ll)n);
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

    // show_st();

    int v = 0;
    for (int i = 1; i <= m; ++i) {
        v = lca((a[2 * i - 1] + v) % n, a[2 * i]);
        answer += v;
    }
    printf("%lld\n", answer);

	return 0;
}
