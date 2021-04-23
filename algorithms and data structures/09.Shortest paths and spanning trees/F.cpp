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

int const MAXN = 250 + 3;
int const INF = 1'000'000'00;

int n;
int d[MAXN][MAXN];
int p[MAXN][MAXN];

int do_floyd() {
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (d[i][i] < 0) {
                return i;
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (-INF < d[i][k] && d[i][k] < INF &&
                        -INF < d[k][j] && d[k][j] < INF &&
                        d[i][k] + d[k][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                    p[i][j] = k;
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (d[i][i] < 0) {
            return i;
        }
    }
    return -1;
}

void make_path(int v, int u, vector<int> &vec) {
    if (p[v][u] != -1) {
        make_path(v, p[v][u], vec);
        make_path(p[v][u], u, vec);
    } else {
        vec.emplace_back(v);
    }
}

int main() {
    // file_raii pomello("data");

    memset(p, 255, sizeof(int) * MAXN * MAXN);

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", &d[i][j]);
        }
    }

    for (int i = 0; i < n; ++i) {
        d[i][i] = min(d[i][i], 0);
    }

    int v = do_floyd();
    if (v == -1) {
        printf("NO\n");
        return 0;
    }
    vector<int> vec;
    make_path(v, v, vec);
    vec.emplace_back(v);
    printf("YES\n");
    printf("%d\n", (int)vec.size());
    for (int const& i : vec) {
        printf("%d ", i + 1);
    } printf("\n");

    return 0;
}
