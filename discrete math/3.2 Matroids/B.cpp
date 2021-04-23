#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


template <class forwardIterator>
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

int read_uint() {
    char c;
    int res = 0;
    while (c = getchar()) {
        if (c == ' ' || c == '\n')
            return res;
        res = 10 * res + (c - '0');
    }
    return res;
}

int const MAXN = 50000 + 3;
int const MAXM = 100000 + 3;

struct edge {
    int a;
    int b;
    ll w;
    int i;

    edge()
    : a()
    , b()
    , w()
    , i()
    {}

    edge(int a, int b, ll w, int i)
    : a(a)
    , b(b)
    , w(w)
    , i(i)
    {}

    void scan(int const i) {
        this->i = i;
        scanf("%d %d %lld", &a, &b, &w);
        --a, --b;
    }

    void print() const {
        printf("%d %d %lld", a + 1, b + 1, w);
    }

    bool operator < (edge const& other) {
        return w < other.w;
    }

    bool operator > (edge const& other) {
        return w > other.w;
    }
};

bool operator < (edge const& e1, edge const& e2) {
    return e1.w < e2.w;
}

bool operator > (edge const& e1, edge const& e2) {
    return e1.w > e2.w;
}

ll s;
int n, m;
edge e[MAXM];
int sz[MAXN];
int parent[MAXN];
bool in_mst[MAXM];

int find_set(int v) {
    return parent[v] == v ? v : parent[v] = find_set(parent[v]);
}

bool unite_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (sz[a] < sz[b])
            swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
    return false;
}

int main() {
    freopen("destroy.in", "r", stdin);
    freopen("destroy.out", "w", stdout);

    vector<int> ans;
    fill(sz, sz + MAXN, 1);
    iota(parent, parent + MAXN, 0);

    scanf("%d %d %lld", &n, &m, &s);
    for (int i = 0; i < m; ++i) {
        e[i].scan(i);
    }

    sort(e, e + m, [](edge const& e1, edge const& e2) { return e1 > e2; });

    for (int i = 0; i < m; ++i) {
        if (unite_sets(e[i].a, e[i].b)) {
            in_mst[e[i].i] = true;
        }
    }
    sort(e, e + m);
    for (int i = 0; i < m; ++i) {
        if (in_mst[e[i].i])
            continue;
        if (e[i].w <= s) {
            ans.emplace_back(e[i].i);
            s -= e[i].w;
        } else {
            break;
        }
    }

    printf("%zd\n", ans.size());
    for (int const& i : ans) {
        printf("%d ", i + 1);
    } printf("\n");

    fclose(stdin);
    fclose(stdout);

    return 0;
}
