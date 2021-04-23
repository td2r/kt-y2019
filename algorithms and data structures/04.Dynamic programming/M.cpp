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




#define INF INT_MAX

const int MAXN = 18;
const int MAXM = (1 << MAXN);

int main() {
    freopen("salesman.in", "r", stdin);
    freopen("salesman.out", "w", stdout);

    int n;
    int m;
    int e[MAXN][MAXN];
    int dp[MAXN][MAXM];
    for (int i = 0; i < MAXN; ++i) {
        fill(e[i], e[i] + MAXN, INF);
        fill(dp[i], dp[i] + MAXM, INF);
        dp[i][1 << i] = 0;
    }

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        e[u - 1][v - 1] = e[v - 1][u - 1] = w;
    }

    for (int mask = 0; mask < (1 << n); ++mask) {
        bitset<18> bit(mask);
        int cnt1 = 0, cnt2 = 0;
        int visited[MAXN], not_visited[MAXN];
        for (int i = 0; i < n; ++i) {
            if (bit[i]) {
                visited[cnt1++] = i;
            } else {
                not_visited[cnt2++] = i;
            }
        }

        for (int i = 0; i < cnt1; ++i) {
            int u = visited[i];
            if (dp[u][mask] != INF) {
                for (int j = 0; j < cnt2; ++j) {
                    int v = not_visited[j];
                    if (e[u][v] != INF) {
                        int nm = (mask | (1 << v));
                        dp[v][nm] = min(dp[v][nm], dp[u][mask] + e[u][v]);
                    }
                }
            }
        }
    }

    int ans = INF;
    for (int i = 0; i < n; ++i) {
        ans = min(ans, dp[i][(1 << n) - 1]);
    }
    if (ans == INF) {
        printf("-1\n");
    } else {
        printf("%d\n", ans);
    }

    fclose(stdin);
    fclose(stdout);
    return 0;
}
