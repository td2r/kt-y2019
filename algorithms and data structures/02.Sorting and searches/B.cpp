#include <cstdlib>
#include <stdio.h>
#include <cstring>


int binsearch(int *a, int l, int r, int x) {
    int m = (l + r) / 2;
    while (l + 1 != r) {
        if (a[m] < x) {
            l = m;
        } else {
            r = m;
        }
        m = (l + r) / 2;
    }
    if (a[m] != x) ++m;
    return m;
}


int main() {
    int n, m;
    scanf("%d", &n);
    int *a = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    scanf("%d", &m);
    for (int i = 0, x; i < m; ++i) {
        scanf("%d", &x);
        int p = binsearch(a, 0, n, x);
        if (a[p] != x) {
            printf("-1 -1\n");
        } else {
            printf("%d %d\n", p + 1, binsearch(a, 0, n, x + 1));
        }
    }
    free(a);
    return 0;
}
