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

int const MAXN = 131'072;

int t[2 * MAXN];

void build(int v, int tl, int tr) {
    if (tl + 1 == tr) {
        t[v] = tl;
    } else {
        int tm = (tl + tr) / 2;
        build(2 * v + 1, tl, tm);
        build(2 * v + 2, tm, tr);
        t[v] = min(t[2 * v + 1], t[2 * v + 2]);
    }
}

int get(int v, int tl, int tr, int l, int r) {
    if (l <= tl && tr <= r) {
        return t[v];
    } else if (tr <= l || r <= tl) {
        return MAXN;
    } else {
        int tm = (tl + tr) / 2;
        return min(get(2 * v + 1, tl, tm, l, r),
                get(2 * v + 2, tm, tr, l, r));
    }
}

void update(int v, int tl, int tr, int i, int value) {
    if (tl + 1 == tr) {
        t[v] = (value == 0 ? tl : MAXN);
    } else {
        int tm = (tl + tr) / 2;
        if (i < tm)
            update(2 * v + 1, tl, tm, i, value);
        else
            update(2 * v + 2, tm, tr, i, value);
        t[v] = min(t[2 * v + 1], t[2 * v + 2]);
    }
}

int main() {
	// file_raii author_ivan_naumov("data");

    int x;
    char s[20];
    int n, m;
    scanf("%d%d", &n, &m);
    build(0, 0, n);
    for (int i = 0; i < m; ++i) {
        scanf("%s %d", s, &x);
        if (s[1] == 'n') {
            int p = get(0, 0, n, x - 1, n);
            if (p == MAXN) {
                p = get(0, 0, n, 0, x);
            }
            printf("%d\n", p + 1);
            update(0, 0, n, p, 1);
        } else {
            update(0, 0, n, x - 1, 0);
        }
    }

	return 0;
}
