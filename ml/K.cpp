#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t const EPS = 1e-9;

floating_t pirson(vector<pair<ll, ll>> const& objects) {
    floating_t const n = objects.size();
    floating_t x_sum = 0.0;
    floating_t y_sum = 0.0;
    floating_t prod_sum = 0.0;
    floating_t x_sq_sum = 0.0;
    floating_t y_sq_sum = 0.0;
    for (auto const& obj : objects) {
        x_sum += obj.first;
        y_sum += obj.second;
        prod_sum += obj.first * obj.second;
        x_sq_sum += obj.first * obj.first;
        y_sq_sum += obj.second * obj.second;
    }
    if (sqrtl(x_sq_sum - x_sum * x_sum / n) < EPS || sqrtl(y_sq_sum - y_sum * y_sum / n) < EPS) {
        return 0;
    }
    return (prod_sum - x_sum * y_sum / n) /
        sqrtl(x_sq_sum - x_sum * x_sum / n) /
        sqrtl(y_sq_sum - y_sum * y_sum / n);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    vector<pair<ll, ll>> objects(n);
    for (int i = 0; i < n; ++i) {
        cin >> objects[i].first >> objects[i].second;
    }
    floating_t p = pirson(objects);
    
    cout << fixed << setprecision(42) << pirson(objects) << endl;
    return 0;
}