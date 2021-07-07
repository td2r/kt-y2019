#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 1'000'000 + 3;

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
int d[MAXN];

int main() {
    memset(d, 255, sizeof(int) * MAXN);
    for (int i = 2; i < MAXN; ++i) {
        if (d[i] == -1) {
            for (int j = i; j < MAXN; j += i) {
                d[j] = i;
            }
        }
    }

    scanf("%d", &n);
    stack<int> st;
    for (int i = 0; i < n; ++i) {
        int a;
        scanf("%d", &a);
        while (a != 1) {
            st.push(d[a]);
            a /= d[a];
        }
        while (!st.empty()) {
            printf("%d ", st.top());
            st.pop();
        } printf("\n");
    }

    return 0;
}
