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


int const MAXN = (2 << 24) + 3;
uint const mod = (2 << 24);
int const n = (2 << 24);


uint a, b;
uint cur = 0;

uint next_rand() {
	cur = cur * a + b;
	return (cur >> 8);
}

int m, q;
uint d[MAXN];


int main() {
	// file_raii author_ivan_naumov("");

	cin >> m >> q;
	cin >> a >> b;

	while (m--) {
		uint add = next_rand();
		uint l = next_rand();
		uint r = next_rand();
		if (l > r)
			swap(l, r);
		d[l] += add;
		d[r + 1] -= add;
	}

	uint balance = 0;
	for (int i = 0; i < n; ++i) {
		balance += d[i];
		d[i] = balance;
		if (i) d[i] += d[i - 1];
	}

	uint answer = 0;
	while (q--) {
		uint l = next_rand();
		uint r = next_rand();
		if (l > r)
			swap(l, r);
		answer += l == 0 ? d[r] : d[r] - d[l - 1];
	}

	cout << answer << endl;
	return 0;
}
