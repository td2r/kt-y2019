#include <bits/stdc++.h>
#define mp make_pair
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
using namespace std;

template<typename T1, typename T2>
ostream& operator << (ostream &os, pair<T1, T2> &p) {
    os << '<' << p.first << ' ' << p.second << '>';
    return os;
}

template<typename T>
ostream& operator << (ostream &os, vector<T> &v) {
    for (int i = 0; i < v.size(); ++i) {
        if (i) os << ' ';
        os << v[i];
    }
    return os;
}

template<typename T>
void showArray(const string arrName, T* beginIterator, T* endIterator) {
    cout << arrName << " = [";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}




const ll mod = 999999937LL;

vector<vector<ll>> operator * (const vector<vector<ll>> &a,
                               const vector<vector<ll>> &b) {
    int n = a.size();
    int m = b[0].size();
    int k = b.size();
    vector<vector<ll>> c(n, vector<ll>(m, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int t = 0; t < k; ++t) {
                c[i][j] = (c[i][j] + a[i][t] * b[t][j]) % mod;
            }
        }
    }
    return c;
}

vector<vector<ll>> binpow(vector<vector<ll>> a, ll p) {
    assert(a.size() == a[0].size());
    int n = a.size();
    vector<vector<ll>> res(n, vector<ll>(n, 0));
    for (int i = 0; i < n; ++i) {
        res[i][i] = 1;
    }
    while (p) {
        if (p & 1) {
            res = res * a;
        }
        a = a * a;
        p >>= 1;
    }
    return res;
}

int main() {
    ll n;

    while (scanf("%lld", &n) != -1 && n) {
        vector<vector<ll>> a = binpow({
            {1LL, 1LL, 1LL, 1LL, 1LL},
            {1LL, 1LL, 1LL, 1LL, 1LL},
            {1LL, 1LL, 1LL, 1LL, 1LL},
            {1LL, 1LL, 0LL, 1LL, 0LL},
            {1LL, 1LL, 0LL, 1LL, 0LL}},
            n - 1);
        a = a * vector<vector<ll>>({{1LL}, {1LL}, {1LL}, {1LL}, {1LL}});
        ll ans = 0;
        for (int i = 0; i < 5; ++i) {
            ans = (ans + a[i][0]) % mod;
        }
        printf("%lld\n", ans);
    }

    return 0;
}
