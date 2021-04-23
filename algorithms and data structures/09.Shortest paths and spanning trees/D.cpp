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

int const MAXN = 30'000 + 3;
int const INF = 1'000'000'000;

int n, m;
int d[MAXN];
vector<pair<int, int>> g[MAXN];

int main() {
    // file_raii pomello("data");

    fill(d, d + MAXN, INF);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        --v, --u;
        g[v].emplace_back(make_pair(u, w));
        g[u].emplace_back(make_pair(v, w));
    }

    set<pair<int, int>> q;
    q.insert(make_pair(0, 0));
    d[0] = 0;
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());
        for (auto const& edge : g[v]) {
            int to = edge.first;
            int w = edge.second;
            if (d[v] + w < d[to]) {
                q.erase(make_pair(d[to], to));
                d[to] = d[v] + w;
                q.insert(make_pair(d[to], to));
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", d[i]);
    } printf("\n");

    return 0;
}
