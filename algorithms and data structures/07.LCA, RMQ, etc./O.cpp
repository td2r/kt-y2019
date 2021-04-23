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

int const MAXN = 200'000 + 3;
int const MAXL = 18 + 2;

int h[MAXN];
int binlog[MAXN] = { INT_MIN, 0 };
vector<int> g[MAXN];
int jnode[MAXL][MAXN];
int jvalue[MAXL][MAXN];

void dfs(int v, int p = -1) {
    for (int to : g[v]) {
        if (to == p) continue;
        h[to] = h[v] + 1;
        for (int i = 1; i < MAXL; ++i) {
            jnode[i][to] = jnode[i - 1][jnode[i - 1][to]];
            jvalue[i][to] = min(jvalue[i - 1][to], jvalue[i - 1][jnode[i - 1][to]]);
        }
        dfs(to, v);
    }
}

int get(int a, int b) {
    int result = INT_MAX;
    while (h[a] > h[b]) {
        result = min(result, jvalue[binlog[h[a] - h[b]]][a]);
        a = jnode[binlog[h[a] - h[b]]][a];
    }
    while (h[b] > h[a]) {
        result = min(result, jvalue[binlog[h[b] - h[a]]][b]);
        b = jnode[binlog[h[b] - h[a]]][b];
    }
    for (int i = MAXL - 1; i >= 0; --i) {
        if (jnode[i][a] != jnode[i][b]) {
            result = min(result, jvalue[i][a]);
            result = min(result, jvalue[i][b]);
            a = jnode[i][a];
            b = jnode[i][b];
        }
    }
    if (a != b) {
        result = min(result, jvalue[0][a]);
        result = min(result, jvalue[0][b]);
    }
    return result;
}

int main() {
	// file_raii author_ivan_naumov("data");

    for (int i = 2; i < MAXN; i *= 2) {
        binlog[i] = binlog[i - 1] + 1;
        for (int j = i + 1; j < 2 * i && j < MAXN; ++j) {
            binlog[j] = binlog[i];
        }
    }

    for (int i = 0; i < MAXL; ++i) {
        jvalue[i][0] = INT_MAX;
    }

    int n;
    scanf("%d", &n);
    for (int i = 1; i < n; ++i) {
        int p, w;
        scanf("%d %d", &p, &w); --p;
        jnode[0][i] = p;
        jvalue[0][i] = w;
        g[p].emplace_back(i);
    }

    dfs(0);

    int m;
    scanf("%d", &m);
    while (m--) {
        int a, b;
        scanf("%d %d", &a, &b);
        --a, --b;
        printf("%d\n", get(a, b));
    }

	return 0;
}
