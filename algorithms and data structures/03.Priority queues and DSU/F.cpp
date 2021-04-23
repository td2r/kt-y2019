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

int readInt() {
    int a = 0, c;
    while ((c = getchar()) != EOF && ('0' <= (char)c && (char)c <= '9')) {
        a = a * 10 + c - '0';
    }
    return a;
}

// -------------------------------------------------------------------------------//

const int MAXN = 5e4 + 1;

int parent[MAXN], rnk[MAXN];

int find(int x) {
    if (x == parent[x]) {
        return x;
    }
    return parent[x] = find(parent[x]);
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

struct Item {
    bool ask;
    int u, v;

    Item() : ask(), u(), v() {}
    Item(bool ask, int u, int v) : ask(ask), u(u), v(v) {}
};

int main() {
    int n = readInt(), m = readInt(), k = readInt();
    stack<Item> st;
    stack<bool> answers;

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
    for (int i = 0; i < m; ++i) {
        readInt(), readInt();
    }
    for (int i = 0; i < k; ++i) {
        char c = (char)getchar();
        getchar(), getchar(), getchar();
        st.push(Item(c == 'a', readInt(), readInt()));
    }
    while (!st.empty()) {
        Item item = st.top(); st.pop();
        if (item.ask) {
            answers.push(find(item.u - 1) == find(item.v - 1));
        } else {
            unite_sets(item.u - 1, item.v - 1);
        }
    }
    while (!answers.empty()) {
        if (answers.top()) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
        answers.pop();
    }
    return 0;
}
