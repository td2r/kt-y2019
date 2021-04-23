#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;





const int mod = 1022869;
const int MAXN = 1e5 + 1;


pair<int, int> operator + (const pair<int, int>& p1, const pair<int, int>& p2) {
    return mp((p1.first + p2.first) % mod, (p1.second + p2.second) % mod);
}

int n;
int sz[MAXN];
vector<int> g[MAXN];



void calc_sizes(int root = 0, int p = -1) {
    sz[root] = 1;
    for (int to : g[root]) {
        if (to != p) {
            calc_sizes(to, root);
            sz[root] += sz[to];
        }
    }
}

int find_centroid(int root = 0, int p = -1) {
    for (int to : g[root]) {
        if (to != p && sz[to] > n / 2) {
            return find_centroid(to, root);
        }
    }
    return root;
}

int cube(size_t x) {
    ll y = (ll)x;
    y = y * y * y;
    y %= (ll)mod;
    return (int)y;
}

int hash_tree(int root, int p) {
    int h = cube(g[root].size());
    for (int to : g[root]) {
        if (to != p) {
            h = (h + hash_tree(to, root)) % mod;
        }
    }
    return h;
}

bool is_buncker(int c) {
    if (g[c].size() != 2) {
        return false;
    }
    int h1 = hash_tree(g[c][0], c);
    int h2 = hash_tree(g[c][1], c);
    return h1 == h2;
}

int main() {
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    scanf("%d", &n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        scanf("%d %d", &v, &u);
        g[u - 1].emplace_back(v - 1);
        g[v - 1].emplace_back(u - 1);
    }

    calc_sizes();

    int c = find_centroid();

    printf("%s\n", is_buncker(c) ? "YES" : "NO");

    // fclose(stdin); fclose(stdout);
    return 0;
}
