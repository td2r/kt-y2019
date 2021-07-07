#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        // freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        // fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first,
                 forwardIterator last,
                 string const& name = "",
                 string const& separator = ", ") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << separator << *first++;
    }
    cout << endl;
}

int read_uint() {
    char c;
    int res = 0;
    while ((c = (char) getchar()) != -1) {
        if (c == ' ' || c == '\n') {
            return res;
        }
        res = 10 * res + c - '0';
    }
    return res;
}

int const MAXN = 100 + 3;
int const ALPHA = 'z' - 'a' + 1;
int const mod = 10'000;

struct node_t {
    bool term;
    int parent;
    int parent_ch;
    int suffix;
    int trans[ALPHA];

    node_t()
    : term(false)
    , parent(-1)
    , suffix(-1)
    {
        memset(trans, 255, sizeof(int) * ALPHA);
    }
};

int l;
int n = 2;
char s[13];
node_t node[MAXN];
vector<vector<int>> a;

void build_suffix_link(int v) {
    int x = node[v].parent_ch;
    int u = node[node[v].parent].suffix;
    while (node[u].trans[x] == -1) {
        u = node[u].suffix;
    }
    u = node[u].trans[x];
    node[v].suffix = u;
    node[v].term |= node[u].term;
}

void build_suffix_links() {
    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int x = 0; x < ALPHA; ++x) {
            int u = node[v].trans[x];
            if (u != -1) {
                build_suffix_link(u);
                q.push(u);
            }
        }
    }
}

int get_next_state(int v, int x) {
    while (node[v].trans[x] == -1) {
        v = node[v].suffix;
    }
    return node[v].trans[x];
}

void mult(vector<vector<int>>& a, vector<vector<int>> const& b) {
    vector<vector<int>> c(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % mod;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = c[i][j];
        }
    }
}

vector<vector<int>> raise_power(int p) {
    vector<vector<int>> result(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        result[i][i] = 1;
    while (p) {
        if (p & 1) {
            mult(result, a);
        }
        mult(a, a);
        p >>= 1;
    }
    return result;
}

int int_power(int p) {
    int res = 1;
    int a = ALPHA;
    while (p) {
        if (p & 1) {
            res = (res * a) % mod;
        }
        a = (a * a) % mod;
        p >>= 1;
    }
    return res;
}

int main() {
    fill(node[0].trans, node[0].trans + ALPHA, 1);
    node[1].suffix = 0;

    int m;
    scanf("%d\n", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%s\n", s);
        int v = 1;
        for (char* c = s; *c; ++c) {
            int x = *c - 'a';
            if (node[v].trans[x] == -1) {
                node[n].parent = v;
                node[n].parent_ch = x;
                node[v].trans[x] = n++;
            }
            v = node[v].trans[x];
        }
        node[v].term = true;
    }
    scanf("%d", &l);

    build_suffix_links();

    a.resize(n, vector<int>(n, 0));
    for (int v = 1; v < n; ++v) {
        if (node[v].term)
            continue;
        for (int x = 0; x < ALPHA; ++x) {
            int u = get_next_state(v, x);
            if (!node[u].term) {
                a[v - 1][u - 1]++;
            }
        }
    }

    vector<vector<int>> b = raise_power(l);

    int ans = int_power(l);
    for (int i = 0; i < n; ++i) {
        ans = (ans - b[0][i] + mod) % mod;
    }
    printf("%d\n", ans);

    return 0;
}
