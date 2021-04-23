#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;


int main() {
    int n;
    int *a;
    scanf("%d", &n);
    a = (int*)malloc(sizeof(int) * (n + 1));
    for (int i = 1; i <= n; ++i) {
        a[i] = i;
    }
    for (int i = n - 1; i > 0; --i) {
        swap(a[i], a[(i + n) / 2]);
    }
    for (int i = 1; i <= n; ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
    free(a);
    return 0;
}
