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




int mod;

vector<vector<int>> operator * (const vector<vector<int>> &a,
        const vector<vector<int>> &b) {
    int n = a.size();
    int m = b[0].size();
    int k = b.size();
    vector<vector<int>> res(n, vector<int>(m, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int t = 0; t < k; ++t) {
                res[i][j] = (res[i][j] + a[i][t] * b[t][j]) % mod;
            }
        }
    }
    return res;
}

char* div2n(char *p, int &l) {
    int carry = 0;
    if (p[0] == '1') {
        carry = 1;
        --l;
        ++p;
    }
    for (int i = 0; i < l; ++i) {
        carry = carry * 10 + (p[i] - '0');
        p[i] = (char)((carry / 2) + '0');
        carry %= 2;
    }
    return p;
}

int main() {
    freopen("nice3.in", "r", stdin);
    freopen("nice3.out", "w", stdout);

    const int MAXL = 100 + 1;
    const int MAXM = 6;

    char n[MAXL], *p = n;
    int l, m;

    scanf("%s %d %d", n, &m, &mod);
    l = (int)strlen(n);

    for (int i = l - 1; i >= 0; --i) {
        if (p[i] == '0') {
            p[i] = '9';
        } else {
            --p[i];
            if (i == 0 && p[i] == '0') {
                --l;
                ++p;
            }
            break;
        }
    }

    vector<vector<int>> s(1 << m, vector<int>(1, 1));
    vector<vector<int>> a(1 << m, vector<int>(1 << m, 1));
    vector<vector<int>> A(1 << m, vector<int>(1 << m, 0));
    vector<vector<int>> e(1 << m, vector<int>(1));

    for (int i = 0; i < (1 << m); ++i) {
        bitset<MAXM> m1(i);
        for (int j = 0; j < (1 << m); ++j) {
            bitset<MAXM> m2(j);
            for (int k = 0; k + 1 < m; ++k) {
                if ((m1[k] + m2[k] + m1[k + 1] + m2[k + 1]) % 4 == 0) {
                    a[i][j] = 0;
                    break;
                }
            }
        }
        A[i][i] = 1;
    }

    while (l) {
        if ((p[l - 1] - '0') % 2 == 1) {
            A = A * a;
        }
        a = a * a;
        p = div2n(p, l);
    }

    e = A * s;

    int ans = 0;
    for (int i = 0; i < (1 << m); ++i) {
        ans = (ans + e[i][0]) % mod;
    }

    printf("%d\n", ans);

    fclose(stdin);
    fclose(stdout);
    return 0;
}
