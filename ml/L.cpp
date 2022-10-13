#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t const EPS = 1e-9;

map<int, int> calc_ranks(vector<int> values) {
    map<int, int> ranks;
    for (auto const& v : values) {
        ranks.insert(make_pair(v, 0));
    }
    int rank = 1;
    for (auto& p : ranks) {
        p.second = rank++;
    }
    return ranks;
}

floating_t spearmen(vector<int>& x, vector<int>& y) {
    if (x.size() == 1)
        return 0;
    auto x_rank = calc_ranks(x);
    auto y_rank = calc_ranks(y);
    floating_t d_sq_sum = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        floating_t d = x_rank[x[i]] - y_rank[y[i]];
        d_sq_sum += d * d;
    }
    floating_t n = x.size();
    return 1 - 6 * d_sq_sum / (n * (n * n - 1));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<int> x(n);
    vector<int> y(n);
    for (int i = 0; i < n; ++i) {
        cin >> x[i] >> y[i];
    }
    
    cout << fixed << setprecision(42) << spearmen(x, y) << endl;
    return 0;
}