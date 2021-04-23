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

const bool debug = false;

int readInt() {
    int a = 0, c;
    while ((c = getchar()) != EOF && ('0' <= (char)c && (char)c <= '9')) {
        a = a * 10 + c - '0';
    }
    return a;
}

// --------------------td2r-------------------------------------------------------//

const ll p = 1e6 + 3;
const int MAXN = 1e5 + 1;

ll zerg = 0;
int parent[2 * MAXN], d[2 * MAXN], freeNode;

int find(int x) {
    if (x != parent[x]) {
        int foundParent = find(parent[x]);
        if (foundParent != parent[x]) {
            d[x] += d[parent[x]];
            parent[x] = foundParent;
        }
    }
    return parent[x];
}

void unite_sets(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        parent[a] = freeNode;
        parent[b] = freeNode++;
        zerg = (13LL * zerg + 11) % p;
    }
}

int main() {
    int n, m;

    scanf("%d %d", &n, &m);

    freeNode = n;
    for (int i = 0; i < 2 * n; ++i) {
        parent[i] = i;
    }
    while (m--) {
        int type, i, j = -1;
        scanf("%d %d", &type, &i);

        i = (i + (int)zerg) % n;
        if (type == 1) {
            ++d[find(i)];
            zerg = (30LL * zerg + 239) % p;
        } else if (type == 2) {
            scanf("%d", &j);
            j = (j + (int)zerg) % n;
            unite_sets(i, j);
        } else {
            int q;
            if (find(i) == i) {
                q = d[i];
                d[i] = 0;
            } else {
                q = d[i] + d[parent[i]];
                d[i] = -d[parent[i]];
            }
            zerg = (100500LL * zerg + q) % p;
            printf("%d\n", q);
        }
        if (debug) {
            printf("type = %d, i = %d, j = %d\n", type, i, j);
            showArray("parent", parent, parent + freeNode);
            showArray("     d", d, d + freeNode);
        }
    }
    return 0;
}
