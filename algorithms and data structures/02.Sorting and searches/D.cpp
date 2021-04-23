#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;


const int MAXN = 1e3 + 3;
const double eps = 1e-9;

bool eq(double a, double b) {
    return fabs(a - b) < eps;
}
bool lseq(double a, double b) {
    return a < b || eq(a, b);
}

double k, x[MAXN], y[MAXN], z[MAXN];


void f(double *h, int n) {
    x[0] = 0, y[0] = 1, z[0] = 0;
    x[n - 1] = 0, y[n - 1] = 1, z[n - 1] = 0;
    for (int i = 1; i < n - 1; ++i) x[i] = 1, y[i] = -2, z[i] = 1, h[i] = 2;
    for (int i = 1; i < n - 1; ++i) {
        k = x[i] / y[i - 1];
        // x[i] -= k * y[i - 1];
        y[i] -= k * z[i - 1];
        h[i] -= k * h[i - 1];
    }
    for (int i = n - 2; i > 0; --i) {
        k = z[i] / y[i + 1];
        // z[i] -= k * y[i + 1];
        h[i] -= k * h[i + 1];
        h[i] /= y[i];
        y[i] = 1;
    }
}


int main() {
    int n;
    scanf("%d", &n);
    double *h = (double*)malloc(sizeof(double) * n);
    scanf("%lf", &h[0]);
    double l = 0., r = 1e6;
    h[n - 1] = r / 2.;
    for (int i = 0; i < 27; ++i) {
        f(h, n);
        if (lseq(*min_element(h, h + n), 0)) {
            l = h[n - 1];
        } else {
            r = h[n - 1];
        }
        h[n - 1] = (l + r) / 2.;
    }
    printf("%.2lf\n", h[n - 1]);
    free(h);
    return 0;
}
