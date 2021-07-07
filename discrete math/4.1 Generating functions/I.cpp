#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000 + 3;
ll const mod = 104'857'601;

void printpf(vector<ll> const& a) {
    printf("%zd\n", a.size() - 1);
    for (size_t i = 0; i < a.size(); ++i) {
        printf("%lld ", a[i]);
    } printf("\n");
}

void filter_parity(vector<ll> &a, ll parity) {
    int k = ((int) a.size() + 1) / 2;
    for (size_t i = 0; i < a.size(); ++i) {
        if (i % 2 == parity) {
            swap(a[i], a[i / 2]);
        }
        if (i >= k) {
            a[i] = 0;
        }
    }
}

ll k, n;
vector<ll> a;
vector<ll> q({1});

void sqrpf(vector<ll> &p) {
    for (int i = 2 * k; i >= 0; --i) {
        if (i % 2) {
            p[i] = 0;
            continue;
        }
        ll pi = 0;
        for (int j = 0; j <= i && j <= k; ++j) {
            pi = (pi + (p[i - j] * (j % 2 ? -p[j] : p[j])) % mod) % mod;
        }
        p[i] = pi;
    }
}

ll solve() {
    n--;
    while (n >= k) {
        for (int i = k; i < 2 * k; ++i) {
            a[i] = 0;
            for (int j = 1; j <= k; ++j) {
                a[i] = (a[i] + (((-q[j]) * a[i - j]) % mod)) % mod;
            }
        }
        sqrpf(q);
        filter_parity(a, n % 2);
        filter_parity(q, 0);
        n /= 2;
    }
    return a[n];
}

int main() {
    scanf("%lld %lld", &k, &n);
    a.resize(2 * k);
    q.resize(2 * k + 1);
    for (int i = 0; i < k; ++i) {
        scanf("%lld", &a[i]);
    }
    for (int i = 1; i <= k; ++i) {
        scanf("%lld", &q[i]);
        q[i] = -q[i];
    }

    printf("%lld\n", (solve() + mod) % mod);

    return 0;
}
