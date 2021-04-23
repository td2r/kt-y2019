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




const int MAXN = 1e5 + 1;

vector<pair<int, int>> g[MAXN];
ll dp[2][MAXN];


void dfs(int v, int p = -1) {
    dp[0][v] = dp[1][v] = 0LL;
    for (pair<int, int> &e : g[v]) {
        int to = e.first;
        if (to != p) {
            dfs(to, v);
            dp[0][v] += max(dp[0][to], dp[1][to]);
        }
    }
    for (pair<int, int> &e : g[v]) {
        int to = e.first, w = e.second;
        if (to != p) {
            dp[1][v] = max(dp[1][v],
                    dp[0][v] - max(dp[0][to], dp[1][to]) + dp[0][to] + w);
        }
    }
}


int main() {
    freopen("matching.in", "r", stdin);
    freopen("matching.out", "w", stdout);

    int n;

    scanf("%d", &n);

    for (int i = 0; i < n - 1; ++i) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        g[u - 1].emplace_back(mp(v - 1, w));
        g[v - 1].emplace_back(mp(u - 1, w));
    }

    dfs(0);

    printf("%lld\n", max(dp[0][0], dp[1][0]));

    fclose(stdin);
    fclose(stdout);
    return 0;
}
