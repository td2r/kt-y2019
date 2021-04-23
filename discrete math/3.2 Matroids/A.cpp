#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


template <class forwardIterator>
void print_array(forwardIterator first, forwardIterator last, string const& name = "") {
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

int read_uint() {
    char c;
    int res = 0;
    while (c = getchar()) {
        if (c == ' ' || c == '\n')
            return res;
        res = 10 * res + (c - '0');
    }
    return res;
}

int const MAXN = 100000 + 3;
int const MAXM = (1 << MAXN) + 3;

int n;
ll ans;
pair<int, int> a[MAXN];
map<int, int> parent;

ostream& operator << (ostream& os, pair<int, int> const& p) {
    os << '(' << p.first << ',' << p.second << ')';
    return os;
}

int find_set(int v) {
    return parent[v] == v ? v : parent[v] = find_set(parent[v]);
}

int main() {
    freopen("schedule.in", "r", stdin);
    freopen("schedule.out", "w", stdout);
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &a[i].second, &a[i].first);
        a[i].second--;
        ans += a[i].first;
    }
    sort(a, a + n);
    reverse(a, a + n);
    for (int i = 0; i < n; ++i) {
        parent.insert({ a[i].second, a[i].second });
    }
    for (int i = 0; i < n; ++i) {
        int t = find_set(a[i].second);
        if (t != -1) {
            ans -= a[i].first;
            parent[t] = t - 1;
            if (parent.find(t - 1) == parent.end()) {
                parent.insert({ t - 1, t - 1 });
            }
            // printf("task d = %d, w = %d must be done.\n", a[i].second, a[i].first);
            // print_array(parent.begin(), parent.end(), "parent");
        }
    }
    printf("%lld\n", ans);
    fclose(stdin);
    fclose(stdout);
    return 0;
}
