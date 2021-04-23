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

int const MAXN = 20'000 + 3;
int const MAXM = 100'000 + 3;

int n, m;
struct edge {
    int v;
    int u;
    int w;
    bool operator<(edge const& other) {
        if (other.w != w) {
            return w < other.w;
        } else if (other.v != v) {
            return v < other.v;
        } else {
            return u < other.u;
        }
    }
} e[MAXM];
int parent[MAXN];
int rnk[MAXN];

int find_set(int v) {
    return (parent[v] == v) ? v : parent[v] = find_set(parent[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rnk[a] < rnk[b])
            swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b])
            ++rnk[a];
    }
}

int main() {
    // file_raii pomello("data");

    iota(parent, parent + MAXN, 0);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &e[i].v, &e[i].u, &e[i].w);
        --e[i].v;
        --e[i].u;
    }

    int ans = 0;
    sort(e, e + m);
    for (int i = 0; i < m; ++i) {
        if (find_set(e[i].v) != find_set(e[i].u)) {
            ans += e[i].w;
            union_sets(e[i].v, e[i].u);
        }
    }

    printf("%d\n", ans);

    return 0;
}
