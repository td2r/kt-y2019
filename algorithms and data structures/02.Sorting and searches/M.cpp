#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <bitset>
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


bool is_sorting_net(int n, int m, int *x, int *y) {
    int testn = (1 << n);
    for (int i = 0; i < testn; ++i) {
        bitset<15> b(i);
        for (int j = 0; j < m; ++j) {
            if (b[x[j]] && !b[y[j]]) {
                b[x[j]] = 0;
                b[y[j]] = 1;
            }
        }
        for (int j = 0, was1 = 0; j < n; ++j) {
            if (b[j]) was1 = 1;
            if (was1 && !b[j]) return false;
        }
    }
    return true;
}


int main() {
    int n, m, k, *x, *y;
    scanf("%d %d %d", &n, &m, &k);
    x = (int*)malloc(sizeof(int) * m);
    y = (int*)malloc(sizeof(int) * m);
    for (int i = 0, sz = 0; i < k; ++i) {
        int r;
        scanf("%d", &r);
        while (r--) {
            scanf("%d %d", &x[sz], &y[sz]);
            --x[sz], --y[sz];
            if (x[sz] > y[sz]) {
                swap(x[sz], y[sz]);
            }
            ++sz;
        }
    }
    if (is_sorting_net(n, m, x, y)) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }
    free(x);
    free(y);
    return 0;
}
