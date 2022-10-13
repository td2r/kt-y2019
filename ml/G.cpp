#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef double floating_t;

void decay_args(int x, vector<int>& ws) {
    for (auto& w : ws) {
        w = 2 * (x & 1) - 1; // equals to (x & 1) ? 1 : -1;
        x >>= 1;
    }
}

floating_t conj(vector<int> const& ws) {
    int b = 0;
    for (auto const& w : ws) {
        if (w == 1)
            b += 1;
    }
    return -((floating_t) b - 0.5);
}

floating_t disj(vector<int> const& ws) {
    int b = 0;
    for (auto const& w : ws) {
        if (w == -1)
            b += 1;
    }
    return (floating_t) b - 0.5;
}

void show_neuron(vector<int> const& ws, floating_t const b) {
    for (auto const& w : ws) {
        cout << (floating_t) w << ' ';
    }
    cout << b << endl;
}

void dnf(int m, vector<int> const& f) {
    vector<int> ws(m);
    int ones = accumulate(f.begin(), f.end(), 0);
    cout << ones << ' ' << 1 << endl;
    for (size_t i = 0; i < f.size(); ++i) {
        if (f[i]) {
            decay_args(i, ws);
            show_neuron(ws, conj(ws));
        }
    }
    ws.resize(ones);
    fill(ws.begin(), ws.end(), 1);
    show_neuron(ws, disj(ws));
}

void knf(int m, vector<int> const& f) {
    vector<int> ws(m);
    int zeros = (1 << m) - accumulate(f.begin(), f.end(), 0);
    cout << zeros << ' ' << 1 << endl;
    for (size_t i = 0; i < f.size(); ++i) {
        if (!f[i]) {
            decay_args(i, ws);
            for (auto& w : ws) {
                w = -w;
            }
            show_neuron(ws, disj(ws));
        }
    }
    ws.resize(zeros);
    fill(ws.begin(), ws.end(), 1);
    show_neuron(ws, conj(ws));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cout << fixed << setprecision(42);
    
    int m;
    cin >> m;
    int n = (1 << m);
    vector<int> f(n);
    for (int i = 0; i < n; ++i) {
        cin >> f[i];
    }
    int ones = accumulate(f.begin(), f.end(), 0);
    if (ones == 0 || ones == n) {
        cout << 1 << endl;
        cout << 1 << endl;
        for (int i = 0; i < m; ++i) {
            cout << 0.0 << ' ';
        }
        cout << (ones ? 0.5 : -0.5) << endl;
        return 0;
    }
    cout << 2 << endl;
    if (ones <= n / 2) {
        dnf(m, f);
    } else {
        knf(m, f);
    }
    
    return 0;
}
