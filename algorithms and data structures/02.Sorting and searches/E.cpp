#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;


const int ALPHA = 27;


int main() {
    int n, m, k;
    char **c, **t;
    int cnt[ALPHA], ind[ALPHA];
    scanf("%d %d %d\n", &n, &m, &k);
    c = (char**)malloc(sizeof(char*) * n);
    t = (char**)malloc(sizeof(char*) * n);
    for (int i = 0; i < n; ++i) {
        c[i] = (char*)malloc(sizeof(char) * m);
        if (i) scanf("\n");
        scanf("%s", c[i]);
    }
    for (int q = 1; q <= k; ++q) {
        memset(cnt, 0, sizeof(int) * ALPHA);
        for (int i = 0; i < n; ++i) {
            ++cnt[c[i][m - q] - 'a'];
        }
        ind[0] = 0;
        for (int i = 1; i < ALPHA; ++i) {
            ind[i] = ind[i - 1] + cnt[i - 1];
        }
        for (int i = 0; i < n; ++i) {
            t[ind[c[i][m - q] - 'a']++] = c[i];
        }
        swap(c, t);
    }
    for (int i = 0; i < n; ++i) {
        printf("%s\n", c[i]);
    }
    for (int i = 0; i < n; ++i) free(c[i]);
    free(c);
    free(t);
    return 0;
}
