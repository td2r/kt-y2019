#include <bits/stdc++.h>
#define mp make_pair
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
using namespace std;


int main() {
    const int MAXN = 1e5;

    int n, a[MAXN];

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
    }

    for (int i = 0; i < n; ++i) {
        if (2 * i + 1 < n && a[i] > a[2 * i + 1]) {
            printf("NO\n");
            return 0;
        } else if (2 * i + 2 < n && a[i] > a[2 * i + 2]) {
            printf("NO\n");
            return 0;
        }
    }
    printf("YES\n");
    return 0;
}
