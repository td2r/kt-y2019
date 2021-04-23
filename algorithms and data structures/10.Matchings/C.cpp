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

int const MAXN = 4'000 + 3;

int n, m, k = 0;
vector<int> g[2 * MAXN];
int used[2 * MAXN];
vector<int> vertex_cover1;
vector<int> vertex_cover2;

void dfs(int v) {
    used[v] = 1;
    for (int to : g[v]) {
        if (used[to] != 1) {
            dfs(to);
        }
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; ++i) {
        int Ki;
        scanf("%d", &Ki);
        while (Ki--) {
            int j;
            scanf("%d", &j);
            g[i].emplace_back(--j + n);
        }
    }
    for (int i = 0; i < n; ++i) {
        int j;
        scanf("%d", &j);
        if (j) {
            --j;
            ++k;
            g[i].erase(find(g[i].begin(), g[i].end(), j + n));
            g[j + n].emplace_back(i);
            used[i] = -1;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            dfs(i);
        }
    }

    printf("%d\n", k);
    for (int i = 0; i < n; ++i) {
        if (used[i] != 1) {
            vertex_cover1.emplace_back(i + 1);
        }
    }
    printf("%zd", vertex_cover1.size());
    for (int const& i : vertex_cover1) {
        printf(" %d", i);
    } printf("\n");
    for (int i = n; i < n + m; ++i) {
        if (used[i] == 1) {
            vertex_cover2.emplace_back(i - n + 1);
        }
    }
    printf("%zd", vertex_cover2.size());
    for (int const& i : vertex_cover2) {
        printf(" %d", i);
    } printf("\n");

    return 0;
}
