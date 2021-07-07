#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 100;

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

wchar_t const view[] = L"⬜⬛";
void print(char const* a, int const d, char const c) {
    for (int i = 0; i < MAXN - d; ++i) {
        putwchar(view[c]);
    }
    for (int i = MAXN - d; i <= MAXN + d; ++i) {
        putwchar(view[a[i]]);
    }
    for (int i = MAXN + d + 1; i < 2 * MAXN + 1; ++i) {
        putwchar(view[c]);
    }
    putwchar('\n');
}

char s[] = "output###.txt";
char change[8];
char a[2 * MAXN + 1];
char b[2 * MAXN + 1];

void live() {
    memset(a, 0, sizeof(char) * (2 * MAXN + 1));
    memset(b, 0, sizeof(char) * (2 * MAXN + 1));
    a[MAXN] = 1;
    print(a, 0, 0);

    char c = 0;

    for (int i = 1; i <= MAXN; ++i) {
        int state = ((c << 1) | a[MAXN - i]) | a[MAXN - i + 1];
        b[MAXN - i] = change[state];
        for (int j = MAXN - i + 1; j < MAXN + i; ++j) {
            state = ((state << 1) & 6) | a[j + 1];
            b[j] = change[state];
        }
        state = ((state << 1) & 6) | c;
        b[MAXN + i] = change[state];

        c = change[(((c << 1) | c) << 1) | c];

        swap(a, b);

        print(a, i, c);
    }
}

int main() {
    setlocale(LC_ALL, "");

    // freopen("output.txt", "w", stdout);
    for (int conf = 0; conf < (1 << 8); ++conf) {
        s[6] = conf / 100 + '0';
        s[7] = (conf / 10) % 10 + '0';
        s[8] = conf % 10 + '0';
        freopen(s, "w", stdout);

        putwchar('#');
        putwchar(conf / 100 + '0');
        putwchar((conf / 10) % 10 + '0');
        putwchar(conf % 10 + '0');
        putwchar('\n');
        for (int i = 0; i < 8; ++i) {
            change[i] = ((conf >> (7 - i)) & 1);
            putwchar((i >> 2) + '0');
            putwchar(((i >> 1) & 1) + '0');
            putwchar((i & 1) + '0');
            putwchar(' ');
            putwchar('-');
            putwchar('>');
            putwchar(' ');
            putwchar(change[i] + '0');
            putwchar('\n');
        }

        live();

        fclose(stdout);
    }
    // fclose(stdout);

    return 0;
}
