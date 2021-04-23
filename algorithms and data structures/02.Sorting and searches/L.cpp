#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
#define mp make_pair
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


const double eps = 1e-9;
bool f(int n, int k, int *v, int *w, double c, bool print = false) {
    // bool debug = (2.1983 < c && c < 2.1985);
    int *i = (int*)malloc(sizeof(int) * n);
    // if (debug) printf("point1\n");
    for (int j = 0; j < n; ++j)
        i[j] = j;
    // if (debug) printf("point2\n");
    sort(i, i + n, [v, w, c] (int i1, int i2) {
        return v[i1] - c * w[i1] > v[i2] - c * w[i2];
    });
    // if (debug) printf("point3\n");
    if (print) {
        for (int j = 0; j < k; ++j) {
            if (j) printf(" ");
            printf("%d", i[j] + 1);
        }
        printf("\n");
    }
    double s = 0.;
    for (int j = 0; j < k; ++j) {
        s += v[i[j]] - c * w[i[j]];
    }
    // if (debug) printf("point4\n");
    free(i);
    // if (debug) printf("point5\n");
    return s > 0 || fabs(s) < eps;
}


int main() {
    int n, k, *v, *w;
    scanf("%d %d", &n, &k);
    v = (int*)malloc(sizeof(int) * n);
    w = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &v[i], &w[i]);
    }
    double l = 0, r = 1e6 + 1., c = 1e3 + 0.5;
    for (int i = 0; i < 35; ++i) {
        // printf("%d c = %.4lf\n", i + 1, c);
        if (f(n, k, v, w, c)) {
            l = c;
        } else {
            r = c;
        }
        c = (l + r) / 2.;
    }
    f(n, k, v, w, r, true);
    free(v);
    free(w);
    return 0;
}
