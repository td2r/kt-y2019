#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 2'000 + 3;
ll const mod = 1'000'000'007;

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
            // ai = (ai + (a[i - j] * b[j]) % mod) % mod;
            ai += a[i - j] * b[j];
        }
        a[i] = ai;
    }
}

ll substpf(vector<ll> const& a, ll t) {
    ll res = 0;
    ll t1 = 1;
    for (size_t i = 0; i < a.size(); ++i) {
        res += a[i] * t1;
        t1 *= t;
    }
    return res;
}

int r, d;
vector<ll> a;
vector<ll> p;
vector<ll> P;
vector<ll> Q({1});

void get_a() {
    a.resize(d + 1);
    ll r1 = 1;
    for (int i = 0; i <= d; ++i) {
        a[i] = substpf(p, i) * r1;
        r1 *= r;
    }
}

void get_p() {
    P.resize(d + 1);
    for (int i = 0; i <= d; ++i) {
        for (int j = 0; j <= i; ++j) {
            P[i] += a[j] * Q[i - j];
        }
    }
}

void get_q() {
    vector<ll> Q1({ 1, -r });
    for (int i = 0; i <= d; ++i) {
        mulpf(Q, Q1);
    }
}

int main() {
    scanf("%d\n%d", &r, &d);
    p.resize(d + 1);
    for (int i = 0; i <= d; ++i) {
        scanf("%lld", &p[i]);
    }

    get_q();
    get_a();
    get_p();

    while (P.size() > 1 && P.back() == 0) {
        P.pop_back();
    }

    printpf(P);
    printpf(Q);

    return 0;
}
