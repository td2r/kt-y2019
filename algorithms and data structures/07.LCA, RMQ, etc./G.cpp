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
ll const NEUTRAL = LLONG_MAX;

int as_pow2(int x) {
	int y = 2;
	while (y < x) {
		y *= 2;
	}
	return y;
}

int N;
ll arr[MAXN];
ll t[2 * MAXN];
ll to_add[2 * MAXN];
ll to_update[2 * MAXN];
bool need_update[2 * MAXN];

void build(int v, int tl, int tr) {
	if (tl + 1 == tr) {
		t[v] = arr[tl];
		return;
	}
	int tm = (tl + tr) / 2;
	build(2 * v + 1, tl, tm);
	build(2 * v + 2, tm, tr);
	t[v] = min(t[2 * v + 1], t[2 * v + 2]);
}

void push(int v, int tl, int tr) {
	if (need_update[v]) {
		if (tl + 1 < tr) {
            t[2 * v + 1] = t[2 * v + 2] = to_update[v];
			to_update[2 * v + 1] = to_update[2 * v + 2] = to_update[v];
            to_add[2 * v + 1] = to_add[2 * v + 2] = 0;
			need_update[2 * v + 1] = need_update[2 * v + 2] = true;
		}
		need_update[v] = false;
	}
	if (to_add[v] != 0) {
		if (tl + 1 < tr) {
            t[2 * v + 1] += to_add[v];
            t[2 * v + 2] += to_add[v];
			to_add[2 * v + 1] += to_add[v];
			to_add[2 * v + 2] += to_add[v];
		}
		to_add[v] = 0;
	}
}

ll get(int v, int tl, int tr, int l, int r) {
	push(v, tl, tr);
	if (l <= tl && tr <= r) {
		return t[v];
	} else if (r <= tl || tr <= l) {
		return NEUTRAL;
	} else {
		int tm = (tl + tr) / 2;
		return min(get(2 * v + 1, tl, tm, l, r),
			get(2 * v + 2, tm, tr, l, r));
	}
}

void add(int v, int tl, int tr, int l, int r, ll x) {
    push(v, tl, tr);
	if (l <= tl && tr <= r) {
		to_add[v] += x;
        t[v] += x;
	} else if (!(r <= tl || tr <= l)) {
		int tm = (tl + tr) / 2;
		add(2 * v + 1, tl, tm, l, r, x);
		add(2 * v + 2, tm, tr, l, r, x);
        t[v] = min(t[2 * v + 1], t[2 * v + 2]);
	}
}

void update(int v, int tl, int tr, int l, int r, ll x) {
    push(v, tl, tr);
	if (l <= tl && tr <= r) {
        t[v] = x;
		need_update[v] = true;
		to_update[v] = x;
		to_add[v] = 0;
	} else if (!(r <= tl || tr <= l)) {
		int tm = (tl + tr) / 2;
		update(2 * v + 1, tl, tm, l, r, x);
		update(2 * v + 2, tm, tr, l, r, x);
        t[v] = min(t[2 * v + 1], t[2 * v + 2]);
	}
}

void show_tree(int v, int tl, int tr) {
    printf("vertex %d {[%d, %d), t = %lld, to_add = %lld, need_update = %s, to_update = %lld}\n",
     v, tl, tr, t[v], to_add[v],
     need_update[v] ? "yes" : "no", to_update[v]);
    if (tl + 1 == tr) {
        return;
    }
    int tm = (tl + tr) / 2;
    show_tree(2 * v + 1, tl, tm);
    show_tree(2 * v + 2, tm, tr);
}

int main() {
	// file_raii author_ivan_naumov("data");

	int n;
	scanf("%d", &n);

	for (int i = 0; i < n; ++i) {
		scanf("%lld", &arr[i]);
	}
	build(0, 0, n);

	char s[20];
	while (scanf("%s", s) != -1) {
		if (s[0] == 's') {
			int l, r;
			ll x;
			scanf("%d %d %lld", &l, &r, &x);
			update(0, 0, n, l - 1, r, x);
		} else if (s[0] == 'a') {
			int l, r;
			ll x;
			scanf("%d %d %lld", &l, &r, &x);
			add(0, 0, n, l - 1, r, x);
		} else {
			int l, r;
			scanf("%d %d", &l, &r);
			printf("%lld\n", get(0, 0, n, l - 1, r));
		}
        // printf("%s--------------------------------------\n", s);
        // show_tree(0, 0, n);
        // printf("--------------------------------------\n");
	}

	return 0;
}
