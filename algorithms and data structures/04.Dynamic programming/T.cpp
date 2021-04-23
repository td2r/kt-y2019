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




const int MAXN = 3e5 + 1;

int n;
ll c[MAXN];
ll sl[MAXN];
ll sz[MAXN];
vector<pair<int, int>> g[MAXN];


void dfs1(int v, int p = -1) {
    sz[v] = 1;
    for (pair<int, int> &e : g[v]) {
        int to = e.first;
        if (to != p) {
            dfs1(to, v);
            sz[v] += sz[to];
            sl[v] += sl[to] + sz[to];
        }
    }
}

void dfs2(int v, int p = -1, ll upsl = 0) {
    for (pair<int, int> &e : g[v]) {
        int to = e.first, i = e.second;
        if (to != p) {
            c[i] = sz[to] * (upsl + sl[v] - (sl[to] + sz[to])) +
                   (n - sz[to]) * sl[to] + sz[to] * (n - sz[to]);
            dfs2(to, v, upsl + sl[v] - (sl[to] + sz[to]) + n - sz[to]);
        }
    }
}


int main() {
    freopen("treedp.in", "r", stdin);
    freopen("treedp.out", "w", stdout);

    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        g[u - 1].emplace_back(mp(v - 1, i));
        g[v - 1].emplace_back(mp(u - 1, i));
    }

    dfs1(0);
    dfs2(0);

    for (int i = 0; i < n - 1; ++i) {
        printf("%lld\n", c[i]);
    }

    fclose(stdin);
    fclose(stdout);
    return 0;
}
