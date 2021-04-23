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




int main() {
    freopen("meetings.in", "r", stdin);
    freopen("meetings.out", "w", stdout);

    const int MAXN = 20 + 1;
    const int MAXM = (1 << MAXN);

    int n, k;
    int a[MAXN];
    int b[MAXN];
    int c[MAXN];
    int p[MAXM];
    int dp[MAXM];
    fill(dp, dp + MAXM, INT_MIN);

    scanf("%d %d", &n, &k);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &a[i], &b[i], &c[i]);
    }
    dp[0] = k;
    p[0] = -1;

    int ans = 0;
    int ans_mask = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
        if (dp[mask] == INT_MIN) continue;
        bitset<MAXN> used(mask);
        if ((int)used.count() > ans) {
            ans = used.count();
            ans_mask = mask;
        }
        for (int i = 0; i < n; ++i) {
            if (used[i] || dp[mask] < a[i] || dp[mask] > b[i]) continue;
            int nm = (mask | (1 << i));
            if (dp[mask] + c[i] > dp[nm]) {
                dp[nm] = dp[mask] + c[i];
                p[nm] = i;
            }
        }
    }

    printf("%d\n", ans);
    stack<int> st;
    while (ans--) {
        int x = p[ans_mask];
        st.push(x);
        ans_mask ^= (1 << x);
    }
    while (!st.empty()) {
        printf("%d ", st.top() + 1);
        st.pop();
    } printf("\n");

    fclose(stdin);
    fclose(stdout);
    return 0;
}
