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

int const MAXN = 262'144;

int r;

struct matrix {
    int a, b, c, d;

    matrix() : a(), b(), c(), d() {}
    matrix(int a, int b, int c, int d)
        : a(a), b(b), c(c), d(d)
    {}

    void scan() {
        scanf("%d%d%d%d", &a, &b, &c, &d);
        a %= r;
        b %= r;
        c %= r;
        d %= r;
    }

    void print() const {
        printf("%d %d\n%d %d\n", a, b, c, d);
    }
};

matrix product(matrix const &m1, matrix const &m2) {
    return matrix(
            (m1.a * m2.a + m1.b * m2.c) % r,
            (m1.a * m2.b + m1.b * m2.d) % r,
            (m1.c * m2.a + m1.d * m2.c) % r,
            (m1.c * m2.b + m1.d * m2.d) % r);
}

matrix const NEUTRAL(1, 0, 0, 1);
matrix arr[MAXN];
matrix t[2 * MAXN];

void build(int v, int tl, int tr) {
    if (tl + 1 == tr) {
        t[v] = arr[tl];
    } else {
        int tm = (tl + tr) / 2;
        build(2 * v + 1, tl, tm);
        build(2 * v + 2, tm, tr);
        t[v] = product(t[2 * v + 1], t[2 * v + 2]);
    }
}

matrix get(int v, int tl, int tr, int l, int r) {
    if (l <= tl && tr <= r) {
        return t[v];
    } else if (r <= tl || tr <= l) {
        return NEUTRAL;
    } else {
        int tm = (tl + tr) / 2;
        return product(get(2 * v + 1, tl, tm, l, r), get(2 * v + 2, tm, tr, l, r));
    }
}


int main() {
	// file_raii author_ivan_naumov("data");

    int n, m;
    scanf("%d %d %d", &r, &n, &m);
    for (int i = 0; i < n; ++i) {
        arr[i].scan();
    }

    build(0, 0, n);

    while (m--) {
        int l, r;
        scanf("%d%d", &l, &r);
        matrix result(get(0, 0, n, l - 1, r));
        result.print();
        if (m) printf("\n");
    }

	return 0;
}
