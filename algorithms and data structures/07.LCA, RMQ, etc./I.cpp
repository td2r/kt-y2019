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

int timer = 0;
struct request {
    int l;
    int r;
    int index;

    request() {}
    request(int l, int r) : l(l), r(r), index(timer++) {}
    request(int l, int r, int index) : l(l), r(r), index(index) {}

    void scan() {
        scanf("%d %d", &l, &r);
        --l;
        index = timer++;
    }
};

int const MAXN = 262144 + 3;
int const BLOCK_SIZE = 512;
int const MAXM = 1'000'000 + 3;

bool operator < (request const &a, request const &b) {
    if (a.l / BLOCK_SIZE != b.l / BLOCK_SIZE) {
        return a.l < b.l;
    }
    return a.r < b.r;
}

int a[MAXN];
int k[MAXM];
ll answer[MAXN];
request req[MAXN];

int main() {
	// file_raii author_ivan_naumov("data");

    int n;
    int m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    for (int i = 0; i < m; ++i) {
        req[i].scan();
    }

    sort(req, req + m);

    ll f = 0;
    int l = 0;
    int r = 0;
    for (int i = 0; i < m; ++i) {
        while (l > req[i].l) {
            --l;
            f += (2LL * k[a[l]] + 1LL) * a[l];
            ++k[a[l]];
        }
        while (r < req[i].r) {
            f += (2LL * k[a[r]] + 1LL) * a[r];
            ++k[a[r]];
            ++r;
        }
        while (l < req[i].l) {
            f -= (2LL * k[a[l]] - 1LL) * a[l];
            --k[a[l]];
            ++l;
        }
        while (r > req[i].r) {
            --r;
            f -= (2LL * k[a[r]] - 1LL) * a[r];
            --k[a[r]];
        }
        answer[req[i].index] = f;
    }
    for (int i = 0; i < m; ++i) {
        printf("%lld\n", answer[i]);
    }

	return 0;
}
