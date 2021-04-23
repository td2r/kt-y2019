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

int const MAXN = 1000 + 3;
int const MAXM = 200000 + 3;

int n, m;
int w[MAXN];
int mt[MAXN];
int rmt[MAXN];
int ind[MAXN];
bool used[MAXN];
vector<int> g[MAXN];

bool kuhn(int v) {
    if (used[v])
        return false;
    used[v] = true;
    for (int const& to : g[v]) {
        if (mt[to] == -1 || kuhn(mt[to])) {
            mt[to] = v;
            rmt[v] = to;
            return true;
        }
    }
    return false;
}

int main() {
    freopen("matching.in", "r", stdin);
    freopen("matching.out", "w", stdout);

    memset(mt, 255, sizeof(int) * MAXN);
    memset(rmt, 255, sizeof(int) * MAXN);
    iota(ind, ind + MAXN, 0);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &w[i]);
    }
    for (int i = 0; i < n; ++i) {
        int deg;
        scanf("%d", &deg);
        g[i].resize(deg);
        for (int j = 0; j < deg; ++j) {
            scanf("%d", &g[i][j]);
            --g[i][j];
        }
    }

    sort(ind, ind + n, [&](int const& i1, int const& i2) { return w[i1] > w[i2]; });
    for (int i = 0; i < n; ++i) {
        kuhn(ind[i]);
        memset(used, 0, sizeof(bool) * n);
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", rmt[i] + 1);
    } printf("\n");

    fclose(stdin);
    fclose(stdout);

    return 0;
}
