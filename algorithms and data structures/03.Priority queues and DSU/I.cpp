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
    int n, m;

    scanf("%d %d", &n, &m);

    if (m <= 16) {
        for (int i = 0; i < m; ++i) {
            printf("0 1 1\n");
        }
        return 0;
    }

    int k = (int)(sqrt(2 * m * log(m) / log(2) + 4) - 2);
    for (int i = 1; i <= k; ++i) {
        assert(n - k >= 1);
        printf("1 %d %d\n", n - k, n - k + i);
        --m;
    }
    while (m > 1 && n - k - 1 >= 1) {
        assert(n - k - 1 >= 1);
        printf("1 %d %d\n", n - k - 1, n - k);
        --m;
        for (int i = 1; i <= k && m; ++i) {
            printf("0 %d %d\n", n - k, n - k + i);
            --m;
        }
        ++k;
    }
    while (m) {
        printf("0 %d %d\n", 1, min(2, n));
        --m;
    }
    return 0;
}
