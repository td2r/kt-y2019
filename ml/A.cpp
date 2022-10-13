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
    
    int n, m, k;
    cin >> n >> m >> k;
    vector<pair<int, int>> c(n);
    vector<vector<int>> part(k);
    for (int i = 0; i < n; ++i) {
        cin >> c[i].first;
        c[i].second = i;
    }
    sort(c.begin(), c.end());
    for (int i = 0; i < n; ++i) {
        part[i % k].emplace_back(c[i].second);
    }
    for (int i = 0; i < k; ++i) {
        cout << part[i].size();
        for (auto const& j : part[i]) {
            cout << ' ' << j + 1;
        }
        cout << endl;
    }
    
    return 0;
}
