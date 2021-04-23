#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


template <class forwardIterator>
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
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

int read_uint() {
    int x;
    scanf("%d", &x);
    return x;
    // char c;
    // int res = 0;
    // while (c = getchar()) {
    //     if (c == ' ' || c == '\n') {
    //         return res;
    //     }
    //     res = 10 * res + (c - '0');
    // }
    // return res;
}

int const MAXN = 20 + 3;
int const MAXM = (1 << MAXN) + 3;

int ans;
int n, m;
int w[MAXN];
int ind[MAXN];
bool independent[MAXM];
vector<int> cycle;

bool is_independent(int s) {
    for (int const& i : cycle) {
        if ((s ^ i) == s - i) {
            return false;
        }
    }
    return true;
}

int main() {
    freopen("cycles.in", "r", stdin);
    freopen("cycles.out", "w", stdout);

    n = read_uint();
    m = read_uint();

    for (int i = 0; i < n; ++i) {
        w[i] = read_uint();
    }

    for (int i = 0; i < m; ++i) {
        int t = read_uint();
        int set_index = 0;
        while (t--) {
            set_index |= (1 << (read_uint() - 1));
        }
        cycle.emplace_back(set_index);
    }

    // print_array(independent, independent + (1 << n), "independent");
    iota(ind, ind + n, 0);
    sort(ind, ind + n, [&](int const& ind1, int const& ind2) { return w[ind1] > w[ind2]; });

    for (int i = 0, s = 0; i < n; ++i) {
        int x = ind[i];
        if (is_independent(s | (1 << x))) {
            ans += w[x];
            s |= (1 << x);
        }
    }

    printf("%d\n", ans);

    fclose(stdin);
    fclose(stdout);
    return 0;
}
