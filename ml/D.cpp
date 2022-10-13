#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t const EPS = 1e-9;

int sgn(floating_t x) {
    if (abs(x) < EPS)
        return 0;
    return x < 0.0 ? -1 : 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    vector<vector<int>> x(n);
    vector<int> y(n);
    for (int i = 0; i < n; ++i) {
        x[i].resize(m + 1);
        for (int j = 0; j < m; ++j) {
            cin >> x[i][j];
        }
        x[i].back() = 1; // X_m = 1 => A_m * X_m = A_m
        cin >> y[i];
    }
    vector<int> a(m + 1);
    for (int i = 0; i < m + 1; ++i) {
        cin >> a[i];
    }
    
    for (int i = 0; i < n; ++i) {
        floating_t y1 = 0.0;
        for (int j = 0; j < m + 1; ++j) {
            y1 += (floating_t) a[j] * (floating_t) x[i][j];
        }
        // cout << fixed << setprecision(2) << "y1 = " << y1 << endl;
        floating_t sgn1 = sgn(y1 - y[i]);
        floating_t sgn2 = sgn(y1);
        floating_t v = abs(y1) + abs(y[i]);
        floating_t u = abs(y1 - y[i]);
        for (int j = 0; j < m + 1; ++j) {
            floating_t u_ = sgn1 * x[i][j];
            floating_t v_ = sgn2 * x[i][j];
            cout << fixed << setprecision(42) << (u_ * v - v_ * u) / v / v << ' ';
        }
        cout << endl;
    }
    
    return 0;
}
