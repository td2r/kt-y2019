#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include <bits/stdc++.h>
#define DEBUG false
#define PI M_PI
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

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

int const MAXN = 524'288;

int read_bigint(complex<double> *a) {
    int sgn = 1;
    int c = getchar();
    if (c == '-') {
        sgn = -1;
        c = getchar();
    }
    int i = 0;
    for (; c != -1 && c != '\n'; c = getchar()) {
        a[i++] = complex<double>(c - '0');
    }
    reverse(a, a + i);
    return sgn;
}

complex<double> buffer[MAXN / 2];

void fft(complex<double> *a, int n, bool invert) {
    if (n == 1) return;

    int h = n / 2;

    for (int i = 0; i < h; ++i) {
        buffer[i] = a[2 * i + 1];
        a[i] = a[2 * i];
    }
    for (int i = 0; i < h; ++i) {
        a[i + h] = buffer[i];
    }

    fft(a, h, invert);
    fft(a + h, h, invert);

    double ang = 2 * PI / n * (invert ? -1 : 1);
    complex<double> w(1), wn(cos(ang), sin(ang));
    for (int i = 0; i < h; ++i) {
        auto a0 = a[i];
        auto a1 = a[i + h];
        a[i] = a0 + w * a1;
        a[i + h] = a0 - w * a1;
        if (invert) {
            a[i] /= 2;
            a[i + h] /= 2;
        }
        w *= wn;
    }
}

complex<double> a[MAXN];
complex<double> b[MAXN];
char s[2 * MAXN];

int main() {
    int sgn = read_bigint(a) * read_bigint(b);

    // print_array("a", a, a + MAXN);
    // print_array("b", b, b + MAXN);

    fft(a, MAXN, false);
    fft(b, MAXN, false);

    for (int i = 0; i < MAXN; ++i) {
        a[i] *= b[i];
    }

    fft(a, MAXN, true);

    // print_array("c", a, a + MAXN);

    int n = 0;
    ll carry = 0;
    for (int i = 0; i < MAXN; ++i) {
        ll res = (ll) (a[i].real() + 0.5) + carry;
        carry = res / 10;
        s[n++] = (char) ((res % 10) + '0');
    }

    while (n > 0 && s[--n] == '0');

    if (sgn == -1 && (n > 0 || s[0] != '0')) {
        putchar('-');
    }

    while (n != -1) {
        putchar(s[n--]);
    }

    return 0;
}
