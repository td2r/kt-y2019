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
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
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

int const MAXN = 10 + 3;

int n;
int m;

vector<int> chromatic_poly(int k, vector<pair<int, int>> edges) {
    if (edges.empty()) {
        vector<int> result(n + 1);
        result[k] = 1;
        return result;
    }
    // remove edge vu
    int v = edges.back().first;
    int u = edges.back().second;
    edges.pop_back();
    vector<int> p1 = chromatic_poly(k, edges);
    // merge verteces v and u
    // u is now v
    for (auto &e : edges) {
        if (e.first == u) {
            e.first = v;
        }
        if (e.second == u) {
            e.second = v;
        }
        if (e.first > e.second) {
            swap(e.first, e.second);
        }
    }
    sort(edges.begin(), edges.end());
    edges.resize(unique(edges.begin(), edges.end()) - edges.begin());
    vector<int> p2 = chromatic_poly(k - 1, edges);
    for (int i = 0; i <= n; ++i) {
        p1[i] -= p2[i];
    }
    return p1;
}

int main() {
    // file_raii pomello("data");

    scanf("%d %d", &n, &m);
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &edges[i].first, &edges[i].second);
    }

    vector<int> p = chromatic_poly(n, edges);
    printf("%d\n", n);
    for (int i = n; i >= 0; --i) {
        printf("%d ", p[i]);
    } printf("\n");

    return 0;
}
