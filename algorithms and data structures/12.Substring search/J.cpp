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

int const MAXN = 10'000 + 3;

int n;
char s[MAXN];
int pi[MAXN];

int prefix_function(int m) {
    int ret = 0;
    for (int i = 1; i < m; ++i) {
        pi[i] = pi[i - 1];
        while (pi[i] > 0 && s[pi[i]] != s[i]) {
            pi[i] = pi[pi[i] - 1];
        }
        if (s[pi[i]] == s[i]) {
            pi[i]++;
        }
        ret = max(ret, pi[i]);
    }
    return ret;
}

int main() {
    scanf("%s", s);
    n = (int) strlen(s);

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        reverse(s, s + i);
        ans += i - prefix_function(i);
        reverse(s, s + i);
        printf("%d\n", ans);
    }

    return 0;
}
