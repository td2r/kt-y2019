#include <cstdlib>
#include <stdio.h>
#include <cstring>


void showArr(int *a, int n) {
    if (n) {
        printf("%d", a[0]);
    }
    for (int i = 1; i < n; ++i) {
        printf(" %d", a[i]);
    }
    printf("\n");
}


const int MAXN = 3e5 + 3;

int b[MAXN];


void merge_sort(int *a, int l, int r) {
    if (r - l == 1) {
        return;
    }
    int m = (l + r) / 2, len = r - l;
    merge_sort(a, l, m);
    merge_sort(a, m, r);
    memcpy(b, a + l, sizeof(int) * len);
    int i = 0, j = m - l, k = l;
    while (i < m - l && j < len) {
        if (b[i] <= b[j]) {
            a[k] = b[i];
            ++i;
        } else {
            a[k] = b[j];
            ++j;
        }
        ++k;
    }
    while (i < m - l) {
        a[k] = b[i];
        ++i, ++k;
    }
    while (j < len) {
        a[k] = b[j];
        ++j, ++k;
    }
}


int main() {
    int n;
    scanf("%d", &n);
    int *a = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }
    merge_sort(a, 0, n);
    showArr(a, n);
    free(a);
    return 0;
}
