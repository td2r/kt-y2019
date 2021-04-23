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

int const MAXN = 1'000 + 3;

int n, m;
multiset<int> g[MAXN];
vector<int> ans;

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
            if (g[to].find(v) == g[to].end()) {
                printf("pomello\n");
            }
            g[to].erase(g[to].find(v));
            st.push(to);
        }
    }
}

int main() {
    // file_raii pomello("data");

    int s = 0;

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int mi;
        scanf("%d", &mi);
        if (mi % 2) {
            s = i;
        }
        while (mi--) {
            int j;
            scanf("%d", &j);
            g[i].insert(--j);
        }
    }

    euler(s);

    printf("%zd\n", ans.size() - 1);
    for (int const& i : ans) {
        printf("%d ", i + 1);
    } printf("\n");

    return 0;
}
