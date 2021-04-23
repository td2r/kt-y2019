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



const int MAXN = 1e3 + 1;
const int MAXW = 1e4 + 1;

int n, W;
int w[MAXN];
int c[MAXN];
int dp[MAXN][MAXW];


int main() {
    freopen("knapsack.in", "r", stdin);
    freopen("knapsack.out", "w", stdout);

    memset(dp, 0, sizeof(int) * MAXN * MAXW);

    scanf("%d %d", &n, &W);
    for (int i = 1; i <= n; ++i) scanf("%d", &w[i]);
    for (int i = 1; i <= n; ++i) scanf("%d", &c[i]);

    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= W; ++j) {
            if (j < w[i]) {
                dp[i][j] = dp[i - 1][j];
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - w[i]] + c[i]);
            }
        }
    }

    stack<int> ans;
    for (int i = n, j = W; i > 0 && dp[i][j] >= 0; --i) {
        if (w[i] <= j && dp[i][j] == dp[i - 1][j - w[i]] + c[i]) {
            ans.push(i);
            j -= w[i];
        }
    }
    printf("%zd\n", ans.size());
    while (!ans.empty()) {
        printf("%d", ans.top());
        ans.pop();
        if (!ans.empty()) {
            printf(" ");
        }
    } printf("\n");

    fclose(stdin);
    fclose(stdout);
    return 0;
}
