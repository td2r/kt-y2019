#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t entropy(int l, int r, vector<pair<int, int>> const& objects) {
    unordered_map<int, int> cnt;
    floating_t const n = r - l;
    for (int i = l; i < r; ++i) {
        cnt[objects[i].second] = cnt[objects[i].second] + 1;
    }
    floating_t ans = 0.0;
    for (auto const& p : cnt) {
        ans += ((floating_t) p.second / n) * log(n / (floating_t) p.second);
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int kx;
    int ky;
    cin >> kx >> ky;
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
        ans += p * entropy(i, j, objects);
    }
    cout << fixed << setprecision(42) << ans << endl;
    return 0;
}