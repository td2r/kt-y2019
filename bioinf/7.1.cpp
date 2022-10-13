#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 400'000 + 1;

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

void build_sufmas(char const* s, vector<int>& p) {
    int n = (int) strlen(s) + 1;
    assert(s[n - 1] == 0);
    vector<int> pn(n);
    vector<int> c(n);
    vector<int> cn(n);
    vector<int> cnt(max(256, n), 0);
    p.resize(n);

    for (int i = 0; i < n; ++i) {
        ++cnt[(int) s[i]];
    }
    for (int i = 1; i < 256; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; --i) {
        p[--cnt[(int) s[i]]] = i;
    }

    c[p[0]] = 0;
    for (int i = 1; i < n; ++i) {
        c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);
    }

    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; ++i) {
            pn[i] = (p[i] - k + n) % n;
        }

        fill(cnt.begin(), cnt.begin() + n, 0);
        for (int i = 0; i < n; ++i) {
            ++cnt[c[pn[i]]];
        }
        for (int i = 1; i <= c[p[n - 1]]; ++i) {
            cnt[i] += cnt[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            p[--cnt[c[pn[i]]]] = pn[i];
        }

        cn[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            int jp = p[i - 1];
            int j = p[i];
            cn[j] = cn[jp] + (c[j] != c[jp] || c[(j + k) % n] != c[(jp + k) % n]);
        }
        swap(c, cn);
    }
}

inline char bwt(vector<int> const& suff, char const* s, int i) {
    return suff[i] == 0 ? 0 : s[suff[i] - 1];
}

pair<int, int> suff_range(string const& pattern,
                          vector<int>& c,
                          unordered_map<char, vector<int>>& o)
{
    pair<int, int> r = make_pair(0, (int) o.begin()->second.size() - 1);
    for (int i = pattern.size() - 1; i >= 0; --i) {
        r.first = c[pattern[i]] + (r.first == 0 ? 0 : o[pattern[i]][r.first - 1]);
        r.second = c[pattern[i]] + o[pattern[i]][r.second] - 1;
    }
    return r;
}

int main() {
    int n;
    string str;
    cin >> str;
    n = (int) str.length() + 1;
    vector<int> suff({});
    auto s = str.c_str();
    build_sufmas(s, suff);

    vector<int> c(256, 0);
    for (int i = 1; i < n; ++i) {
        if (s[suff[i]] != s[suff[i - 1]]) {
            c[s[suff[i]]] = i;
        }
    }
    unordered_map<char, vector<int>> o;
    for (auto ch : {'A', 'C', 'G', 'T'}) {
        o.insert({ch, vector<int>(n)});
        o[ch][0] = (bwt(suff, s, 0) == ch);
        for (int i = 1; i < n; ++i) {
            o[ch][i] = o[ch][i - 1] + (bwt(suff, s, i) == ch);
        }
    }

    set<int> pos;
    string pattern;
    while (cin >> pattern) {
        auto range = suff_range(pattern, c, o);
        for (int i = range.first; i <= range.second; ++i) {
            pos.insert(suff[i]);
        }
    }

    for (auto it : pos) {
        cout << it << ' ';
    }
    cout << endl;

    return 0;
}
