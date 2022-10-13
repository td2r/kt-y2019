#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double real_t;

void from_input(int k, int d, unordered_map<string, deque<string>>& g) {
    string s;
    while (cin >> s) {
        string v = s.substr(0, k - 1) + "|" + s.substr(k + 1, k - 1);
        string u = s.substr(1, k - 1) + "|" + s.substr(k + 2, k - 1);
        if (g.find(v) == g.end())
            g.insert(make_pair(v, deque<string>()));
        g[v].push_back(u);
    }
}

string start_vertex(unordered_map<string, deque<string>> const& g) {
    unordered_map<string, pair<int, int>> degree;
    for (auto const& it : g) {
        degree.insert(make_pair(it.first, make_pair(0, 0)));
    }
    for (auto const& it : g) {
        auto const& v = it.first;
        for (auto const& u : it.second) {
            ++degree[v].first;
            ++degree[u].second;
        }
    }
    for (auto const& it : degree) {
        auto const& deg = it.second;
        if (deg.first > deg.second) {
            assert(deg.first == deg.second + 1);
            return it.first;
        }
    }
    return degree.begin()->first;
}

int main() {
    int k;
    int d;
    cin >> k >> d;
    assert(k > 1);
    unordered_map<string, deque<string>> g; // de brujin graph
    from_input(k, d, g);

    // cout << "de brujin graph" << endl;
    // for (auto const& it : g) {
    //     cout << it.first << " -> {";
    //     for (auto const& v : it.second) {
    //         cout << ' ' << v;
    //     }
    //     cout << " }" << endl;
    // }

    string s = start_vertex(g);
    // cout << "start vertex: " << s << endl;
    stack<string> st;
    st.push(s);
    vector<string> path;
    while (!st.empty()) {
        auto const& v = st.top();
        if (!g[v].empty()) {
            st.push(g[v][0]);
            g[v].pop_front();
        } else {
            path.push_back(v);
            st.pop();
        }
    }
    reverse(path.begin(), path.end());
    cout << path[0].substr(0, k - 1);
    for (size_t i = 1; i < path.size(); ++i) {
        cout << path[i][k - 2];
    }
    size_t i = path.size() - 1 - d - 1;
    cout << path[i].substr(k, k - 1);
    while (++i < path.size()) {
        cout << path[i].back();
    }
    cout << endl;

    return 0;
}
