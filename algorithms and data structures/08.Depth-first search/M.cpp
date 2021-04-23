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

int const MAXN = 26 + 3;
int const MAXM =  1'000 + 3;

int const n = 26;
int m;
int value[MAXN];
bool used[MAXN];
vector<int> g[MAXN];
vector<int> topsort;

size_t l[MAXM];
char s[MAXM][21];
bool can_be_zero[MAXN];

void dfs(int v) {
    used[v] = true;
    for (int const& to : g[v]) {
        if (!used[to]) {
            dfs(to);
        }
    }
    topsort.emplace_back(v);
}

int main() {
    // file_raii xxxxxxxxxxxxxxxxxxxxxx("data");

    memset(value, 255, sizeof(int) * MAXN);
    fill(can_be_zero, can_be_zero + MAXN, true);

    scanf("%d\n", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%s\n", s[i]);
        l[i] = strlen(s[i]);
        if (l[i] > 1) {
            can_be_zero[s[i][0] - 'a'] = false;
        }
        for (int j = 0; j < i; ++j) {
            if (l[j] > l[i]) {
                printf("No\n");
                return 0;
            } else if (l[j] == l[i]) {
                size_t k = 0;
                while (k < l[i] && s[j][k] == s[i][k]) {
                    ++k;
                }
                if (k == l[i]) {
                    printf("No\n");
                    return 0;
                }
                g[s[i][k] - 'a'].emplace_back(s[j][k] - 'a');
                can_be_zero[s[i][k] - 'a'] = false;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i);
        }
    }

    int min_val = 0;
    for (int const& v : topsort) {
        if (can_be_zero[v]) {
            value[v] = min_val++;
            break;
        }
    }
    if (min_val == 0) {
        printf("No\n");
        return 0;
    }
    for (int const& v : topsort) {
        if (value[v] == -1) {
            value[v] = min_val++;
        }
    }
    for (int v = 0; v < n; ++v) {
        for (int const& to : g[v]) {
            if (value[v] < value[to]) {
                printf("No\n");
                return 0;
            }
        }
    }
    printf("Yes\n");
    for (int i = 0; i < n; ++i) {
        printf("%d ", value[i]);
    } printf("\n");

    if (DEBUG) {
        printf("topsort: ");
        for (int const& i : topsort) {
            printf("%d ", i);
        } printf("\n");
        printf("can_be_zero[a] = %s\n", can_be_zero[0] ? "true" : "false");
    }

    return 0;
}
