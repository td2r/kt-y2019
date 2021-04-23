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

int const MAXN = 1'024 + 3;

int n;
int g[MAXN][MAXN];
int a[MAXN];

struct cmp {
    bool operator()(int i, int j) {
        if (g[i][j] == -1) {
            char s[4];
            printf("1 %d %d\n", i, j);
            fflush(stdout);
            scanf("%s", s);
            g[i][j] = (int) (s[0] == 'Y');
            g[j][i] = (g[i][j] ^ 1);
        }
        return g[i][j];
    }
};

int main() {
    // file_raii pomello("data");

    scanf("%d", &n);
    iota(a + 1, a + n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        memset(g[i] + 1, 255, sizeof(int) * n);
        g[i][0] = 1;
        g[i][i] = 0;
    }

    stable_sort(a + 1, a + n + 1, cmp());

    printf("0");
    for (int i = 1; i <= n; ++i) {
        printf(" %d", a[i]);
    } printf("\n");
    fflush(stdout);

    return 0;
}
