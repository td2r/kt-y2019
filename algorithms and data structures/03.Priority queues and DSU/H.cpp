#include <bits/stdc++.h>
#define mp make_pair
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
using namespace std;

template<typename T1, typename T2>
bool operator < (const pair<T1, T2> &p1, const pair<T1, T2> &p2) {
    if (p1.first == p2.first) {
        return p1.second < p2.second;
    }
    return p1.first < p2.first;
}

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

const bool debug = true;

int readInt() {
    int a = 0, c;
    while ((c = getchar()) != EOF && ('0' <= (char)c && (char)c <= '9')) {
        a = a * 10 + c - '0';
    }
    return a;
}

// --------------------td2r-------------------------------------------------------//

const int MAXN = 2e5 + 1;

int parent[MAXN], rnk[MAXN], parity[MAXN];

int find(int x) {
    if (x != parent[x]) {
       int foundParent = find(parent[x]);
       parity[x] ^= parity[parent[x]];
       parent[x] = foundParent;
    }
    return parent[x];
}

void unite_sets(int a, int b) {
    find(a), find(b);
    int x = parity[a], y = parity[b];
    a = parent[a], b = parent[b];
    if (a != b) {
        if (rnk[a] < rnk[b]) {
            swap(a, b);
        }
        parity[b] = x ^ y ^ 1;
        parent[b] = a;
        if (rnk[a] == rnk[b]) {
            ++rnk[a];
        }
    }
}

int main() {
    int n, m, shift = 0;

    scanf("%d %d", &n, &m);

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
    while (m--) {
        int type, a, b;
        scanf("%d %d %d", &type, &a, &b);
        if (type == 0) {
            unite_sets((a + shift - 1) % n, (b + shift - 1) % n);
        } else {
            find((a + shift - 1) % n);
            find((b + shift - 1) % n);
            if (parity[(a + shift - 1) % n] == parity[(b + shift - 1) % n]) {
                printf("YES\n");
                shift = (shift + 1) % n;
            } else {
                printf("NO\n");
            }
        }
    }
    return 0;
}
