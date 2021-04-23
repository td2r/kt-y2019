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
    freopen("lis.in", "r", stdin);
    freopen("lis.out", "w", stdout);
    const int MAXN = 3e5 + 2;

    int n;
    int a[MAXN];
    int p[MAXN];
    int e_ind[MAXN] = {-1};
    int e_val[MAXN] = {INT_MIN};
    fill(e_val + 1, e_val + MAXN, INT_MAX);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);

        int j = lower_bound(e_val, e_val + MAXN, a[i]) - e_val;

        assert(j > 0);

        e_val[j] = a[i];
        e_ind[j] = i;
        p[i] = e_ind[j - 1];
    }

    int m = (lower_bound(e_val, e_val + MAXN, INT_MAX) - e_val) - 1;
    printf("%d\n", m);
    stack<int> st;
    for (int i = e_ind[m]; i != -1; i = p[i]) {
        st.push(a[i]);
    }
    while (true) {
        printf("%d", st.top());
        st.pop();
        if (st.empty()) {
            printf("\n");
            break;
        } else {
            printf(" ");
        }
    }

    fclose(stdin);
    fclose(stdout);
    return 0;
}
