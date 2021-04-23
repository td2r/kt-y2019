#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        fclose(stdout);
        fclose(stdin);
    }
};

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

int const MAXN = 5'000 + 3;
int const INF = 1'000'000'00;

int n;
int x[MAXN];
int y[MAXN];
int me[MAXN];
int mw[MAXN];

int dist(int i, int j) {
    return (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]);
}

int main() {
    // file_raii pomello("data");

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &x[i], &y[i]);
        me[i] = 0;
        mw[i] = dist(0, i);
    }

    double ans = 0;
    me[0] = mw[0] = -1;
    for (int i = 0; i < n - 1; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j) {
            if (me[j] != -1 && (v == -1 || mw[j] < mw[v])) {
                v = j;
            }
        }
        ans += sqrt(mw[v]);
        me[v] = mw[v] = -1;
        for (int j = 0; j < n; ++j) {
            if (me[j] != -1 && dist(j, v) < mw[j]) {
                mw[j] = dist(j, v);
            }
        }
    }

    printf("%.228lf\n", ans);

    return 0;
}
