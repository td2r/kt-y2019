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


struct Layer {
    int r;
    pair<int, int> cmp[8];

    Layer() : r(0) {}

    void add(int w1, int w2) {
        cmp[r++] = mp(w1, w2);
    }
    void print() const {
        printf("%d", r);
        for (int i = 0; i < r; ++i) {
            printf(" %d %d", cmp[i].first, cmp[i].second);
        }
        printf("\n");
    }
};


void merges(int len, int q, int n, Layer *layer, int &k) {
    for (int l = 1, r = 2 * len; r <= q; l += 2 * len, r += 2 * len) {
        for (int i = 0; i < len; ++i) {
            if (r - i <= n) {
                layer[k].add(l + i, r - i);
            }
        }
    }
    ++k;
}

void bitonic_sorts(int len, int q, int n, Layer *layer, int &k) {
    if (len == 1) return;
    for (int l = 1, r = 1 + len / 2; r <= q; l += len, r += len) {
        for (int i = 0; i < len / 2; ++i) {
            if (r + i <= n) {
                layer[k].add(l + i, r + i);
            }
        }
    }
    ++k;
    bitonic_sorts(len / 2, q, n, layer, k);
}


int main() {
    int n, m = 0, k = 0;
    Layer layer[12];
    scanf("%d", &n);
    int q = 1;
    while (q < n) {
        q *= 2;
    }
    for (int len = 1; len != q; len *= 2) {
        merges(len, q, n, layer, k);
        bitonic_sorts(len, q, n, layer, k);
    }
    for (int i = 0; i < k; ++i) {
        m += layer[i].r;
    }
    printf("%d %d %d\n", n, m, k);
    for (int i  = 0; i < k; ++i) {
        layer[i].print();
    }
    return 0;
}
