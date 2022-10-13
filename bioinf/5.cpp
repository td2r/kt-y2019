#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

// 2 * i     -- head of vertex i
// 2 * i + 1 -- tail of vertex i
struct breakpoint_graph_t {
    vector<int> nxt;

    breakpoint_graph_t() : nxt(2, 0) {}

    breakpoint_graph_t(string const& line) {
        vector<string> strs;
        int i = 0;
        int j = 0;
        while (i < line.length()) {
            while (i < line.length() && line[i]  != '(')
                ++i;
            if (i == line.length())
                break;
            j = i;
            while (j < line.length() && line[j] != ')')
                ++j;
            strs.push_back(line.substr(i + 1, j - i - 1));
            i = j;
        }
        vector<vector<int>> cycles(strs.size());
        for (i = 0; i < strs.size(); ++i) {
            int value;
            stringstream ss(strs[i]);
            while (ss >> value) {
                cycles[i].push_back(value);
            }
        }
        int max_value = -1;
        for (auto const& cycle : cycles) {
            for (auto const& it : cycle) {
                max_value = max(max_value, abs(it));
            }
        }
        nxt.resize(2 * (max_value + 1), 0);
        for (auto const& cycle : cycles) {
            for (i = 0; i < cycle.size() - 1; ++i) {
                int x = 2 * abs(cycle[i]) + (cycle[i] < 0);
                int y = 2 * abs(cycle[i + 1]) + (cycle[i + 1] > 0);
                nxt[x] = y;
                nxt[y] = x;
            }
            int x = 2 * abs(cycle.back()) + (cycle.back() < 0);
            int y = 2 * abs(cycle[0]) + (cycle[0] > 0);
            nxt[x] = y;
            nxt[y] = x;
        }
    }

    void print_cycle(int s, vector<bool>& used) const {
        cout << "(";
        used[s] = true;
        int v = 2 * s;
        cout << "+" << s;
        while (nxt[v] / 2 != s) {
            cout << " " << ((nxt[v] & 1) ? "+" : "-") << nxt[v] / 2;
            v = (nxt[v] ^ 1);
            used[v / 2] = true;
        }
        cout << ")";
    }

    void print() const {
        vector<bool> used(nxt.size() / 2, false);
        used[0] = true;
        for (int i = 1; i < used.size(); ++i) {
            if (!used[i]) {
                print_cycle(i, used);
            }
        }
        cout << endl;
    }
};

bool double_break(breakpoint_graph_t& g1, breakpoint_graph_t const& g2, int i) {
    int j = g1.nxt[i];
    int k = g2.nxt[j];
    if (i == k)
        return false;
    int t = g1.nxt[k];
    g1.nxt[i] = t;
    g1.nxt[t] = i;
    g1.nxt[j] = k;
    g1.nxt[k] = j;
    return true;
}

int main() {
    string line;
    getline(cin, line);
    breakpoint_graph_t g1(line);
    getline(cin, line);
    breakpoint_graph_t g2(line);

    g1.print();
    while (true) {
        bool changed = false;
        for (int i = 2; i < g1.nxt.size(); ++i) {
            if (double_break(g1, g2, i)) {
                g1.print();
                changed = true;
                break;
            }
        }
        if (!changed)
            break;
    }

    return 0;
}
