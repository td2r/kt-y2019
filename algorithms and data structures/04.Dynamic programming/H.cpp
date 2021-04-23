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




int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}


const int MAXN = 16;
const int MAXM = (1 << MAXN);

int n, m, k;
int a[MAXN];
int g[MAXN][MAXN];
ll dp[MAXN][MAXM];


int main() {
    freopen("perm.in", "r", stdin);
    freopen("perm.out", "w", stdout);

    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        dp[i][1 << i] = 1LL;
    }
    sort(a, a + n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            g[i][j] = g[j][i] = gcd(a[i], a[j]);
        }
    }

    for (int mask = 0; mask < (1 << n); ++mask) {
        bitset<MAXN> bit(mask);

        int cnt1 = 0, cnt2 = 0;
        int used[MAXN], not_used[MAXN];
        for (int i = 0; i < n; ++i) {
            if (bit[i]) {
                used[cnt1++] = i;
            } else {
                not_used[cnt2++] = i;
            }
        }

        for (int i = 0; i < cnt1; ++i) {
            int x = used[i];
            for (int j = 0; j < cnt2; ++j) {
                int y = not_used[j];
                if (g[x][y] >= k) {
                    int nm = (mask | (1 << y));
                    dp[y][nm] += dp[x][mask];
                }
            }
        }
    }

    vector<int> ans;
    bitset<MAXN> used(0);
    bitset<MAXN> not_used((1 << n) - 1);
    for (int i = 0, pi = -1; i < n; ++i) {
        int xmask = (int)not_used.to_ulong();
        for (int j = 0; j < n; ++j) {
            if (used[j] || (pi != -1 && g[pi][j] < k)) continue;
            if (m <= dp[j][xmask]) {
                ans.emplace_back(a[j]);
                used[j] = 1;
                not_used[j] = 0;
                pi = j;
                break;
            }
            m -= (int)dp[j][xmask];
        }
        if (ans.empty()) {
            printf("-1\n");
            return 0;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%d", ans[i]);
    }

    fclose(stdin);
    fclose(stdout);
    return 0;
}
