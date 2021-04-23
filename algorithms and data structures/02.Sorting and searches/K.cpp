#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <set>
#define mp make_pair
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


ull n = (ull)1e18, m;


ull request(ull i) {
    printf("? %llu\n", i % (n - m) + 1);
    fflush(stdout);
    ull val;
    scanf("%llu", &val);
    return val;
}

void answer(ull i) {
    printf("! %llu\n", i % (n - m) + 1);
    fflush(stdout);
}


int main() {
    set<ull> check;
    ull x, s, l, r, i;
    scanf("%llu %llu", &x, &m);
    s = request(0);
    if (s < x) {
        l = x - m + 2 * (n - m) - s;
        r = x + 1 + 2 * (n - m) - s;
    } else if (s > x) {
        l = x + 2 * (n - m) - s;
        r = x + m + 1 + 2 * (n - m) - s;
    } else {
        answer(0);
        return 0;
    }
    check.insert(0);
    l %= n - m;
    r %= n - m;
    if (r < l) r += n - m;
    i = (l + r) / 2;
    while (l != i) {
        ull val = request(i);
        if ((val < x && !(x > m && val < x - m)) || val > x + m) {
            l = i;
        } else if (val > x || (x > m && val < x - m)) {
            r = i;
        } else {
            answer(i);
            return 0;
        }
        check.insert(i % (n - m));
        i = (l + r) / 2;
    }
    if (check.find(i % (n - m)) != check.end()) {
        printf("! -1\n");
        fflush(stdout);
        return 0;
    }
    ull val = request(i);
    if (val == x) {
        answer(i);
    } else {
        printf("! -1\n");
        fflush(stdout);
    }
    return 0;
}
