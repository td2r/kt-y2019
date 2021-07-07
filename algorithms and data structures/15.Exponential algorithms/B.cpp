#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 20 + 3;

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

void solve_half(char const* s, int n, vector<pair<string, string>>* p) {
    for (int mask = 0; mask < (1 << n); ++mask) {
        string s1 = "";
        string s2 = "";
        for (int k = 0; k < n; ++k) {
            if (mask & (1 << k)) {
                s1.push_back(s[k]);
            } else {
                s2.push_back(s[k]);
            }
        }
        p[s1.length()].push_back(make_pair(s1, s2));
    }
}

void put_prefix_diff(const string& s1, const string& s2, map<string, int> &cnt) {
    size_t n = s1.length();
    size_t m = s2.length();
    if (n <= m && s2.compare(0, n, s1) == 0 ) {
        cnt[s2.substr(n, m - n)]++;
    } else if (n > m && s1.compare(0, m, s2) == 0) {
        cnt[s1.substr(m, n - m)]++;
    }
}

void put_suffix_diff(const string& s1, const string& s2, map<string, int> &cnt) {
    size_t n = s1.length();
    size_t m = s2.length();
    if (n >= m && s1.compare(n - m, m, s2) == 0) {
        cnt[s1.substr(0, n - m)]++;
    } else if (n < m && s2.compare(m - n, n, s1) == 0) {
        cnt[s2.substr(0, m - n)]++;
    }
}

int n;
char s[2 * MAXN];
vector<pair<string, string>> p1[MAXN];
vector<pair<string, string>> p2[MAXN];

int main() {
    scanf("%s", s);
    n = (int) strlen(s) / 2;

    solve_half(s, n, p1);
    solve_half(s + n, n, p2);

    ll ans = 0;

    for (int i = 0; i <= n; ++i) {
        map<string, int> cnt1;
        map<string, int> cnt2;
        for (auto const& p : p1[i]) {
            put_prefix_diff(p.first, p.second, cnt1);
        }
        for (auto const& p : p2[n - i]) {
            put_suffix_diff(p.first, p.second, cnt2);
        }
        for (auto const& p : cnt1) {
            ans += (ll) p.second * cnt2[p.first];
        }
    }

    printf("%lld\n", ans);

    return 0;
}
