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


const uint MAXN = 1000000;
uint buff[MAXN];

ull merge_sort(uint *x, uint l, uint r) {
    if (l + 1 == r) {
        return 0ULL;
    }
    uint m = (l + r) / 2, len = r - l;
    ull inv = 0;
    inv += merge_sort(x, l, m);
    inv += merge_sort(x, m, r);
    memcpy(buff, x + l, sizeof(uint) * len);
    uint i = 0, j = m - l, k = l;
    while (i < m - l && j < len) {
        if (buff[i] <= buff[j]) {
            x[k] = buff[i];
            ++i;
        } else {
            inv += m - l - i;
            x[k] = buff[j];
            ++j;
        }
        ++k;
    }
    while (i < m - l) {
        x[k] = buff[i];
        ++i, ++k;
    }
    while (j < len) {
        x[k] = buff[j];
        ++j, ++k;
    }
    return inv;
}


int main() {
    uint *x;
    uint n, m;
    scanf("%u %u\n%u %u", &n, &m, &a, &b);
    x = (uint*)malloc(sizeof(uint) * n);
    for (uint i = 0; i < n; ++i) {
        x[i] = nextRand24() % m;
    }
    printf("%llu\n", merge_sort(x, 0, n));
    free(x);
    return 0;
}
