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

const int MAXN = 2e5 + 1;

int readInt() {
    int a = 0, c;
    while ((c = getchar()) != EOF && ('0' <= (char)c && (char)c <= '9')) {
        a = a * 10 + c - '0';
    }
    return a;
}

struct DSU {
    int parent[MAXN], rnk[MAXN];

    DSU(int n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i, rnk[i] = 0;
        }
    }

    int find(int x) {
        if (x == parent[x]) {
            return x;
        }
        stack<int> st;
        while (x != parent[x]) {
            st.push(x);
            x = parent[x];
        }
        while (!st.empty()) {
            parent[st.top()] = x;
            st.pop();
        }
        return x;
    }

    void unite_sets(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (rnk[a] < rnk[b]) {
                swap(a, b);
            }
            parent[b] = a;
            if (rnk[a] == rnk[b]) {
                ++rnk[a];
            }
        }
    }

    bool inSameSet(int x, int y) {
        return find(x) == find(y);
    }

    void show(int n) const {
        printf("---dsu-----------------\n");
        showArray("parent", parent, parent + n);
        printf("\n");
    }
};

struct LeadDSU {
    int parent[MAXN], rnk[MAXN], leader[MAXN];

    LeadDSU(int n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i, rnk[i] = 0, leader[i] = i;
        }
    }

    int find(int x) {
        if (x == parent[x]) {
            return x;
        }
        stack<int> st;
        while (x != parent[x]) {
            st.push(x);
            x = parent[x];
        }
        int t;
        while (!st.empty()) {
            t = st.top();
            parent[t] = x;
            leader[t] = leader[x];
            st.pop();
        }
        return x;
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

    int getLeader(int x) {
        return leader[find(x)];
    }

    void show(int n) const {
        printf("---ldsu-----------------\n");
        showArray("parent", parent, parent + n);
        showArray("leader", leader, leader + n);
        printf("\n");
    }
};

int main() {
    int n = readInt(), q = readInt();

    DSU dsu = DSU(n);
    LeadDSU ldsu = LeadDSU(n);
    int type, x, y;

    while (q--) {
        type = readInt(), x = readInt(), y = readInt();
        if (type == 1) {
            dsu.unite_sets(x - 1, y - 1);
        } else if (type == 2) {
            for (x = ldsu.getLeader(x - 1); x + 1 < y; x = ldsu.getLeader(x + 1)) {
                dsu.unite_sets(x, x + 1);
                ldsu.unite_sets(x, x + 1);
            }
        } else {
            if (dsu.inSameSet(x - 1, y - 1)) {
                putchar('Y');
                putchar('E');
                putchar('S');
                putchar('\n');
            } else {
                putchar('N');
                putchar('O');
                putchar('\n');
            }
        }
        if (debug) {
            dsu.show(n);
            ldsu.show(n);
        }
    }
    return 0;
}
