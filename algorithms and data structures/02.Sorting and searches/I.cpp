#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
#define mp make_pair
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


const int MAXM = 1e5;
ll c[MAXM];


bool can_buy(int n, int m, int p, int *a, int *b, int *t, ll T) {
    for (int i = 0; i < m; ++i) {
        if (a[i] == 0) {
            if (b[i] + t[i] <= T) {
                return true;
            }
            c[i] = 0;
        } else {
            c[i] = max(0LL, (T - b[i] - t[i]) / a[i]) * -1LL;
        }
    }
    sort(c, c + m);
    ll C = 0;
    for (int i = 0; i < n && i < m; ++i) {
        C -= c[i];
    }
    return C >= p;
}


int main() {
    int n, m, p, *a, *b, *t;
    scanf("%d", &m);
    a = (int*)malloc(sizeof(int) * m);
    b = (int*)malloc(sizeof(int) * m);
    t = (int*)malloc(sizeof(int) * m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &a[i], &b[i], &t[i]);
    }
    scanf("%d %d", &n, &p);
    ll l = 0, r = 1e11, x = 5e10;
    while (l != x) {
        if (can_buy(n, m, p, a, b, t, x)) {
            r = x;
        } else {
            l = x;
        }
        x = (l + r) / 2;
    }
    if (!can_buy(n, m, p, a, b, t, x)) {
        ++x;
    }
    printf("%lld\n", x);
    free(a);
    free(b);
    free(t);
    return 0;
}
