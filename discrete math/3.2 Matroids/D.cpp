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

int const MAXN = 10 + 3;
int const MAXM = (1 << MAXN) + 3;

int n, m;
vector<int> I;
bool independent[MAXM];

int bits_count(int x) {
    int res = 0;
    while (x) {
        res += (x & 1);
        x >>= 1;
    }
    return res;
}

bool supplement(int a, int b) {
    if (bits_count(a) == bits_count(b))
        return true;
    if (bits_count(a) < bits_count(b))
        swap(a, b);
    for (int k = 1; k <= (1 << (n - 1)); k <<= 1) {
        if ((a & k) != 0 && (b & k) == 0 && independent[b | k]) {
            return true;
        }
    }
    return false;
}

bool check() {
    if (!independent[0])
        return false;
    for (int const& i : I) {
        for (int j = i; j != 0; j = ((j - 1) & i)) {
            if (!independent[j])
                return false;
        }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < i; ++j) {
            if (!supplement(I[i], I[j])) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    freopen("check.in", "r", stdin);
    freopen("check.out", "w", stdout);

    n = read_uint();
    m = read_uint();
    I.resize(m);
    for (int i = 0; i < m; ++i) {
        int t = read_uint();
        while (t--) {
            I[i] |= (1 << (read_uint() - 1));
        }
        independent[I[i]] = true;
    }
    sort(I.begin(), I.end());

    printf("%s\n", check() ? "YES" : "NO");

    fclose(stdin);
    fclose(stdout);
    return 0;
}
