#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXW = 5'500 + 1;
int const N = 7;

int pos;
char s[300];
ll C[MAXW][MAXW];
ll dp[N][N];
ll factorial[11];

ll binomial(ll n, ll k) {
    // concider, that k <= 6
    ll res = 1;
    for (ll i = 0; i < k; ++i) {
        res *= n - i;
    }
    return res / factorial[k];
}

vector<ll> parse() {
    if (s[pos] == 'B') {
        pos++;
        return vector<ll>({ 0, 1, 0, 0, 0, 0, 0 });
    } else if (s[pos] == 'L') {
        pos++;
        assert(s[pos++] == '(');
        vector<ll> a = parse();
        assert(s[pos++] == ')');
        vector<ll> res(N);
        res[0] = 1;
        for (int i = 1; i < N; ++i) {
            for (int j = 1; j <= i; ++j) {
                res[i] += a[j] * res[i - j];
            }
        }
        return res;
    } else if (s[pos] == 'S') {
        pos++;
        assert(s[pos++] == '(');
        vector<ll> a = parse();
        assert(s[pos++] == ')');
        for (int n = 1; n < N; ++n) {
            for (int k = 1; k < N; ++k) {
                dp[n][k] = dp[n][k - 1];
                for (int i = 1; i <= n / k; ++i) {
                    dp[n][k] += binomial(a[k] + i - 1, i) * dp[n - i * k][k - 1];
                }
            }
        }
        vector<ll> res(N);
        for (int i = 0; i < N; ++i) {
            res[i] = dp[i][i];
        }
        return res;
    } else if (s[pos] == 'P') {
        pos++;
        assert(s[pos++] == '(');
        vector<ll> a = parse();
        assert(s[pos++] == ',');
        vector<ll> b = parse();
        assert(s[pos++] == ')');
        vector<ll> res(N);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j <= i; ++j) {
                res[i] += a[j] * b[i - j];
            }
        }
        return res;
    } else {
        return vector<ll>({ 228, 228, 228, 228, 228, 228, 228 });
    }
}

int main() {
    factorial[0] = 1;
    for (int i = 1; i < 11; ++i) {
        factorial[i] = i * factorial[i - 1];
    }

    for (int n = 0; n < MAXW; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
        }
    }

    for (int i = 0; i < N; ++i) {
        dp[0][i] = 1;
    }

    scanf("%s", s);

    vector<ll> res = parse();

    for (int i = 0; i < N; ++i) {
        printf("%lld ", res[i]);
    } printf("\n");

    return 0;
}
