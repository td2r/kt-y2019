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

int const MAXN = 100 + 3;

int n;
int adj[MAXN][MAXN];
deque<int> q;

int get_range() {
    for (int k = 2; k + 1 < n; ++k) {
        if (adj[q[0]][q[k]] && adj[q[1]][q[k + 1]]) {
            return k + 1;
        }
    }
    return -1;
}

bool solve() {
    for (int i = 0; i < n * (n - 1); ++i) {
        if (!adj[q[0]][q[1]]) {
            int k = get_range();
            if (k == -1)
                return false;
            reverse(q.begin() + 1, q.begin() + k);
        }
        q.push_back(q.front());
        q.pop_front();
    }
    if (!adj[q.front()][q.back()]) {
        return false;
    }
    for (int i = 1; i < n; ++i) {
        if (!adj[q[i - 1]][q[i]]) {
            return false;
        }
    }
    return true;
}

int main() {
    // file_raii pomello("data");

    scanf("%d\n", &n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            char ch;
            scanf("%c", &ch);
            adj[i][j] = adj[j][i] = (int) (ch - '0');
        }
        scanf("\n");
    }

    q.resize(n);
    do {
        iota(q.begin(), q.end(), 0);
        random_shuffle(q.begin(), q.end());
        random_shuffle(q.begin(), q.end());
        random_shuffle(q.begin(), q.end());
    } while (!solve());

    for (int const& v : q) {
        printf("%d ", v + 1);
    } printf("\n");

    return 0;
}
