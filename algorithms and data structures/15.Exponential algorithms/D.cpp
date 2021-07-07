#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 2000 + 3;

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

int n, m, k;
int r;
vector<int> g[MAXN];
int degree[MAXN];
bool sensor[MAXN];

unordered_set<int> leaves;
set<pair<int, int>> verteces;

void ans_yes() {
    printf("Yes\n");
    for (int i = 0; i < n; ++i) {
        if (sensor[i]) {
            printf("%d ", i + 1);
        } else if (k > 0) {
            --k;
            printf("%d ", i + 1);
        }
    }
}

void ans_no() {
    printf("No\n");
}

void chd(int v, int dd) {
    if (degree[v] == 1) {
        leaves.erase(v);
        // printf("%d become leaf\n", v + 1);
    }
    verteces.erase(make_pair(-degree[v], v));

    degree[v] += dd;
    r += dd;

    verteces.insert(make_pair(-degree[v], v));
    if (degree[v] == 1) {
        leaves.insert(v);
        // printf("%d no more leaf\n", v + 1);
    }
}

void make_sensor(int v) {
    // printf("try to make %d sensor\n", v + 1);
    assert(!sensor[v] && k > 0);
    for (int const& to : g[v]) {
        if (!sensor[to]) {
            chd(v, -1);
            chd(to, -1);
        }
    }
    --k;
    sensor[v] = true;
    verteces.erase(make_pair(-degree[v], v));
    // printf("made %d sensor\n", v + 1);
}

void unmake_sensor(int v) {
    // printf("try to unmake %d sensor\n", v + 1);
    assert(sensor[v]);
    for (int const& to : g[v]) {
        if (!sensor[to]) {
            chd(v, 1);
            chd(to, 1);
        }
    }
    ++k;
    sensor[v] = false;
    verteces.insert(make_pair(-degree[v], v));
    // printf("unmade %d sensor\n", v + 1);
}

bool cover() {
    if (r == 0) {
        return true;
    } else if (k == 0) {
        return false;
    }
    int v;
    if (!leaves.empty()) {
        v = *leaves.begin();
        for (size_t i = 0; i < g[v].size(); ++i) {
            if (!sensor[g[v][i]]) {
                v = g[v][i];
                break;
            }
        }
        make_sensor(v);
        if (cover()) {
            return true;
        }
        unmake_sensor(v);
        return false;
    }
    v = verteces.begin()->second;
    make_sensor(v);
    if (cover()) {
        return true;
    }
    unmake_sensor(v);
    if (degree[v] > k || degree[v] == 2) {
        return false;
    }
    stack<int> sensored;
    for (int const& to : g[v]) {
        if (!sensor[to]) {
            make_sensor(to);
            sensored.push(to);
        }
    }
    if (cover()) {
        return true;
    }
    while (!sensored.empty()) {
        int to = sensored.top();
        sensored.pop();
        unmake_sensor(to);
    }
    return false;
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    r = 2 * m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        scanf("%d %d", &u, &v);
        ++degree[--u];
        ++degree[--v];
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    for (int i = 0; i < n; ++i) {
        verteces.insert(make_pair(-degree[i], i));
        if (degree[i] == 1) {
            leaves.insert(i);
        }
    }

    cover() ? ans_yes() : ans_no();

    return 0;
}
