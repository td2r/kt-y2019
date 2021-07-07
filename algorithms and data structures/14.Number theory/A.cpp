#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 20'000'000 + 3;

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

int n;
bool composite[MAXN];

int main() {
    scanf("%d", &n);
    for (int i = 2; i < MAXN; ++i) {
        if (!composite[i]) {
            for (int j = 2 * i; j < MAXN; j += i) {
                composite[j] = true;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        int a;
        scanf("%d", &a);
        printf("%s\n", composite[a] ? "NO" : "YES");
    }

    return 0;
}
