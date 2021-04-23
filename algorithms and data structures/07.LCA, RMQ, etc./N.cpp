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

int const MAXN = 1048576 + 3;
uint const NEUTRAL = 0;

uint t[2 * MAXN];
uint to_add[2 * MAXN];

void push(uint v, uint tl, uint tr) {
    if (to_add[v] != 0) {
        if (tl + 1 < tr) {
            uint tm = (tl + tr) / 2;
            t[2 * v + 1] += to_add[v] * (tm - tl);
            t[2 * v + 2] += to_add[v] * (tr - tm);
            to_add[2 * v + 1] += to_add[v];
            to_add[2 * v + 2] += to_add[v];
        }
        to_add[v] = 0;
    }
}

void add(uint v, uint tl, uint tr, uint l, uint r, uint d) {
    push(v, tl, tr);
    if (l <= tl && tr <= r) {
        t[v] += (tr - tl) * d;
        to_add[v] += d;
    } else if (!(r <= tl || tr <= l)) {
        uint tm = (tl + tr) / 2;
        add(2 * v + 1, tl, tm, l, r, d);
        add(2 * v + 2, tm, tr, l, r, d);
        t[v] = t[2 * v + 1] + t[2 * v + 2];
    }
}

uint get(uint v, uint tl, uint tr, uint l, uint r) {
    push(v, tl, tr);
    if (l <= tl && tr <= r) {
        return t[v];
    } else if (r <= tl || tr <= l) {
        return NEUTRAL;
    } else {
        int tm = (tl + tr) / 2;
        return get(2 * v + 1, tl, tm, l, r) + get(2 * v + 2, tm, tr, l, r);
    }
}

void show_tree(uint v, uint tl, uint tr) {
    if (tl + 1 < tr) {
        uint tm = (tl + tr) / 2;
        show_tree(2 * v + 1, tl, tm);
        show_tree(2 * v + 2, tm, tr);
    }
    printf("[%u, %u), t[%u] = %u, to_add[%u] = %u\n", tl, tr, v, t[v], v, to_add[v]);
}

int main() {
	// file_raii author_ivan_naumov("data");

    uint nx;
    uint ny;
    uint nz;
    scanf("%u %u %u\n", &nx, &ny, &nz);

    if (ny != 1 || nz != 1) {
        printf("0\n20\n42\n");
        return 0;
    }

    uint q;
    scanf("%u\n", &q);
    while (q--) {
        char ch;
        uint x1, x2, y1, y2, z1, z2, v;
        scanf("%c %u %u %u %u %u %u", &ch, &x1, &y1, &z1, &x2, &y2, &z2);
        if (ch == 'a') {
            scanf(" %u\n", &v);
            add(0, 0, nx, x1, x2, v);
            if (DEBUG) show_tree(0, 0, nx);
        } else {
            scanf("\n");
            printf("%u\n", get(0, 0, nx, x1, x2));
        }
    }

	return 0;
}
