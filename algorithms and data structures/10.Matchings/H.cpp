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

int n, m;
int mt[MAXN];
bool odd[MAXN];
vector<int> ans;
multiset<int> g[MAXN];

void showGraph() {
    for (int i = 0; i < n; ++i) {
        printf("g[%d] :", i + 1);
        for (int const& j : g[i]) {
            printf(" %d", j + 1);
        } printf("\n");
    }
}

void euler(int s) {
    stack<int> st;
    st.push(s);
    while (!st.empty()) {
        int v = st.top();
        if (g[v].empty()) {
            ans.emplace_back(v);
            st.pop();
        } else {
            int to = *(g[v].begin());
            g[v].erase(g[v].begin());
            g[to].erase(g[to].find(v));
            st.push(to);
        }
    }
}

int prepare() {
    vector<int> oddVertex;
    for (int i = 0; i < n; ++i) {
        if (g[i].size() % 2 == 1) {
            odd[i] = true;
            oddVertex.emplace_back(i);
        }
    }
    for (size_t i = 2; i < oddVertex.size(); i += 2) {
        int v = oddVertex[i];
        int u = oddVertex[i + 1];
        g[v].insert(u);
        g[u].insert(v);
        mt[v] = u;
        mt[u] = v;
    }
    printf("%d\n", max(1, (int) oddVertex.size() / 2));
    return oddVertex.empty() ? 0 : oddVertex[0];
}

int main() {
    // file_raii pomello("data");

    memset(mt, 255, sizeof(int) * MAXN);

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].insert(u);
        g[u].insert(v);
    }


    euler(prepare());

    for (size_t i = 0; i < ans.size() - 1; ++i) {
        printf("%d ", ans[i] + 1);
        if (mt[ans[i]] == ans[i + 1]) {
            assert(mt[ans[i + 1]] == ans[i]);
            printf("\n");
            mt[ans[i]] = mt[ans[i + 1]] = -1;
        }
    }
    printf("%d\n", ans.back() + 1);

    return 0;
}
