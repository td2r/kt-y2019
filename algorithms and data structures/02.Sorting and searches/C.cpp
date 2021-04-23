#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
using namespace std;


void showArr(int *a, int l, int r) {
    for (int i = l; i < r; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
}


int kth(int *a, int l, int r, int k) {
    if (r - l < 5) {
        for (int i = l; i < r - 1; ++i) {
            for (int j = r - 1; j > i; --j) {
                if (a[j - 1] > a[j]) swap(a[j - 1], a[j]);
            }
        }
        return a[l + k - 1];
    }
    int n = r - l;
    int *b = (int*)malloc(sizeof(int) * (n / 5));
    for (int i = l, j = 0; i + 5 <= r; i += 5, ++j) {
        // a[i]
        if (a[i + 3] > a[i + 4]) swap(a[i + 3], a[i + 4]);
        if (a[i + 2] > a[i + 3]) swap(a[i + 2], a[i + 3]);
        if (a[i + 1] > a[i + 2]) swap(a[i + 1], a[i + 2]);
        if (a[i] > a[i + 1]) swap(a[i], a[i + 1]);
        // a[i + 1]
        if (a[i + 3] > a[i + 4]) swap(a[i + 3], a[i + 4]);
        if (a[i + 2] > a[i + 3]) swap(a[i + 2], a[i + 3]);
        if (a[i + 1] > a[i + 2]) swap(a[i + 1], a[i + 2]);
        // a[i + 2]
        if (a[i + 3] > a[i + 4]) swap(a[i + 3], a[i + 4]);
        if (a[i + 2] > a[i + 3]) swap(a[i + 2], a[i + 3]);
        b[j] = a[i + 2];
    }
    int x = kth(b, 0, n / 5, (n / 5 + 1) / 2);
    free(b);
    int i = l, j = r - 1;
    while (i < j) {
        while (a[i] < x) {
            ++i;
        }
        while (a[j] > x) {
            --j;
        }
        if (i < j) {
            swap(a[i], a[j]);
            ++i, --j;
        }
    }
    if (a[i] < x) ++i;
    if (k <= i - l) {
        return kth(a, l, i, k);
    } else {
        return kth(a, i, r, k - i + l);
    }
}


int main() {
    int *a;
    int n, k, A, B, C;
    scanf("%d %d", &n, &k);
    a = (int*)malloc(sizeof(int) * n);
    scanf("%d %d %d %d %d", &A, &B, &C, &a[0], &a[1]);
    for (int i = 2; i < n; ++i) {
        a[i] = A * a[i - 2] + B * a[i - 1] + C;
    }
    printf("%d\n", kth(a, 0, n, k));
    free(a);
    return 0;
}
