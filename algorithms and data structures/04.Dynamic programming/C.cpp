#include <bits/stdc++.h>
#define debug false
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

template<class forwardIterator>
void show_container(forwardIterator beginIterator, forwardIterator endIterator) {
    cout << "[";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}

template<class forwardIterator>
void show_container(const string cname,
        forwardIterator beginIterator,
        forwardIterator endIterator) {
    cout << cname << " = [";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}




const int MAXN = 2e3 + 1;
const int INF = INT_MAX;

int n;
int f[MAXN];
ll w[MAXN][MAXN];
ll d[MAXN][MAXN];
int r[MAXN][MAXN];
char s[MAXN];


void extract_code(int a, int b, int p) {
    if (a >= b) {
        printf("%s\n", s);
        return;
    }
    s[p] = '0';
    extract_code(a, r[a][b], p + 1);
    s[p] = '1';
    extract_code(r[a][b] + 1, b, p + 1);
    s[p] = 0;
}

int main() {
    freopen("optimalcode.in", "r", stdin);
    freopen("optimalcode.out", "w", stdout);
    // freopen("data.in", "r", stdin);
    // freopen("data.out", "w", stdout);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &f[i]);
    }

    for (int i = 0; i < n; ++i) {
        w[i][i] = f[i];
        r[i][i] = i;
        for (int j = i + 1; j < n; ++j) {
            w[i][j] = w[i][j - 1] + f[j];
        }
    }
    for (int len = 1; len < n; ++len) {
        for (int i = 0; i + len < n; ++i) {
            int j = i + len;
            d[i][j] = LLONG_MAX;
            for (int k = r[i][j - 1]; k <= r[i + 1][j]; ++k) {
                if (d[i][k] + d[k + 1][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k + 1][j];
                    r[i][j] = k;
                }
            }
            d[i][j] += w[i][j];
        }
    }

    printf("%lld\n", d[0][n - 1]);
    extract_code(0, n - 1, 0);

    fclose(stdin);
    fclose(stdout);
    return 0;
}
