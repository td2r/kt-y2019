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

struct rectangle {
    int x1;
    int y1;
    int x2;
    int y2;

    rectangle() {}
    rectangle(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

    void scan() {
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
    }

    void print() const {
        printf("%d %d %d %d\n", x1, y1, x2, y2);
    }

    ll area() {
        if (x1 > x2 || y1 > y2) return 0;
        return (ll)(x2 - x1) * (ll)(y2 - y1);
    }
};

rectangle intersection(rectangle const &a, rectangle const &b) {
    return rectangle(max(a.x1, b.x1), max(a.y1, b.y1), min(a.x2, b.x2), min(a.y2, b.y2));
}

rectangle intersection(initializer_list<rectangle> ilist) {
    rectangle result(INT_MIN, INT_MIN, INT_MAX, INT_MAX);
    for (auto &it : ilist) {
        result = intersection(result, it);
    }
    return result;
}

int const MAXN = 128 + 3;
int const MAXL = 7 + 1;
ll const mod = 1'000'000'007;

int binlog[MAXN] = { INT_MIN, 0 };
rectangle st[MAXL][MAXL][MAXN][MAXN];

ll get_intersection(int l1, int l2, int r1, int r2) {
    if (l1 > r1) swap(l1, r1);
    if (l2 > r2) swap(l2, r2);
    int p1 = binlog[r1 - l1 + 1];
    int p2 = binlog[r2 - l2 + 1];
    int m1 = r1 + 1 - (1 << p1);
    int m2 = r2 + 1 - (1 << p2);
    return intersection({
            st[p1][p2][l1][l2],
            st[p1][p2][m1][l2],
            st[p1][p2][l1][m2],
            st[p1][p2][m1][m2]
            }).area();
}

int main() {
	// file_raii author_ivan_naumov("data");

    for (int i = 2; i < MAXN; i *= 2) {
        binlog[i] = binlog[i - 1] + 1;
        for (int j = i + 1; j < 2 * i && j < MAXN; ++j) {
            binlog[j] = binlog[i];
        }
    }

    int n, m;
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            st[0][0][i][j].scan();
        }
    }

    for (int j = 1; j <= binlog[m]; ++j) {
        for (int k = 0; k < n; ++k) {
            for (int t = 0; t + (1 << j) <= m; ++t) {
                st[0][j][k][t] = intersection(st[0][j - 1][k][t],
                                              st[0][j - 1][k][t + (1 << (j - 1))]);
            }
        }
    }

    for (int i = 1; i <= binlog[n]; ++i) {
        for (int k = 0; k + (1 << i) <= n; ++k) {
            for (int t = 0; t < m; ++t) {
                st[i][0][k][t] = intersection(st[i - 1][0][k][t],
                                              st[i - 1][0][k + (1 << (i - 1))][t]);
            }
        }
    }

    for (int i = 1; i <= binlog[n]; ++i) {
        for (int j = 1; j <= binlog[m]; ++j) {
            for (int k = 0; k + (1 << i) <= n; ++k) {
                for (int t = 0; t + (1 << j) <= m; ++t) {
                    st[i][j][k][t] = intersection({
                            st[i - 1][j - 1][k][t],
                            st[i - 1][j - 1][k + (1 << (i - 1))][t],
                            st[i - 1][j - 1][k][t + (1 << (j - 1))],
                            st[i - 1][j - 1][k + (1 << (i - 1))][t + (1 << (j - 1))]
                            });
                }
            }
        }
    }

    int q;
    ll A, B, v[4], u[4], result = 0;
    scanf("%d %lld %lld %lld", &q, &A, &B, &v[3]);
    while (q--) {
        u[0] = (A * v[3] + B) % mod;
        u[1] = (A * u[0] + B) % mod;
        u[2] = (A * u[1] + B) % mod;
        u[3] = (A * u[2] + B) % mod;
        swap(v, u);
        result += get_intersection((int)v[0] % n, (int)v[1] % m, (int)v[2] % n, (int)v[3] % m);
        result %= mod;
    }
    printf("%lld\n", result);

	return 0;
}
