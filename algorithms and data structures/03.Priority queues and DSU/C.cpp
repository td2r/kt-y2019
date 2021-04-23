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

#define X first
#define Y second

vector<pair<int, int>> a;
vector<int> p;

void siftDown(int i) {
    int sz = (int)a.size();
    while (2 * i + 1 < sz) {
        int left = 2 * i + 1,
            right = 2 * i + 2,
            j = left;
        if (right < sz && a[right].X < a[left].X) {
            j = right;
        }
        if (a[i].X <= a[j].X) {
            break;
        }
        swap(p[a[i].Y], p[a[j].Y]);
        swap(a[i], a[j]);
        i = j;
    }
}

void siftUp(int i) {
    while (a[i].X < a[(i - 1) / 2].X) {
        swap(p[a[i].Y], p[a[(i - 1) / 2].Y]);
        swap(a[i], a[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void insert(int x) {
    p.emplace_back((int)a.size());
    a.emplace_back(mp(x, (int)p.size() - 1));
    siftUp((int)a.size() - 1);
}

pair<int, int> extract_min() {
    if (a.empty()) {
        printf("pomello\n");
    }
    pair<int, int> ret = a[0];
    p[a.back().Y] = 0;
    p[a[0].Y] = -1;
    swap(a[0], a.back());
    a.pop_back();
    if (!a.empty()) {
        siftDown(0);
    }
    return ret;
}

void decrease_key(int T, int newval) {
    if (T >= (int)p.size() || p[T] >= (int)a.size()) {
        printf("pomello\n");
    }
    if (p[T] == -1) {
        return;
    }
    a[p[T]].X = newval;
    siftUp(p[T]);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string buff;
    while (cin >> buff) {
        if (buff[0] == 'p') {
            int key;
            cin >> key;
            insert(key);
        } else if (buff[0] == 'e') {
            if (a.empty()) {
                cout << "*" << endl;
            } else {
                pair<int, int> ret = extract_min();
                cout << ret.X << ' ' << ret.Y + 1 << endl;
            }
            p.emplace_back(-1);
        } else {
            int T, newval;
            cin >> T >> newval;
            decrease_key(T - 1, newval);
            p.emplace_back(-1);
        }
    }
    return 0;
}
