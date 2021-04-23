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




int main() {
    freopen("number.in", "r", stdin);
    freopen("number.out", "w", stdout);

    const int MAXN = 1e3 + 1;

    int n;
    int d[MAXN][MAXN];
    int r[MAXN][MAXN];
    memset(d, 255, sizeof(int) * MAXN * MAXN);
    memset(r, 255, sizeof(int) * MAXN * MAXN);

    scanf("%d", &n);

    queue<pair<int, int>> q;
    q.push(mp(0, 0));
    while (true) {
        if (q.empty()) {
            return 1;
        }
        int sum = q.front().first;
        int rem = q.front().second;
        q.pop();
        if (sum == n && rem == 0) {
            break;
        }
        for (int c = (sum == 0 ? 1 : 0); c <= 9 && sum + c <= n; ++c) {
            int sum1 = sum + c;
            int rem1 = (rem * 10 + c) % n;
            if (d[sum1][rem1] == -1) {
                d[sum1][rem1] = c;
                r[sum1][rem1] = rem;
                q.push(mp(sum1, rem1));
            }
        }
    }

    stack<int> ans;
    for (int i = n, j = 0; i > 0;) {
        int c = d[i][j];
        ans.push(c);
        j = r[i][j];
        i -= c;
    }

    while (!ans.empty()) {
        printf("%d", ans.top());
        ans.pop();
    } printf("\n");

    fclose(stdin);
    fclose(stdout);
    return 0;
}
