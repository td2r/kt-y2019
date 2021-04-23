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
vector<int> path;
vector<int> cycle;

void make_path() {
    for (int i = 0; i < n; ++i) {
        int j = 0;
        while (j < i && g[path[j]][i]) {
            ++j;
        }
        if (j == i) path.emplace_back(i);
        else path.insert(path.begin() + j, i);
    }
}

int longest_cycle() {
    for (int k = n - 1; k >= 0; --k) {
        if (g[path[k]][path[0]]) {
            return k;
        }
    }
    assert(false);
}

int inp(int j) {
    for (int i = 0; i < (int) cycle.size(); ++i) {
        if (g[path[j]][cycle[i]]) {
            return i;
        }
    }
    return -1;
}

void make_cycle() {
    int k = longest_cycle() + 1;
    for (int i = 0; i < k; ++i) {
        cycle.emplace_back(path[i]);
    }
    for (; k < n; ++k) {
        for (int j = k; j < n; ++j) {
            int i = inp(j);
            if (i == -1) {
                continue;
            }
            if (i != 0) {
                cycle.insert(cycle.begin() + i, path.begin() + k, path.begin() + j + 1);
            }
            k = j;
            break;
        }
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%d\n", &n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            char ch;
            scanf("%c", &ch);
            g[i][j] = (int) (ch - '0');
            g[j][i] = (g[i][j] ^ 1);
        }
        scanf("\n");
    }

    make_path();
    for (int i = 0; i + 1 < n; ++i) {
        assert(g[path[i]][path[i + 1]]);
    }
    make_cycle();

    for (int const& v : cycle) {
        printf("%d ", v + 1);
    } printf("\n");

    return 0;
}
