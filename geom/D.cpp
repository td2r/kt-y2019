#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

struct point_t {
    int x;
    int y;
};

// point_t operator+(point_t const& p1, point_t const& p2) {
//     return point_t{p1.x + p2.x, p1.y + p2.y};
// }

point_t operator-(point_t const& p1, point_t const& p2) {
    return point_t{p1.x - p2.x, p1.y - p2.y};
}

ll operator*(point_t const& p1, point_t const& p2) {
    return (ll) p1.x * p2.y - (ll) p2.x * p1.y;
}

ll operator%(point_t const& p1, point_t const& p2) {
    return (ll) p1.x * p2.x + (ll) p1.y * p2.y;
}

// bool operator==(point_t const& p1, point_t const& p2) {
//     return p1.x == p2.x && p1.y == p2.y;
// }
//
// inline bool operator!=(point_t const& p1, point_t const& p2) {
//     return !(p1 == p2);
// }

ostream& operator<<(ostream& os, point_t const& p) {
    os << "{" << p.x << "," << p.y << "}";
    return os;
}

// dist ** 2
inline ll sdist(point_t const& p1, point_t const& p2) {
    point_t const d = p2 - p1;
    return d % d;
}

inline ll orient2d(point_t const& p0,
                   point_t const& p1,
                   point_t const& p2)
{
    return (p1 - p0) * (p2 - p0);
}

inline int turn2d(point_t const& p0,
                  point_t const& p1,
                  point_t const& p2)
{
    auto det = orient2d(p0, p1, p2);
    return det < 0 ? -1 : det == 0 ? 0 : 1;
}

bool is_point_inside_poly(point_t const& p, vector<point_t> const& poly) {
    auto t1 = turn2d(poly[0], poly[1], p);
    auto tn = turn2d(poly[0], poly.back(), p);
    if (t1 == -1 || tn == 1)
        return false;
    int l = 1;
    int r = (int) poly.size();
    int i = (l + r) / 2;
    while (i != l) {
        if (turn2d(poly[0], poly[i], p) == -1) {
            r = i;
        } else {
            l = i;
        }
        i = (l + r) / 2;
    }
    assert(l + 1 == r);
    assert(turn2d(poly[0], poly[i], p) != -1);
    if (i + 1 < poly.size())
        assert(turn2d(poly[0], poly[i + 1], p) == -1);
    if (turn2d(poly[0], poly[i], p) == 0) {
        return sdist(poly[0], p) <= sdist(poly[0], poly[i]);
    } else {
        assert(i + 1 < poly.size());
        return turn2d(poly[i], poly[i + 1], p) != -1;
    }
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<point_t> poly(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &poly[i].x, &poly[i].y);
    }
    int hit = 0;
    for (int i = 0; i < m; ++i) {
        point_t p;
        scanf("%d %d", &p.x, &p.y);
        hit += (int) is_point_inside_poly(p, poly);
    }
    printf("%s\n", hit >= k ? "YES" : "NO");
    return 0;
}
