#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cmath>
#include <climits>
#include <algorithm>

#include <bitset>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

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


int const MAXN = 524'288 + 3;  // == 2**18
int const NEUTRAL = INT_MAX;

int as_pow2(int x) {
	int y = 2;
	while (y < x) {
		y *= 2;
	}
	return y;
}

int N;
int t[2 * MAXN];

int get(int l, int r) {
	l += N - 1;
	r += N - 1;
	int lres = NEUTRAL;
	int rres = NEUTRAL;
	while (l < r) {
		if (l % 2 == 0) {
			lres = min(lres, t[l]);
		}
		l /= 2;
		if (r % 2 == 1) {
			rres = min(rres, t[r]);
		}
		r = r / 2 - 1;
	}
	if (l == r) {
		lres = min(lres, t[l]);
	}
	return min(lres, rres);
}


void update(int i, int v) {
	t[i += N - 1] = v;
	do {
		i = (i - 1) / 2;
		t[i] = min(t[2 * i + 1], t[2 * i + 2]);
	} while (i != 0);
}

int main() {
	// file_raii author_ivan_naumov("");

	int n;
	scanf("%d", &n);
	N = as_pow2(n);

	for (int i = 0; i < n; ++i) {
		scanf("%d", &t[N - 1 + i]);
	}
	for (int i = n; i < N; ++i) {
		t[N - 1 + i] = NEUTRAL;
	}
	for (int i = N - 2; i >= 0; --i) {
		t[i] = min(t[2 * i + 1], t[2 * i + 2]);
	}

	int x, y;
	char s[20];
	while (scanf("%s %d %d", s, &x, &y) != -1) {
		if (s[0] == 'm') {
			printf("%d\n", get(x - 1, y - 1));
		} else {
			update(x - 1, y);
		}
	}

	return 0;
}
