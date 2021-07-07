#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000 + 3;
ll const mod = 998'244'353;

void printpf(vector<ll> const& a) {
    printf("%zd\n", a.size() - 1);
    for (size_t i = 0; i < a.size(); ++i) {
        printf("%lld ", a[i]);
    } printf("\n");
}

void mulpf(vector<ll> &a, vector<ll> const& b) {
    int n = (int) a.size();
    int m = (int) b.size();
    a.resize(n + m - 1);
    for (int i = n + m - 2; i >= 0; --i) {
        ll ai = 0;
        for (int j = 0; j <= i && j < m; ++j) {
            ai = (ai + (a[i - j] * b[j]) % mod) % mod;
        }
        a[i] = ai;
    }
}

int r, k;
vector<ll> p;

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

void print_ratio(ll num, ll denom) {
    if (denom < 0) {
        num = -num;
        denom = -denom;
    }
    if (num < 0) {
        printf("-");
        num = -num;
    }
    ll g = gcd(num, denom);
    printf("%lld/%lld", num / g, denom / g);
}

int main() {
    scanf("%d %d", &r, &k);
    p.resize(k + 1);
    for (int i = 0; i <= k; ++i) {
        scanf("%lld", &p[i]);
    }

    ll denominator = 1;
    for (int i = 1; i <= k; ++i) {
        denominator *= r;
        denominator *= i;
    }

    vector<ll> ans(k + 1);

    for (int i = 0; i <= k; ++i) {
        vector<ll> a({1});
        for (int j = 1; j <= k; ++j) {
            mulpf(a, vector<ll>({j - i, 1}));
        }
        ll c = 1;
        for (int j = 1; j <= k - i; ++j) {
            c *= r;
        }
        for (int j = 0; j <= k; ++j) {
            ans[j] += c * p[i] * a[j];
        }
    }

    for (int i = 0; i <= k; ++i) {
        print_ratio(ans[i], denominator);
        printf(" ");
    }

    return 0;
}
