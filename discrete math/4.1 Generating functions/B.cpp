#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'00 + 3;
ll const mod = 998'244'353;

ll fast_pow(ll a, ll p) {
    ll res = 1LL;
    while (p) {
        if (p & 1) {
            res = (res * a) % mod;
            --p;
        }
        a = (a * a) % mod;
        p >>= 1;
    }
    return res;
}

ll reverse_element(ll a) {
    a = ((a % mod) + mod) % mod;
    return fast_pow(a, mod - 2);
}

void printpf(vector<ll> const& a) {
    for (size_t i = 0; i < a.size(); ++i) {
        printf("%lld ", a[i]);
    } printf("\n");
}

void mulpf_nostretch(vector<ll> &a, vector<ll> const& b) {
    int n = (int) a.size();
    int m = (int) b.size();
    for (int i = n - 1; i >= 0; --i) {
        ll ai = 0;
        for (int j = 0; j <= i && j < m; ++j) {
            ai = (ai + (a[i - j] * b[j]) % mod) % mod;
        }
        a[i] = ai;
    }
}

vector<ll> sqrtpf(vector<ll> const& a, int m) {
    ll twor = reverse_element(2);
    ll k = 1;
    vector<ll> res(m);
    res[0] = 1;
    vector<ll> b(m);
    b[0] = 1;
    for (int i = 1; i < m; ++i) {
        k = (k * (((3 - 2 * i) % mod + mod) % mod)) % mod;
        k = (k * twor) % mod;
        k = (k * reverse_element(i)) % mod;
        mulpf_nostretch(b, a);
        for (int j = 0; j < m; ++j) {
            res[j] = (res[j] + (k * b[j]) % mod) % mod;
        }
    }
    return res;
}

vector<ll> exppf(vector<ll> const& a, int m) {
    ll k = 1;
    vector<ll> res(m);
    res[0] = 1;
    vector<ll> b(m);
    b[0] = 1;
    for (int i = 1; i < m; ++i) {
        k = (k * reverse_element(i)) % mod;
        mulpf_nostretch(b, a);
        for (int j = 0; j < m; ++j) {
            res[j] = (res[j] + (k * b[j]) % mod) % mod;
        }
    }
    return res;
}

vector<ll> lnpf(vector<ll> const& a, int m) {
    ll k = 1;
    vector<ll> res(m);
    vector<ll> b(m);
    b[0] = 1;
    for (int i = 1; i < m; ++i) {
        k = reverse_element(i);
        if (i % 2 == 0)
            k = mod - k;
        mulpf_nostretch(b, a);
        for (int j = 0; j < m; ++j) {
            res[j] = (res[j] + (k * b[j]) % mod) % mod;
        }
    }
    return res;
}


int n;
int m;
vector<ll> p;

int main() {
    scanf("%d %d", &n, &m);
    p.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
        scanf("%lld", &p[i]);
    }

    printpf(sqrtpf(p, m));
    printpf(exppf(p, m));
    printpf(lnpf(p, m));

    return 0;
}
