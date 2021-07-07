#include <bits/stdc++.h>
#define DEBUG false
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

void build_sufmas(char const* s, int* p) {
    int n = (int) strlen(s) + 1;
    int* pn = (int*) malloc(sizeof(int) * n);
    int* c = (int*) malloc(sizeof(int) * n);
    int* cn = (int*) malloc(sizeof(int) * n);
    int* cnt = (int*) malloc(sizeof(int) * max(256, n));

    memset(cnt, 0, sizeof(int) * 256);
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

        memset(cnt, 0, sizeof(int) * n);
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

    free(cnt);
    free(cn);
    free(c);
    free(pn);
}

int n;
char s[MAXN];
int p[MAXN];
int r[MAXN];
int lcp[MAXN];

int main() {
    scanf("%s", s);
    n = (int) strlen(s);

    build_sufmas(s, p);

    ++n;

    for (int i = 0; i < n; ++i) {
        r[p[i]] = i;
    }

    for (int i = 0; i < n; ++i) {
        if (r[i] == n - 1) continue;
        int j = p[r[i] + 1];
        int len = i == 0 ? 0 : max(0, lcp[r[i - 1]] - 1);
        while (s[i + len] == s[j + len]) {
            ++len;
        }
        lcp[r[i]] = len;
    }

    ll ans = ((ll) n) * ((ll) (n - 1)) / 2LL;
    for (int i = 1; i < n - 1; ++i) {
        ans -= lcp[i];
    }
    printf("%lld\n", ans);

    return 0;
}
