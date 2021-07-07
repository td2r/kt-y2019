#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 2'000 + 3;
ll const mod = 1'000'000'007;

int k, m;
ll c[MAXN];
ll t[MAXN];
ll p[MAXN];

int main() {
    scanf("%d %d", &k, &m);
    for (int i = 0; i < k; ++i) {
        scanf("%lld", &c[i]);
    }

    sort(c, c + k);
    k = unique(c, c + k) - c;

    t[0] = p[0] = 1;

    for (ll w = 1; w <= m; ++w) {
        for (int i = 0; i < k && c[i] <= w; ++i) {
            t[w] = (t[w] + p[w - c[i]]) % mod;
        }
        for (ll w1 = 0; w1 <= w; ++w1) {
            p[w] = (p[w] + ((t[w1] * t[w - w1]) % mod)) % mod;
        }
    }

    for (int w = 1; w <= m; ++w) {
        printf("%lld ", t[w]);
    } printf("\n");

    return 0;
}
