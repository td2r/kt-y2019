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




const int MAXN = 5e3 + 1;

int n, m, dp[MAXN][MAXN];
char s[MAXN], t[MAXN];


int main() {
    freopen("levenshtein.in", "r", stdin);
    freopen("levenshtein.out", "w", stdout);
    scanf("%s\n%s", s, t);
    n = strlen(s);
    m = strlen(t);
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0) {
                if (j == 0) {
                    dp[i][j] = 0;
                } else {
                    dp[i][j] = j;
                }
            } else {
                if (j == 0) {
                    dp[i][j] = i;
                } else if (s[i - 1] == t[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    dp[i][j] = min({dp[i][j - 1] + 1, dp[i - 1][j] + 1,
                            dp[i - 1][j - 1] + 1});
                }
            }
        }
    }
    printf("%d\n", dp[n][m]);
    fclose(stdin);
    fclose(stdout);
    return 0;
}
