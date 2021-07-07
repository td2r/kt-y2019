#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        // freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        // fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first,
                 forwardIterator last,
                 string const& name = "",
                 string const& separator = ", ") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << separator << *first++;
    }
    cout << endl;
}

int read_uint() {
    char c;
    int res = 0;
    while ((c = (char) getchar()) != -1) {
        if (c == ' ' || c == '\n') {
            return res;
        }
        res = 10 * res + c - '0';
    }
    return res;
}

int const MAXN = 100'000 + 3;
ll const mod = 1'000'027'727;
ll const p = 31;
ll const r = 516'143'343;

int n;
int m;
ll h[MAXN];
char s[MAXN];

ll fast_pow(ll a, ll b) {
    if (b == 0) return 1LL;
    ll t = fast_pow(a, b / 2);
    t = (t * t) % mod;
    if (b & 1) {
        t = (t * a) % mod;
    }
    return t;
}

ll subhash(int a, int b) {
    ll sh = (h[b] - h[a - 1] + mod) % mod;
    sh = (sh * fast_pow(r, a)) % mod;
    return sh;
}

int main() {
    scanf("%s\n", s + 1);
    n = (int) strlen(s + 1);
    
    ll p_pow = 1;
    for (int i = 1; i <= n; ++i) {
        h[i] = (h[i - 1] + s[i] * p_pow) % mod;
        p_pow = (p_pow * p) % mod;
    }

    scanf("%d", &m);
    while (m--) {
        int a, b, c, d;
        scanf("%d %d %d %d", &a, &b, &c, &d);
        printf("%s\n", (subhash(a, b) == subhash(c, d)) ? "Yes" : "No");
    }

    return 0;
}
