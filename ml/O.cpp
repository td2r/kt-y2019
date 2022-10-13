#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int kx;
    int ky;
    cin >> kx >> ky;
    int n;
    cin >> n;
    vector<pair<int, int>> obj(n);
    vector<int> cnt_x(kx);
    vector<int> cnt_y(ky);
    for (int i = 0; i < n; ++i) {
        cin >> obj[i].first >> obj[i].second;
        --obj[i].first;
        --obj[i].second;
        ++cnt_x[obj[i].first];
        ++cnt_y[obj[i].second];
    }
    
    floating_t ans = 0.0;
    sort(obj.begin(), obj.end());
    for (int l = 0, r = 0; l < n; l = r) {
        while (r < n && obj[l].first == obj[r].first && obj[l].second == obj[r].second)
            ++r;
        floating_t o = r - l;
        floating_t e = (floating_t) cnt_x[obj[l].first] * (floating_t) cnt_y[obj[l].second] / n;
        ans += o * o / e;
    }
    cout << fixed << setprecision(42) << ans - n << endl;
    return 0;
}