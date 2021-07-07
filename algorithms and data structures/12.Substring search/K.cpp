#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        // freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        // fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first,
                 forwardIterator last,
                 string const& name = "",
                 string const& separator = ", ") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << separator << *first++;
    }
    cout << endl;
}

int read_uint() {
    char c;
    int res = 0;
    while ((c = (char) getchar()) != -1) {
        if (c == ' ' || c == '\n') {
            return res;
        }
        res = 10 * res + c - '0';
    }
    return res;
}

int const MAXN = 5'000 + 3;

int n;
char s[MAXN];
char t[MAXN];
int pi[MAXN];
int dp[MAXN][MAXN];

int main() {
    scanf("%s", t);
    n = (int) strlen(t);

    for (int j = 1; j <= n; ++j) {
        dp[j - 1][j] = 1;
        s[0] = t[j - 1];
        int pi_max = 0;
        for (int i = j - 2, k = 1; i >= 0; --i, ++k) {
            s[k] = t[i];
            pi[k] = pi[k - 1];
            while (pi[k] > 0 && s[pi[k]] != s[k]) {
                pi[k] = pi[pi[k] - 1];
            }
            if (s[pi[k]] == s[k]) {
                pi[k]++;
            }
            if (pi[k] > pi_max)
                pi_max = pi[k];
            dp[i][j] = dp[i][j - 1] + j - i - pi_max;
        }
    }

    int q;
    scanf("%d", &q);
    while (q--) {
        int l, r;
        scanf("%d %d", &l, &r);
        printf("%d\n", dp[l - 1][r]);
    }

    return 0;
}
