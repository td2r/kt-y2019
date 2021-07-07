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

int const MAXN = 1'000'000 + 3;
int const MAXT = 29;

ll gcd(ll a, ll b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

ll binmul(ll a, ll b, ll m) {
    ll sgn = 1;
    if (a < 0) {
        sgn = -sgn;
        a = -a;
    }
    if (b < 0) {
        sgn = -sgn;
        b = -b;
    }
    a %= m;
    b %= m;
    ll c = 0;
    while (b) {
        if (b & 1) {
            c = (c + a) % m;
        }
        b >>= 1;
        a = (a << 1) % m;
    }
    return sgn * c;
}

ll binpow(ll a, ll p, ll m) {
    a %= m;
    ll b = 1;
    while (p) {
        if (p & 1) {
            b = binmul(b, a, m);
        }
        a = binmul(a, a, m);
        p >>= 1;
    }
    return b;
}

ll build_long(ll a, ll b) {
    return ((a << 32) | b);
}

ll random_long() {
    return build_long(rand(), rand());
}

ll random(ll l, ll r) {
    return l + (random_long() % (r - l));
}

void sq_decomposition(ll const& n, ll& s, ll& q) {
    s = 0;
    q = n;
    while (!(q & 1)) {
        ++s;
        q >>= 1;
    }
}

bool miller_rabin(ll n) {
    ll a = 2;
    for (ll g; (g = gcd(n, a)) != 1; ++a) {
        if (n > g) {
            return false;
        }
    }

    ll s, q;
    sq_decomposition(n - 1, s, q);

    ll rem = binpow(a, q, n);
    if (rem == 1 || rem == n - 1) {
        return true;
    }

    for (int i = 1; i < s; ++i) {
        rem = binmul(rem, rem, n);
        if (rem == n - 1) {
            return true;
        }
    }

    return false;
}

ll jacobi(ll a, ll b) {
    if (a == 0) return 0;
    if (a == 1) return 1;
    if (a < 0) {
        return (b & 2) == 0 ? jacobi(-a, b) : -jacobi(-a, b);
    }
    ll a1 = a;
    ll e = 0;
    while (!(a1 & 1)) {
        a1 >>= 1;
        ++e;
    }
    ll s = ((e & 1) == 0 || (b & 7) == 1 || (b & 7) == 7) ? 1 : -1;
    if ((b & 3) == 3 && (a1 & 3) == 3) {
        s = -s;
    }
    if (a1 == 1) {
        return s;
    }
    return s * jacobi(b % a1, a1);
}

bool lucas_selfridge(ll n) {
    ll dd = 5;
    for (;; dd = dd < 0 ? -dd + 2 : -dd - 2) {
        ll g = gcd(n, abs(dd));
        if (g != 1) {
            return false;
        }
        if (jacobi(dd, n) == -1) {
            break;
        }
    }

    ll p = 1;
    ll q = (p * p - dd) / 4;

    ll s, d;
    sq_decomposition(n + 1, s, d);

    ll u = 1,
       v = p,
       u2m = 1,
       v2m = p,
       qm = q,
       qm2 = q * 2,
       qkd = q;

    for (ll mask = 2; mask <= d; mask <<= 1) {
        u2m = binmul(u2m, v2m, n);
        v2m = binmul(v2m, v2m, n);
        while (v2m < qm2) {
            v2m += n;
        }
        v2m -= qm2;
        qm = binmul(qm, qm, n);
        qm2 = 2 * qm;
        if (d & mask) {
            ll t1 = binmul(u2m, v, n);
            ll t2 = binmul(v2m, u, n);

            ll t3 = binmul(v2m, v, n);
            ll t4 = binmul(binmul(u2m, u, n), dd, n);

            u = t1 + t2;
            if (u & 1) {
                u += n;
            }
            u = (u >> 1) % n;

            v = t3 + t4;
            if (v & 1) {
                v += n;
            }
            v >>= 1;
            v %= n;
            qkd = binmul(qkd, qm, n);
        }
    }

    if (u == 0 || v == 0) {
        return true;
    }

    ll qkd2 = 2 * qkd;
    for (ll r = 1; r < s; ++r) {
        v = binmul(v, v, n);
        v -= qkd2;
        if (v < 0) v += n;
        if (v < 0) v += n;
        if (v >= n) v -= n;
        if (v >= n) v -= n;
        if (v == 0) {
            return true;
        }
        if (r < s - 1) {
            qkd = binmul(qkd, qkd, n);
            qkd2 = 2 * qkd;
        }
    }

    return false;
}

bool trivial_prime(ll n) {
    if (n == 1) return false;
    for (ll i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

bool bpsw(ll n) {
    if (n <= MAXT * MAXT) {
        return trivial_prime(n);
    }
    for (ll i = 2; i <= MAXT; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    ll s = (ll) sqrt(n);
    return n % 2 == 1
                && s * s != n
                && miller_rabin(n)
                && lucas_selfridge(n);
}

int main() {
    int t;
    scanf("%d", &t);

    while (t--) {
        ll n;
        scanf("%lld", &n);
        printf("%s\n", bpsw(n) ? "YES" : "NO");
    }

    return 0;
}
