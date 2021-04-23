#include <bits/stdc++.h>
#define mp make_pair
typedef unsigned int uint;
typedef long long ll;
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
void showArray(string arrName, T* beginIterator, T* endIterator) {
    cout << arrName << " = [";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ' ' << *beginIterator++;
    }
    cout << "]" << endl;
}

const bool debug = false;

// -------------------------------------------------------------------------------//

const int MAXN = 3e5 + 1;

int parent[MAXN], rnk[MAXN], leader[MAXN];

void make_set(int x) {
    parent[x] = x;
    leader[x] = x;
}

int find(int x) {
    if (x == parent[x]) {
        return x;
    }
    parent[x] = find(parent[x]);
    leader[x] = leader[parent[x]];
    return parent[x];
}

void unite_sets(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) {
        leader[a] = leader[b];
        if (rnk[a] < rnk[b]) {
            swap(a, b);
        }
        parent[b] = a;
        if (rnk[a] == rnk[b]) {
            ++rnk[a];
        }
    }
}

int main() {
    int n;

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        make_set(i);
    }

    for (int i = 0, p; i < n; ++i) {
        if (i) printf(" ");
        scanf("%d", &p);
        p = find(p - 1);
        printf("%d", leader[p] + 1);
        unite_sets(p, (leader[p] + 1) % n);
        if (debug) {
            printf("\n");
            showArray("parent", parent, parent + n);
            showArray("leader", leader, leader + n);
        }
    }
    printf("\n");
    return 0;
}
