#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

pair<int, int> get_min_limb(vector<vector<int>> const& d, int n) {
    pair<int, int> p = make_pair(0, 1);
    int min_len = d[0][n - 1] + d[n - 1][1] - d[0][1];
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            int new_len = d[i][n - 1] + d[n - 1][j] - d[i][j];
            if (new_len < min_len) {
                p.first = i;
                p.second = j;
                min_len = new_len;
            }
        }
    }
    return p;
}

bool dfs(vector<set<pair<int, int>>> const& g,
         vector<int>& route,
         int v,
         int aim,
         int p = -1)
{
    if (v == aim) {
        route.emplace_back(v);
        return true;
    }
    for (auto const& e : g[v]) {
        int to = e.first;
        if (to != p && dfs(g, route, to, aim, v)) {
            route.emplace_back(v);
            return true;
        }
    }
    return false;
}

vector<set<pair<int, int>>> additive_phylogeny(vector<vector<int>> const& d,
                                               int const n)
{
    if (n == 2) {
        vector<set<pair<int, int>>> t(d.size());
        t[0].insert(make_pair(1, d[0][1]));
        t[1].insert(make_pair(0, d[0][1]));
        return t;
    }
    auto p = get_min_limb(d, n);
    int limb_len =
        (d[p.first][n - 1] + d[n - 1][p.second] - d[p.first][p.second]) / 2;
    int x = d[p.first][n - 1] - limb_len;

    auto t = additive_phylogeny(d, n - 1);

    // adding new vertex between path from p.first to p.second
    // at distance x from p.first
    vector<int> route;
    assert(dfs(t, route, p.second, p.first));
    int v;
    int i = 0;
    while (t[route[i]].lower_bound(make_pair(route[i + 1], 0))->second <= x) {
        x -= t[route[i]].lower_bound(make_pair(route[i + 1], 0))->second;
        ++i;
    }
    if (x == 0) {
        v = route[i];
    } else {
        v = t.size();
        t.emplace_back(); // new vertex
        int l = t[route[i]].lower_bound(make_pair(route[i + 1], 0))->second;
        // delete edge route[i] <-> route[i + 1]
        t[route[i]].erase(make_pair(route[i + 1], l));
        t[route[i + 1]].erase(make_pair(route[i], l));
        // add edge route[i] <-> v
        t[route[i]].insert(make_pair(v, x));
        t[v].insert(make_pair(route[i], x));
        // add edge route[i + 1] <-> v
        t[route[i + 1]].insert(make_pair(v, l - x));
        t[v].insert(make_pair(route[i + 1], l - x));
    }
    t[v].insert(make_pair(n - 1, limb_len));
    t[n - 1].insert(make_pair(v, limb_len));
    return t;
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> d(n);
    for (int i = 0; i < n; ++i) {
        d[i].resize(n);
        for (int j = 0; j < n; ++j) {
            cin >> d[i][j];
            d[i][j] *= 2;
        }
    }

    auto t = additive_phylogeny(d, d.size());

    for (size_t i = 0; i < t.size(); ++i) {
        for (auto const& e : t[i]) {
            cout << i << "->" << e.first << ":" << e.second / 2 << (e.second % 2 ? ".5" : "") << endl;
        }
    }

    return 0;
}
