#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


uint a, b, cur = 0;
uint nextRand24() {
    cur = cur * a + b;
    return cur >> 8;
}
uint nextRand32() {
    uint a = nextRand24(), b = nextRand24();
    return (a << 8) ^ b;
}


struct Item {
    ll c, *pref, *suff, sum;

    Item() : c(), pref(), suff(), sum() {}
    Item(ll c, ll *pref, ll *suff, ll sum) : c(c), pref(pref), suff(suff), sum(sum) {}
};


Item f(int *x, int l, int r, ll k) {
    ll *pref = (ll*)malloc(sizeof(ll) * (r - l)), *suff = (ll*)malloc(sizeof(ll) * (r - l));
    if (l + 1 == r) {
        pref[0] = (ll)x[l];
        suff[0] = (ll)x[l];
        return Item((x[l] <= k ? 1 : 0), pref, suff, (ll)x[l]);
    }
    int m = (l + r) / 2, len1 = m - l, len2 = r - m;
    Item item1 = f(x, l, m, k);
    Item item2 = f(x, m, r, k);
    ll c = item1.c + item2.c;
    for (int i = 0, j = len2 - 1; i < len1; ++i) {
        while (j >= 0 && item1.suff[i] + item2.pref[j] > k) {
            --j;
        }
        c += j + 1;
    }
    for (int j = 0; j < len2; ++j)
        item2.pref[j] += item1.sum;
    for (int i = 0, j = 0, t = 0; i < len1 || j < len2; ++t) {
        if (i == len1) {
            pref[t] = item2.pref[j++];
        } else if (j == len2) {
            pref[t] = item1.pref[i++];
        } else {
            if (item1.pref[i] <= item2.pref[j]) {
                pref[t] = item1.pref[i++];
            } else {
                pref[t] = item2.pref[j++];
            }
        }
    }
    for (int i = 0; i < len1; ++i)
        item1.suff[i] += item2.sum;
    for (int i = 0, j = 0, t = 0; i < len1 || j < len2; ++t) {
        if (i == len1) {
            suff[t] = item2.suff[j++];
        } else if (j == len2) {
            suff[t] = item1.suff[i++];
        } else {
            if (item1.suff[i] <= item2.suff[j]) {
                suff[t] = item1.suff[i++];
            } else {
                suff[t] = item2.suff[j++];
            }
        }
    }
    free(item1.pref);
    free(item2.pref);
    free(item1.suff);
    free(item2.suff);
    return Item(c, pref, suff, item1.sum + item2.sum);
}


int main() {
    ll k;
    int n, *x;
    scanf("%d %lld\n%u %u", &n, &k, &a, &b);
    x = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        x[i] = (int)nextRand32();
    }
    Item item = f(x, 0, n, k - 1);
    printf("%lld\n", (ll)n * (n + 1) / 2 - item.c);
    free(item.pref);
    free(item.suff);
    free(x);
    return 0;
}
