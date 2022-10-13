#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

template<typename forwIterator>
ll dist(forwIterator begin, forwIterator end) {
    ll d = 0;
    ll pd = 0;
    forwIterator prev = begin;
    ++begin;
    ll i = 1;
    while (begin != end) {
        pd += i * (begin->second - prev->second);
        d += pd;
        ++prev;
        ++begin;
        ++i;
    }
    return d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int k;
    cin >> k;
    int n;
    cin >> n;
    vector<pair<int, int>> obj(n);
    for (int i = 0; i < n; ++i) {
        cin >> obj[i].second >> obj[i].first;
    }
    sort(obj.begin(), obj.end());
    ll in_d = 0;
    for (int l = 0, r = 0; l < n; l = r) {
        while (r < n && obj[l].first == obj[r].first)
            ++r;
        in_d += dist(obj.begin() + l, obj.begin() + r);
    }
    sort(obj.begin(), obj.end(), [](pair<int, int> const& p1, pair<int, int> const& p2) {
        return p1.second < p2.second;
    });
    ll out_d = dist(obj.begin(), obj.end()) - in_d;
    cout << 2 * in_d << endl;
    cout << 2 * out_d << endl;
    return 0;
}
