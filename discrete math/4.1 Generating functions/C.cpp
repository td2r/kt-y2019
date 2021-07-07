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

int k;
vector<ll> a;
vector<ll> p;
vector<ll> q;

int main() {
    scanf("%d", &k);
    a.resize(k);
    q.resize(k + 1);
    for (int i = 0; i < k; ++i) {
        scanf("%lld", &a[i]);
    }
    q[0] = 1;
    for (int i = 1; i <= k; ++i) {
        scanf("%lld", &q[i]);
        q[i] = -q[i];
    }

    int z = 0;
    for (int i = 0; z < k; ++i) {
        if (i >= k) {
            a.emplace_back(0);
            for (int j = 1; j <= k; ++j) {
                a.back() += (-q[j]) * a[i - j];
            }
        }
        p.emplace_back(a[i]);
        for (int j = 1; j <= min(k, i); ++j) {
            p.back() -= a[i - j] * (-q[j]);
        }
        if (p.back() != 0) {
            z = 0;
        } else {
            z++;
        }
    }

    while (p.back() == 0) {
        p.pop_back();
    }
    
    printpf(p);
    printpf(q);

    return 0;
}
