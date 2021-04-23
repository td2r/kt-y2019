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

int const MAXN = 20 + 3;

ull d;
int k;
ull pow10[MAXN];
unordered_map<ull, vector<ull>> g;
unordered_set<ull> vertex;
vector<ull> euler;

string to_code(ull v) {
    string code;
    while (v) {
        code.push_back((char) (v % 10 + '0'));
        v /= 10;
    }
    while ((int) code.length() < k - 1) {
        code.push_back('0');
    }
    reverse(code.begin(), code.end());
    return code;
}

void generate_vertex(ull v, int step) {
    if (step == k - 1) {
        vertex.insert(v);
    } else {
        for (ull i = 0; i < d; ++i) {
            generate_vertex(10ULL * v + i, step + 1);
        }
    }
}

void find_euler() {
    stack<ull> st;
    st.push(0);
    while (!st.empty()) {
        ull v = st.top();
        if (g[v].empty()) {
            euler.emplace_back(v);
            st.pop();
        } else {
            ull to = g[v].back();
            g[v].pop_back();
            st.push(to);
        }
    }
    reverse(euler.begin(), euler.end());
}

int main() {
    // file_raii pomello("data");

    pow10[0] = 1;
    for (int i = 1; i < 20; ++i) {
        pow10[i] = pow10[i - 1] * 10;
    }

    scanf("%llu %d", &d, &k);

    if (k == 1) {
        for (ull i = 0; i < d; ++i) {
            printf("%llu", i);
        } printf("\n");
        return 0;
    }

    generate_vertex(0, 0);

    for (ull const& v : vertex) {
        ull u = 10 * (v % pow10[k - 2]);
        for (ull i = 0; i < d; ++i) {
            if (true) {
                g[v].emplace_back(u);
            }
            u++;
        }
    }

    find_euler();

    string ans = to_code(euler[0]);

    for (size_t i = 1; i < euler.size(); ++i) {
        ans.push_back((char) (euler[i] % 10 + '0'));
    }

    printf("%s\n", ans.c_str());

    return 0;
}
