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




const int MAXN = 150 + 1;

int x, a, y, b, l;
int dp[MAXN][MAXN];


bool correct_wide(int W) {
    int n = 0;
    int dx[2 * MAXN];
    int dy[2 * MAXN];
    for (int i = (W + a - 1) / a, j = 0; i >= 0;) {
        dx[n] = i;
        dy[n++] = j;
        --i;
        j = (W - i * a + b - 1) / b;
    }
    int maxl = 0;
    memset(dp, 0, sizeof(int) * MAXN * MAXN);
    for (int i = x; i >= 0; --i) {
        for (int j = y; j >= 0; --j) {
            for (int k = 0; k < n; ++k) {
                int i1 = i - dx[k];
                int j1 = j - dy[k];
                if (i1 >= 0 && j1 >= 0) {
                    dp[i1][j1] = max(dp[i1][j1], dp[i][j] + 1);
                }
            }
            maxl = max(maxl, dp[i][j]);
        }
    }
    return maxl >= l;
}


int main() {
    freopen("bridge.in", "r", stdin);
    freopen("bridge.out", "w", stdout);

    scanf("%d %d %d %d %d", &x, &a, &y, &b, &l);

    int i = min(a, b);
    int j = (x * a + y * b) / l + 1;
    int w = (i + j) / 2;
    while (w != i) {
        if (correct_wide(w)) {
            i = w;
        } else {
            j = w;
        }
        w = (i + j) / 2;
    }

    printf("%d\n", w);

    fclose(stdin);
    fclose(stdout);
    return 0;
}
