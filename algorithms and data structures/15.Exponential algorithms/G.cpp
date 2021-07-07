#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 50 + 1;

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

int n, m;
vector<int> g[MAXN];
int color[MAXN];

bool banned[4];

bool dfs(int v) {
    banned[1] = banned[2] = banned[3] = false;
    for (int const& to : g[v]) {
        banned[color[to]] = true;
    }
    if (!banned[1] && banned[2] && banned[3]) {
        color[v] = 1;
    } else if (banned[1] && !banned[2] && banned[3]) {
        color[v] = 2;
    } else if (banned[1] && banned[2] && !banned[3]) {
        color[v] = 3;
    } else if (banned[1] && banned[2] && banned[3]) {
        return false;
    } else {
        while (color[v] == 0 || banned[color[v]]) {
            color[v] = (rand() % 3) + 1;
        }
    }
    for (int const& to : g[v]) {
        if (color[to] == 0 && !dfs(to)) {
            return false;
        }
    }
    return true;
}

int main() {
    srand(random_device()());

    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        int v, u;
        scanf("%d %d", &v, &u);
        --v, --u;
        g[v].emplace_back(u);
        g[u].emplace_back(v);
    }

    vector<int> verteces(n);
    iota(verteces.begin(), verteces.end(), 0);
    while (true) {
        random_shuffle(verteces.begin(), verteces.end());
        random_shuffle(verteces.begin(), verteces.end());
        random_shuffle(verteces.begin(), verteces.end());
        bool success = true;
        for (int const& v : verteces) {
            if (color[v] == 0 && !dfs(v)) {
                success = false;
                break;
            }
        }
        if (success) {
            for (int i = 0; i < n; ++i) {
                printf("%d ", color[i]);
            } printf("\n");
            return 0;
        }
        memset(color, 0, sizeof(int) * n);
    }

    return 0;
}
