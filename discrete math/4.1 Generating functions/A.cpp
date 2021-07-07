#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000 + 3;
ll const mod = 998'244'353;

int n;
int m;
ll p[2 * MAXN];
ll q[2 * MAXN];

void print(vector<ll> &res) {
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }
    printf("%zd\n", res.size() - 1);
    for (size_t i = 0; i < res.size(); ++i) {
        printf("%lld ", res[i]);
    } printf("\n");
}

void sum() {
    vector<ll> res(max(n, m) + 1);
    for (size_t i = 0; i < res.size(); ++i) {
        res[i] = (p[i] + q[i]) % mod;
    }
    print(res);
}

void mul() {
    vector<ll> res(n + m + 1);
    for (int k = 0; k < n + m + 1; ++k) {
        res[k] = 0;
        for (int i = 0; i <= k; ++i) {
            res[k] += (p[i] * q[k - i]) % mod;
            res[k] %= mod;
        }
    }
    print(res);
}

void div() {
    vector<ll> res(1000);
    for (int i = 0; i < 1000; ++i) {
        res[i] = p[i];
        ll s = 0;
        for (int k = 0; k < i; ++k) {
            s = (s + res[k] * q[i - k]) % mod;
        }
        res[i] = (p[i] - s + mod) % mod;
        printf("%lld ", res[i]);
    } printf("\n");
}

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i <= n; ++i) {
        scanf("%lld", &p[i]);
    }
    for (int i = 0; i <= m; ++i) {
        scanf("%lld", &q[i]);
    }
    sum();
    mul();
    div();
    return 0;
}
