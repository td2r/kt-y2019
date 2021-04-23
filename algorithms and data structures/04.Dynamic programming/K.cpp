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
    os << endl;
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




int main() {
    // freopen(".in", "r", stdin);
    // freopen(".out", "w", stdout);

    const int MAXN = 2e3 + 1;
    const int mod = 1e9;

    int n;
    int a[MAXN];
    int dp[MAXN][MAXN];
    memset(dp, 0, sizeof(int) * MAXN * MAXN);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        dp[i][i] = 1;
        for (int j = i - 1; j >= 0; --j) {
            dp[j][i] = (dp[j + 1][i] + dp[j][i - 1]) % mod;
            if (a[j] == a[i]) {
                dp[j][i]++;
                dp[j][i] %= mod;
            } else {
                dp[j][i] = (dp[j][i] - dp[j + 1][i - 1]) % mod;
            }
        }
    }

    printf("%d\n", (dp[0][n - 1] + mod) % mod);

    // fclose(stdin);
    // fclose(stdout);
    return 0;
}
