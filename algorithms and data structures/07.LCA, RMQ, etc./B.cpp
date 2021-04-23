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


int const MAXN = 100'000 + 3;
int const MAXL = 17;
int const mod = 16714589;



int binlog[MAXN] = { INT_MIN, 0 };
int sparse_table[MAXL][MAXN];

int rmq(int l, int r) {
	int b = binlog[r - l];
	int m = r - (1 << b);
	return min(sparse_table[b][l], sparse_table[b][m]);
}

int main() {
	// file_raii author_ivan_naumov("");

	for (int i = 2; i < MAXN; i *= 2) {
		binlog[i] = binlog[i - 1] + 1;
		for (int j = i + 1; j < 2 * i && j < MAXN; ++j) {
			binlog[j] = binlog[i];
		}
	}

	int n, m;
	int v, u, ans;

	cin >> n >> m >> sparse_table[0][0];
	cin >> u >> v;

	for (int i = 1; i < n; ++i) {
		sparse_table[0][i] = (23 * sparse_table[0][i - 1] + 21563) % mod;
	}
	for (int i = 1; i <= binlog[n]; ++i) {
		for (int j = 0; j + (1 << i) <= n; ++j) {
			sparse_table[i][j] = min(sparse_table[i - 1][j],
									 sparse_table[i - 1][j + (1 << (i - 1))]);
		}
	}

	ans = rmq(min(u, v) - 1, max(u, v));
	for (int i = 1; i < m; ++i) {
		u = ((17 * u + 751 + ans + 2 * i) % n) + 1;
		v = ((13 * v + 593 + ans + 5 * i) % n) + 1;
		ans = rmq(min(u, v) - 1, max(u, v));
	}

	cout << u << ' ' << v << ' ' << ans << endl;

	return 0;
}
