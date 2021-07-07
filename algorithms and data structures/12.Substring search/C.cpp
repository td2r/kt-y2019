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

struct edge {
    int a;
    int b;
    int cap;
    int cost;
    int f;
    int i;

    edge()
    : a(0)
    , b(0)
    , cap(0)
    , cost(0)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b)
    : a(a)
    , b(b)
    , cap(0)
    , cost(0)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap)
    : a(a)
    , b(b)
    , cap(cap)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap, int cost)
    : a(a)
    , b(b)
    , cap(cap)
    , cost(cost)
    , f(0)
    , i(-1)
    {}

    edge(int a, int b, int cap, int cost, int i)
    : a(a)
    , b(b)
    , cap(cap)
    , cost(cost)
    , f(0)
    , i(i)
    {}

    void scan() {
        scanf("%d %d %d %d", &a, &b, &cap, &cost);
        --a, --b;
        f = 0;
        i = -1;
    }

    void print() const {
        printf("{ a=%d, b=%d, cap=%d, cost=%d, f=%d residual=%d}\n",
                a, b, cap, cost, f, residual());
    }

    int residual() const {
        return cap - f;
    }
};

struct task_t {
    int s;
    int f;
    int c;

    void scan() {
        scanf("%d %d %d", &s, &f, &c);
        f += s;
    }
};

int const MAXN = 1'000'000 + 3;

int n, m;
char s[MAXN];
int z[MAXN];
int p[MAXN];

void z_function() {
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            z[i] = min(z[i - l], r - i + 1);
        }
        while (i + z[i] < n && s[i + z[i]] == s[z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

void prefix_function() {
    p[0] = 0;
    for (int i = 1; i < n; ++i) {
        p[i] = p[i - 1];
        while (p[i] != 0 && s[p[i]] != s[i]) {
            p[i] = p[p[i] - 1];
        }
        if (s[p[i]] == s[i]) {
            ++p[i];
        }
    }
}

int main() {
    // file_raii pomello("data");

    scanf("%s\n", s);
    n = (int) strlen(s);

    prefix_function();

    for (int i = 0; i < n; ++i) {
        printf("%d ", p[i]);
    } printf("\n");

    return 0;
}
