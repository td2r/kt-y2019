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


const uint BASE = 0x10000, MAXN = 100001;
uint cnt[BASE], ind[BASE], x[MAXN], y[MAXN];


int main() {
    uint t, n;
    scanf("%u %u\n%u %u", &t, &n, &a, &b);
    while (t--) {
        for (uint i = 1; i <= n; ++i) {
            x[i] = nextRand32();
        }
        memset(cnt, 0, sizeof(uint) * BASE);
        ind[0] = 1;
        for (uint i = 1; i <= n; ++i) {
            ++cnt[(x[i] << 16) >> 16];
        }
        for (uint i = 1; i < BASE; ++i) {
            ind[i] = ind[i - 1] + cnt[i - 1];
        }
        for (uint i = 1; i <= n; ++i) {
            y[ind[(x[i] << 16) >> 16]++] = x[i];
        }
        swap(x, y);
        memset(cnt, 0, sizeof(uint) * BASE);
        ind[0] = 1;
        for (uint i = 1; i <= n; ++i) {
            ++cnt[x[i] >> 16];
        }
        for (uint i = 1; i < BASE; ++i) {
            ind[i] = ind[i - 1] + cnt[i - 1];
        }
        for (uint i = 1; i <= n; ++i) {
            y[ind[x[i] >> 16]++] = x[i];
        }
        swap(x, y);
        ull res = 0;
        for (ull i = 1; i <= n; ++i) {
            res += i * x[i];
        }
        printf("%llu\n", res);
    }
    return 0;
}
