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

int const MAXN = 1'000'000 + 3;

int n;
int m;
char s1[2 * MAXN];
char s2[2 * MAXN];
int z1[2 * MAXN];
int z2[2 * MAXN];

void z_function(char* const s, int* z) {
    z[0] = n + m + 1;
    // z[0] = 0;
    for (int i = 1, l = 0, r = 0; i < n + m + 1; ++i) {
        if (i <= r) {
            z[i] = min(z[i - l], r - i + 1);
        }
        while (i + z[i] < n + m + 1 && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

int main() {
    scanf("%s\n", s1);
    m = (int) strlen(s1);
    s1[m] = '$';
    scanf("%s", s1 + m + 1);
    n = (int) strlen(s1) - m - 1;
    strcpy(s2, s1);
    reverse(s2, s2 + m);
    reverse(s2 + m + 1, s2 + n + m + 1);
    z_function(s1, z1);
    z_function(s2, z2);

    // print_array(s1, s1 + n + m + 1, "s1");
    // print_array(z1, z1 + n + m + 1, "z1");
    // print_array(s2, s2 + n + m + 1, "s2");
    // print_array(z2, z2 + n + m + 1, "z2");

    vector<int> ans;
    for (int i = m + 1; i <= n + 1; ++i) {
        if (z1[i] == m || z1[i] + z2[n + m - i + 2] == m - 1) {
            ans.emplace_back(i - m);
        }
    }

    printf("%zd\n", ans.size());
    for (int const& it : ans) {
        printf("%d ", it);
    } printf("\n");
    return 0;
}
