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




int main() {
    const int MAXN = 1e4 + 2;

    char p[MAXN];
    char s[MAXN];
    bool dp[2][MAXN];
    memset(dp, 0, sizeof(bool) * 2 * MAXN);

    scanf("%s\n%s", p + 1, s + 1);
    int n = (int)strlen(p + 1);
    int m = (int)strlen(s + 1);

    dp[0][0] = true;
    for (int i = 1; i <= n; ++i) {
        dp[1][0] = dp[0][0] && p[i] == '*';
        for (int j = 1; j <= m; ++j) {
            if (p[i] == '?') {
                dp[1][j] = dp[0][j - 1];
            } else if (p[i] == '*') {
                dp[1][j] = dp[0][j] || dp[1][j - 1];
            } else {
                dp[1][j] = dp[0][j - 1] && p[i] == s[j];
            }
        }
        swap(dp[0], dp[1]);
    }

    if (dp[0][m]) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }

    return 0;
}
