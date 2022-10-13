#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

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

int main() {
    string s;
    cin >> s;
    int n = (int) s.length() + 1;
    auto cs = s.c_str();
    vector<int> suff;
    build_sufmas(cs, suff);
    vector<int> lcp(n);
    vector<int> pos(n);
    for (int i = 0; i < n; ++i) {
        pos[suff[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < n; ++i) {
        if (pos[i] == n - 1) {
            lcp[n - 1] = -1;
            k = 0;
            continue;
        }
        if (k > 0) {
            --k;
        }
        int j = suff[pos[i] + 1];
        while (cs[i + k] == cs[j + k])
            ++k;
        lcp[pos[i]] = k;
    }

    int max_lcp = -1;
    int suff_i = n - 1;
    for (int i = 0; i < n - 1; ++i) {
        if (lcp[i] > max_lcp) {
            max_lcp = lcp[i];
            suff_i = i;
        }
    }
    cout << s.substr(suff[suff_i], max_lcp) << endl;

    return 0;
}
