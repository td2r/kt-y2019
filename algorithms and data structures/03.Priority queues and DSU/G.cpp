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

const bool debug = true;

int readInt() {
    int a = 0, c;
    while ((c = getchar()) != EOF && ('0' <= (char)c && (char)c <= '9')) {
        a = a * 10 + c - '0';
    }
    return a;
}

// --------------------td2r-------------------------------------------------------//

#define X first
#define Y second

const int MAXN = 3e5 + 1;

struct Item {
    int x, l, r;

    Item() : x(), l(), r() {}
    Item(int x, int l, int r) : x(x), l(l), r(r) {}

    bool operator == (const Item &it) const {
        return (x == it.x && (l == it.l && r == it.r));
    }

    bool operator > (const Item &it) const {
        if (x != it.x) {
            return x > it.x;
        } else if (l != it.l) {
            return l > it.l;
        } else {
            return r > it.r;
        }
    }

    bool operator >= (const Item &it) const {
        if (x != it.x) {
            return x > it.x;
        } else if (l != it.l) {
            return l > it.l;
        } else {
            return r >= it.r;
        }
    }
};

struct Heap {
    int sz;
    Item *a;

    Heap() : sz(0) {
        a = new Item[MAXN];
    }

    ~Heap() {
        delete a;
    }

    void siftDown(int i) {
        while (2 * i + 1 < sz) {
            int left = 2 * i + 1,
                right = 2 * i + 2,
                j = left;
            if (right < sz && a[right] > a[left]) {
                j = right;
            }
            if (a[i] >= a[j]) {
                break;
            }
            swap(a[i], a[j]);
            i = j;
        }
    }

    void siftUp(int i) {
        while (a[i] > a[(i - 1) / 2]) {
            swap(a[i], a[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void add(Item x) {
        a[sz++] = x;
        siftUp(sz - 1);
    }

    Item get_max() {
        return a[0];
    }

    Item extract_max() {
        Item ret = a[0];
        swap(a[0], a[sz - 1]);
        if (--sz) {
            siftDown(0);
        }
        return ret;
    }

    bool empty() const {
        return sz == 0;
    }
};

int main() {
    int n, m;
    vector<Item> rmq[MAXN];
    Heap h = Heap();

    scanf("%d %d", &n, &m);
    while (m--) {
        int l, r, q;
        scanf("%d %d %d", &l, &r, &q);
        rmq[l - 1].emplace_back(Item(q, l - 1, r));
    }
    for (int i = 0; i < n; ++i) {
        for (Item it : rmq[i]) {
            h.add(it);
        }
        while (!h.empty() && h.get_max().r <= i) {
            h.extract_max();
        }
        if (i) printf(" ");
        if (h.empty()) {
            printf("0");
        } else {
            printf("%d", h.get_max().x);
        }
    }
    printf("\n");
    return 0;
}
