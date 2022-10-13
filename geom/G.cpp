#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

struct point_t {
    ll x;
    ll y;

    point_t() {}
    point_t(ll x, ll y) : x(x), y(y) {}
};

point_t operator-(point_t const& p) {
    return {-p.x, -p.y};
}

point_t operator+(point_t const& p1, point_t const& p2) {
    return {p1.x + p2.x, p1.y + p2.y};
}

point_t operator-(point_t const& p1, point_t const& p2) {
    return {p1.x - p2.x, p1.y - p2.y};
}

ll operator*(point_t const& p1, point_t const& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

ll operator%(point_t const& p1, point_t const& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

bool operator==(point_t const& p1, point_t const& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

ostream& operator<<(ostream& os, point_t const& p) {
    os << "{" << p.x << "," << p.y << "}";
    return os;
}

ll rotate(point_t const& p, point_t const& p1, point_t const& p2) {
    return (p1 - p) * (p2 - p);
}

int turn(point_t const& p, point_t const& p1, point_t const& p2) {
    ll r = rotate(p, p1, p2);
    return r == 0 ? 0 : r < 0 ? -1 : 1;
}

int bottom_left_point_index(int const n, point_t const* p) {
    int idx = 0;
    for (int i = 0; i < n; ++i) {
        if (p[i].y < p[idx].y || p[i].y == p[idx].y && p[i].x < p[idx].x) {
        // if (p[i].x < p[idx].x || p[i].x == p[idx].x && p[i].y < p[idx].y) {
            idx = i;
        }
    }
    return idx;
}

void minkowski_sum(int const n1, point_t const* p1,
                   int const n2, point_t const* p2,
                   vector<point_t>& res)
{
    // cout << "minkowski_sum" << endl;
    // cout << n1 << endl;
    // for (int i = 0; i < n1; ++i) {
    //     cout << p1[i] << endl;
    // }
    // cout << n2 << endl;
    // for (int i = 0; i < n2; ++i) {
    //     cout << p2[i] << endl;
    // }
    int s1 = bottom_left_point_index(n1, p1);
    int s2 = bottom_left_point_index(n2, p2);
    // cout << s1 + 1 << endl;
    // cout << s2 + 1 << endl;
    for (int t1 = 0, t2 = 0; t1 < n1 || t2 < n2;) {
        int i1 = (s1 + t1) % n1;
        int i2 = (s2 + t2) % n2;
        res.emplace_back(p1[i1] + p2[i2]);
        ll p = (p1[(i1 + 1) % n1] - p1[i1]) * (p2[(i2 + 1) % n2] - p2[i2]);
        if (p > 0) {
            ++t1;
        } else if (p < 0) {
            ++t2;
        } else {
            ++t1;
            ++t2;
        }
    }
}

bool in_segment(point_t const& p, point_t const& p1, point_t const& p2) {
    ll x1 = p1.x;
    ll x2 = p2.x;
    if (x1 > x2)
        swap(x1, x2);
    ll y1 = p1.y;
    ll y2 = p2.y;
    if (y1 > y2)
        swap(y1, y2);
    return rotate(p, p1, p2) == 0 &&
        x1 <= p.x && p.x <= x2 &&
        y1 <= p.y && p.y <= y2;
}

bool is_point_inside_polygon(point_t const& pt, vector<point_t> const& p) {
    size_t n = p.size();
    bool res = false;
    for (size_t i = 1; i <= n; ++i) {
        point_t p1 = p[i - 1];
        point_t p2 = p[i % n];
        if (in_segment(pt, p1, p2)) {
            return true;
        }
        if (p1.y > p2.y) {
            swap(p1, p2);
        }
        if (p1.y < pt.y && pt.y <= p2.y && turn(pt, p1, p2) == 1) {
            res = !res;
        }
    }
    return res;
}

long double dist(point_t const& p1, point_t const& p2) {
    point_t d = p1 - p2;
    return sqrtl((long double) (d % d));
}

// is vertex angle A acute
bool is_acute(point_t const& a, point_t const& b, point_t const& c) {
    // return ((b - c) % (b - c)) - ((a - b) % (a - b)) - ((a - c) % (a - c)) < 0; // cos theorem
    return ((b - c) % (b - c)) - ((a - b) % (a - b)) <  ((a - c) % (a - c));
};

long double point_segment_dist(point_t const& p, point_t const& p1, point_t const& p2) {
    if (is_acute(p1, p, p2) && is_acute(p2, p, p1)) {
        return abs(rotate(p, p1, p2)) / dist(p1, p2);
    } else {
        return min(dist(p, p1), dist(p, p2));
    }
}

int const MAXN = 50000 + 3;

int n1;
int n2;
point_t p1[MAXN];
point_t p2[MAXN];


int main() {
    scanf("%d", &n1);
    for (int i = 0; i < n1; ++i) {
        scanf("%lld %lld", &p1[i].x, &p1[i].y);
    }
    scanf("%d", &n2);
    for (int i = 0; i < n2; ++i) {
        scanf("%lld %lld", &p2[i].x, &p2[i].y);
    }
    reverse(p1, p1 + n1);
    reverse(p2, p2 + n2);

    long double ans;

    if (n1 > n2) {
        swap(n1, n2);
        swap(p1, p2);
    }
    if (n1 == 1) {
        ans = point_segment_dist(p1[0], p2[0], p2[n2 - 1]);
        for (int i = 1; i < n2; ++i) {
            ans = min(ans, point_segment_dist(p1[0], p2[i - 1], p2[i]));
        }
    } else if (n1 == 2) {
        if (p1[0] == p1[1])
            throw(1);
        // ans = segment_segment_dist(p1[0], p1[1], p2[0], p2[n2 - 1]);
        ans = min({
                point_segment_dist(p1[0], p2[0], p2[n2 - 1]),
                point_segment_dist(p1[1], p2[0], p2[n2 - 1]),
                point_segment_dist(p2[0], p1[0], p1[1]),
                point_segment_dist(p2[n2 - 1], p1[0], p1[1])
                });
        for (int i = 1; i < n2; ++i) {
            ans = min({
                    ans,
                    point_segment_dist(p1[0], p2[i - 1], p2[i]),
                    point_segment_dist(p1[1], p2[i - 1], p2[i]),
                    point_segment_dist(p2[i - 1], p1[0], p1[1]),
                    point_segment_dist(p2[i], p1[0], p1[1])
                    });
        }
    } else {
        for (int i = 0; i < n1; ++i) {
            p1[i] = -p1[i];
        }
        assert(turn(p1[0], p1[1], p1[2]) != -1);
        assert(turn(p2[0], p2[1], p2[2]) != -1);
        vector<point_t> mink;
        minkowski_sum(n1, p1, n2, p2, mink);
        if (is_point_inside_polygon({0, 0}, mink)) {
            ans = 0.;
        } else {
            ans = point_segment_dist({0, 0}, mink.back(), mink[0]);
            for (size_t i = 1; i < mink.size(); ++i) {
                ans = min(ans, point_segment_dist({0, 0}, mink[i - 1], mink[i]));
            }
        }
    }
    printf("%.50Lf\n", ans);
    return 0;
}
