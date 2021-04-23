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




const int MAXN = 1e4 + 1;

string s;
string t;
string ans = "";

int lcs1[2][MAXN];
int lcs2[2][MAXN];

void hirshberg(int sl, int sr, int tl, int tr) {
    if (tl == tr) {
        return;
    }
    if (sl + 1 == sr) {
        for (int j = tl; j < tr; ++j) {
            if (s[sl] == t[j]) {
                ans.push_back(s[sl]);
                break;
            }
        }
        return;
    }

    int sm = (sl + sr) / 2;

    memset(lcs1[0], 0, sizeof(int) * (tr - tl + 1));
    memset(lcs2[0], 0, sizeof(int) * (tr - tl + 1));

    for (int i = 1; i <= sm - sl; ++i) {
        for (int j = 1; j <= tr - tl; ++j) {
            if (s[sl + i - 1] == t[tl + j - 1]) {
                lcs1[1][j] = lcs1[0][j - 1] + 1;
            } else {
                lcs1[1][j] = max(lcs1[0][j], lcs1[1][j - 1]);
            }
        }
        swap(lcs1[0], lcs1[1]);
    }

    for (int i = 1; i <= sr - sm; ++i) {
        for (int j = 1; j <= tr - tl; ++j) {
            if (s[sr - i] == t[tr - j]) {
                lcs2[1][j] = lcs2[0][j - 1] + 1;
            } else {
                lcs2[1][j] = max(lcs2[0][j], lcs2[1][j - 1]);
            }
        }
        swap(lcs2[0], lcs2[1]);
    }

    int maxsum = lcs2[0][tr - tl];
    int len = 0;
    for (int i = 1; i <= tr - tl; ++i) {
        if (lcs1[0][i] + lcs2[0][tr - tl - i] > maxsum) {
            maxsum = lcs1[0][i] + lcs2[0][tr - tl - i];
            len = i;
        }
    }
    hirshberg(sl, sm, tl, tl + len);
    hirshberg(sm, sr, tl + len, tr);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> s >> t;

    hirshberg(0, s.size(), 0, t.size());
    cout << ans << endl;

    return 0;
}
