#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 30 + 1;

template <class forwardIterator>
void print_array(string const& name, forwardIterator first, forwardIterator last) {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

void solve_half(int* x, int n, vector<ll> &a) {
    for (int mask = 0; mask < (1 << n); ++mask) {
        ll w = 0;
        for (int k = 0; k < n; ++k) {
            if (mask & (1 << k)) {
                w += x[k];
            }
        }
        a.emplace_back(w);
    }
    sort(a.begin(), a.end());
}

int n;
ll c;
int x[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &x[i]);
    }
    scanf("%lld", &c);

    int n1 = n / 2;
    int n2 = n - n1;
    vector<ll> a;
    vector<ll> b;
    solve_half(x, n1, a);
    solve_half(x + n1, n2, b);

    // print_array("a", a.begin(), a.end());
    // print_array("b", b.begin(), b.end());

    int ans = 0;
    int i = 0;
    int j = (int) b.size() - 1;
    while (i < (int) a.size()) {
        while (j >= 0 && a[i] + b[j] > c) {
            --j;
        }
        if (j < 0) {
            break;
        }
        ans += j + 1;
        ++i;
    }
    printf("%d\n", ans);

    return 0;
}
