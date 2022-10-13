#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int vector_product(int x1, int y1, int x2, int y2) {
    return x1 * y2 - x2 * y1;
}

int rotate_square(int x, int y, int x1, int y1, int x2, int y2) {
    return vector_product(x1 - x, y1 - y, x2 - x, y2 - y);
}

bool in_segment(int x, int y, int x1, int y1, int x2, int y2) {
    return rotate_square(x, y, x1, y1, x2, y2) == 0 &&
        min(x1, x2) <= x && x <= max(x1, x2) &&
        min(y1, y2) <= y && y <= max(y1, y2);
}

int const MAXN = 100000 + 3;

int n, px, py;
int parity = 0;
int x[MAXN];
int y[MAXN];

int main() {
    scanf("%d %d %d", &n, &px, &py);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &x[i], &y[i]);
    }
    x[n] = x[0];
    y[n] = y[0];
    for (int i = 1; i <= n; ++i) {
        int x1 = x[i - 1];
        int y1 = y[i - 1];
        int x2 = x[i];
        int y2 = y[i];
        if (in_segment(px, py, x1, y1, x2, y2)) {
            printf("YES\n");
            return 0;
        }
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        if (y1 < py && py <= y2 && rotate_square(px, py, x1, y1, x2, y2) > 0) {
            parity ^= 1;
        }
    }
    printf("%s\n", parity ? "YES" : "NO");
    return 0;
}
