#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000'000'000;
uint const MAXM = (MAXN - 3) / 2;

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

vector<bool> prime(MAXM, true);

int main() {
    int n, x, h = 2;
    scanf("%d %d", &n, &x);
    if (n == 2) {
        printf("%d\n", h);
        return 0;
    }
    n = (n - 3) / 2;
    for (int i = 0; i <= 15810 && i <= n; ++i) {
        if (prime[i]) {
            h = h * x + 2 * i + 3;
            // (2 * i + 3) * (2 * i + 3)
            // = 4i^2 + 12i + 9
            // ->
            // (4i^2 + 12i + 9 - 3) / 2
            // = (4i^2 + 12i + 6) / 2
            // = 2i^2 + 6i + 3
            //
            // 2j + 3 -> 2j + 3 + 2(2i + 3)
            // (2j + 3 + 4i + 6 - 3) / 2
            // = (2j + 4i + 6) / 2
            // = j + 2i + 3
            int s = (i << 1) + 3;
            for (int j = 2 * i * i + 6 * i + 3; j <= n; j += s) {
                prime[j] = false;
            }
        }
    }

    for (int i = 15811; i <= n; ++i) {
        if (prime[i]) {
            h = h * x + 2 * i + 3;
        }
    }

    printf("%d\n", h);

    return 0;
}
