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

int n;
int adj[MAXN][MAXN];
vector<int> cycle;
bool in_cycle[MAXN];
bool adj_to_verteces_in_cycle[MAXN];

void add_vertex_in_cycle(int v) {
    in_cycle[v] = true;
    for (int u = 0; u < n; ++u) {
        if (adj[v][u]) {
            adj_to_verteces_in_cycle[u] = true;
        }
    }
}

int adj_not_in_cycle(int u) {
    for (int v = 0; v < n; ++v) {
        if (adj[v][u] && !in_cycle[v]) {
            return v;
        }
    }
    return -1;
}

void turn_to_cycle() {
    for (size_t i = 1; i < cycle.size(); ++i) {
        if (adj[cycle[0]][cycle[i]] && adj[cycle[i - 1]][cycle.back()]) {
            stack<int> st;
            for (size_t j = 0; j < i; ++j) {
                st.push(cycle[j]);
            }
            cycle.erase(cycle.begin(), cycle.begin() + i);
            while (!st.empty()) {
                cycle.emplace_back(st.top());
                st.pop();
            }
            return;
        }
    }
    assert(false);
}

void expand_cycle() {
    int v = -1;
    for (int i = 0; i < n; ++i) {
        if (!in_cycle[i] && adj_to_verteces_in_cycle[i]) {
            v = i;
            break;
        }
    }
    assert(v > 0);
    for (size_t i = 0; i < cycle.size(); ++i) {
        int u = cycle[i];
        if (adj[v][u]) {
            queue<int> q;
            for (size_t j = 0; j < i; ++j) {
                q.push(cycle[j]);
            }
            cycle.erase(cycle.begin(), cycle.begin() + i);
            cycle.insert(cycle.begin(), v);
            while (!q.empty()) {
                cycle.emplace_back(q.front());
                q.pop();
            }
            add_vertex_in_cycle(v);
            return;
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
            adj[i][j] = adj[j][i] = (int) (ch - '0');
        }
        scanf("\n");
    }

    cycle.emplace_back(0);
    add_vertex_in_cycle(0);
    while ((int) cycle.size() < n) {
        int v = adj_not_in_cycle(cycle[0]);
        if (v != -1) {
            cycle.insert(cycle.begin(), v);
            add_vertex_in_cycle(v);
            continue;
        }
        v = adj_not_in_cycle(cycle.back());
        if (v != -1) {
            cycle.emplace_back(v);
            add_vertex_in_cycle(v);
            continue;
        }
        turn_to_cycle();
        expand_cycle();
    }
    turn_to_cycle();

    for (int const& i : cycle) {
        printf("%d ", i + 1);
    } printf("\n");

    return 0;
}
