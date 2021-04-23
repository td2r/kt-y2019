#include <bits/stdc++.h>
#define mp make_pair
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
using namespace std;

const int MAXN = 3e5 + 1;

int p[MAXN], rnk[MAXN], sz[MAXN], mine[MAXN], maxe[MAXN];

int find(int x) {
    if (x == p[x]) {
        return x;
    }
    return p[x] = find(p[x]);
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        if (rnk[a] < rnk[b]) {
            swap(a, b);
        }
        p[b] = a;
        if (rnk[a] == rnk[b]) {
            ++rnk[a];
        }
        sz[a] += sz[b];
        mine[a] = min(mine[a], mine[b]);
        maxe[a] = max(maxe[a], maxe[b]);
    }
}

int main() {
    int n;

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        p[i] = i;
        sz[i] = 1;
        mine[i] = i;
        maxe[i] = i;
    }

    char buff[16];
    while (scanf("\n%s", buff) != -1) {
        if (buff[0] == 'u') {
            int a, b;
            scanf(" %d %d", &a, &b);
            --a, --b;
            unite(a, b);
        } else {
            int x;
            scanf(" %d", &x);
            --x;
            x = find(x);
            printf("%d %d %d\n", mine[x] + 1, maxe[x] + 1, sz[x]);
        }
    }
    return 0;
}
