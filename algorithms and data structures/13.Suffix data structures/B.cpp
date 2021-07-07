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
    assert(s[n - 1] == 0);
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

    int i = find(p, p + n, n - 1) - p;
    while (i < n) {
        swap(p[i], p[i + 1]);
        ++i;
    }
    p[n - 1] = -1;

    free(cnt);
    free(cn);
    free(c);
    free(pn);
}

int n;
char s[MAXN];
int sufmas[MAXN];

int main() {
    scanf("%s", s);
    n = (int) strlen(s);

    build_sufmas(s, sufmas);

    for (int i = 0; i < n; ++i) {
        printf("%d ", sufmas[i] + 1);
    } printf("\n");

    return 0;
}
