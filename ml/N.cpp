#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t dispersion(int l, int r, vector<pair<int, int>> const& objects) {
    floating_t s = 0.0;
    floating_t sq_s = 0.0;
    for (int i = l; i < r; ++i) {
        s += objects[i].second;
        sq_s += (ll) objects[i].second * (ll) objects[i].second;
    }
    floating_t n = r - l;
    return sq_s / n - (s / n) * (s / n);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int k;
    cin >> k;
    int n;
    cin >> n;
    vector<pair<int, int>> objects(n);
    for (int i = 0; i < n; ++i) {
        cin >> objects[i].first >> objects[i].second;
    }
    sort(objects.begin(), objects.end());
    floating_t ans = 0.0;
    for (int i = 0, j = 0; i < n; i = j) {
        while (j < n && objects[j].first == objects[i].first)
            ++j;
        floating_t p = (floating_t) (j - i) / (floating_t) n;
        ans += p * dispersion(i, j, objects);
    }
    cout << fixed << setprecision(42) << ans << endl;
    return 0;
}