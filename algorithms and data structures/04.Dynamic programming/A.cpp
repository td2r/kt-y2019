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




const int MAXN = 400 + 1;

ll dp[MAXN][MAXN];
int cp[MAXN][MAXN];


void print(int i, int j) {
    if (i == j || i + 1 == j) {
        printf("A");
        return;
    }
    printf("(");
    print(i, cp[i][j]);
    print(cp[i][j], j);
    printf(")");
}


int main() {
    freopen("matrix.in", "r", stdin);
    freopen("matrix.out", "w", stdout);

    int n;
    ll a[MAXN];
    ll b[MAXN];

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%lld %lld", &a[i], &b[i]);

    // for (int l = 0; l < 2; ++l) {
    //     for (int i = 0, j = l; j <= n; ++i, ++j) {
    //         dp[i][j] = 0;
    //         cp[i][j] = j;
    //     }
    // }
    for (int l = 2; l <= n; ++l) {
        for (int i = 0, j = l; j <= n; ++i, ++j) {
            dp[i][j] = dp[i][j - 1] + a[i] * a[j - 1] * b[j - 1];
            cp[i][j] = j - 1;
            for (int c = i + 1; c < j; ++c) {
                ll cc = dp[i][c] + dp[c][j] + a[i] * a[c] * b[j - 1];
                if (cc < dp[i][j]) {
                    dp[i][j] = cc;
                    cp[i][j] = c;
                }
            }
        }
    }

    print(0, n);
    printf("\n");

    fclose(stdin);
    fclose(stdout);
    return 0;
}
