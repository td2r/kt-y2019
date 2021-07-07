#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 40 + 2;

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

bool bi_dfs(int v, int c = 1) {
    color[v] = c;
    int nc = c == 1 ? 2 : 1;
    for (int const& to : g[v]) {
        if (color[to] == 0) {
            if (!bi_dfs(to, nc)) {
                return false;
            }
        } else {
            if (color[to] == color[v]) {
                return false;
            }
        }
    }
    return true;
}

bool bicolor() {
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && !bi_dfs(i)) {
            return false;
        }
    }
    return true;
}

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

    double start = clock();

    scanf("%d %d", &n, &m);
    int degree[MAXN];
    memset(degree, 0, sizeof(int) * MAXN);
    vector<pair<int, int>> edges(m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &edges[i].first, &edges[i].second);
        ++degree[--edges[i].first];
        ++degree[--edges[i].second];
    }

    for (int i = 0; i < n; ++i) {
        if (degree[i] > 3) {
            printf("NO\n");
            return 0;
        }
    }

    n = m;
    m = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (edges[i].first == edges[j].first
                || edges[i].first == edges[j].second
                || edges[i].second == edges[j].first
                || edges[i].second == edges[j].second) {
                g[i].emplace_back(j);
                g[j].emplace_back(i);
                ++m;
            }
        }
    }

    if (n == 1) {
        printf("YES\n1\n");
        return 0;
    }

    if (bicolor()) {
        printf("YES\n");
        for (int i = 0; i < n; ++i) {
            printf("%d ", color[i]);
        } printf("\n");
        return 0;
    }

    vector<int> verteces(n);
    iota(verteces.begin(), verteces.end(), 0);
    while ((clock() - start) / CLOCKS_PER_SEC < 0.95) {
        random_shuffle(verteces.begin(), verteces.end());
        random_shuffle(verteces.begin(), verteces.end());
        random_shuffle(verteces.begin(), verteces.end());
        random_shuffle(verteces.begin(), verteces.end());
        bool success = true;
        memset(color, 0, sizeof(int) * n);
        for (int const& v : verteces) {
            if (color[v] == 0 && !dfs(v)) {
                success = false;
                break;
            }
        }
        if (success) {
            printf("YES\n");
            for (int i = 0; i < n; ++i) {
                printf("%d ", color[i]);
            } printf("\n");
            return 0;
        }
    }

    printf("NO\n");

    return 0;
}
