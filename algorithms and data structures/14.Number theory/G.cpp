#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000'000 + 3;

template <class forwardIterator>
void print_array(string const& name, forwardIterator first, forwardIterator last) {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

ll modMul(ll a, ll b, ll m) {
    a %= m;
    b %= m;
    ll c = 0;
    while (b != 0) {
        if (b & 1) {
            c = (c + a) % m;
        }
        b >>= 1;
        a = (a << 1) % m;
    }
    return c;
}

ll modPow(ll a, ll p, ll m) {
    a %= m;
    ll b = 1;
    while (p) {
        if (p & 1) {
            b = modMul(b, a, m);
        }
        a = modMul(a, a, m);
        p >>= 1;
    }
    return b;
}

ll a, b, n, m, p = 0;
multimap<ll, ll> log_of;
ll pow_of[MAXN];

bool is_answer(ll i, ll j) {
    return i * m - j + p < n && modMul(pow_of[i - 1], modPow(a, m - j + p, n), n) == b;
}

int main() {
    scanf("%lld %lld %lld", &a, &b, &n);
    a %= n;
    b %= n;
    if (b == 1) {
        printf("0\n");
        return 0;
    }
    m = (ll) sqrt(n) + 1;

    ll r = 1;
    ll g = gcd(a, n);
    while (g != 1) {
        if (b % g) {
            printf("-1\n");
            return 0;
        }
        b /= g;
        n /= g;
        ++p;
        r = modMul(r, a / g, n);
        if (b == r) {
            printf("%lld\n", p);
            return 0;
        }
        g = gcd(a, n);
    }

    pow_of[0] = 1;
    ll a_pow_m = modMul(modPow(a, m, n), r, n);
    for (ll i = 1, x = a_pow_m; i <= m; ++i, x = modMul(x, a_pow_m, n)) {
        pow_of[i] = x;
        log_of.emplace(x, i);
    }

    for (ll j = 0, x = b; j < m; ++j, x = modMul(x, a, n)) {
        auto range = log_of.equal_range(x);
        while (range.first != range.second) {
            ll i = range.first->second;
            if (is_answer(i, j)) {
                printf("%lld\n", i * m - j + p);
                return 0;
            }
            ++range.first;
        }
    }

    printf("-1\n");
    return 0;
}
