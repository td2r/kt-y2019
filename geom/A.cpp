#include <bits/stdc++.h>
#include <boost/numeric/interval.hpp>
#include <gmpxx.h>
#include "seg_intersection_tests.h"
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

struct point_t {
    double x;
    double y;
};

bool operator==(point_t const& p1, point_t const& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(point_t const& p1, point_t const& p2) {
    return !(p1 == p2);
}

ostream& operator<<(ostream& os, point_t const& p) {
    os << fixed << setprecision(0) << "{" << p.x << ", " << p.y << "}";
    return os;
}

// AFAIK, the real machine epsilon couldn't be represented in type for this epsilon
// and its actual value is numeric_limits<double::epsilon() / 2
// So therefore we multiply it by 4 instead of 8
constexpr double const CALC_EPS = 4 * numeric_limits<double>::epsilon();

// optional req C++17
pair<bool, int> regular_pred(point_t const& p0, point_t const& p1, point_t const& p2) {
    double v1 = (p1.x - p0.x) * (p2.y - p0.y);
    double v2 = (p1.y - p0.y) * (p2.x - p0.x);
    double p = v1 - v2;
    double e = (fabs(v1) + fabs(v2)) * CALC_EPS;
    if (isnan(p) || isnan(e) || -e <= p && p <= e) {
        return make_pair(false, 0);
    } else {
        return make_pair(true, p < 0 ? -1 : p > 0 ? 1 : 0);
    }
}

pair<bool, int> interval_pred(point_t const& p0, point_t const& p1, point_t const& p2) {
    auto inter = (boost::numeric::interval<double>(p1.x) - p0.x)
                    * (boost::numeric::interval<double>(p2.y) - p0.y)
               - (boost::numeric::interval<double>(p1.y) - p0.y)
                    * (boost::numeric::interval<double>(p2.x) - p0.x);
    auto l = inter.lower();
    auto r = inter.upper();
    // todo: [0; 0] might be valid inter, but could it be?
    if (isnan(l) || isnan(r) || boost::numeric::zero_in(inter)) {
        return make_pair(false, 0);
    }
    if (l > 0) {
        assert(r > 0);
        return make_pair(true, 1);
    } else {
        assert(l < 0 && r < 0);
        return make_pair(true, -1);
    }
}

// todo: precision might be calculated more accurately for specified numbers,
// but it should be set for each number accordingly
int const MPF_PREC_BASE = 2 * (numeric_limits<double>::digits +
    (numeric_limits<double>::max_exponent - numeric_limits<double>::min_exponent));

// (a - b) * (c - d)
void mpf_prod_of_subs(double a, double b, double c, double d, mpf_t& res) {
    mpf_t mpf_a;
    mpf_init_set_d(mpf_a, a);
    mpf_t mpf_b;
    mpf_init_set_d(mpf_b, b);
    mpf_t mpf_c;
    mpf_init_set_d(mpf_c, c);
    mpf_t mpf_d;
    mpf_init_set_d(mpf_d, d);

    mpf_t sub1;
    mpf_init(sub1);
    mpf_sub(sub1, mpf_a, mpf_b);
    mpf_t sub2;
    mpf_init(sub2);
    mpf_sub(sub2, mpf_c, mpf_d);
    mpf_clear(mpf_a);
    mpf_clear(mpf_b);
    mpf_clear(mpf_c);
    mpf_clear(mpf_d);

    // mpf_clear(res);
    mpf_init(res);
    mpf_mul(res, sub1, sub2);
    mpf_clear(sub1);
    mpf_clear(sub2);
}

int mpf_pred(point_t const& p0, point_t const& p1, point_t const& p2) {
    mpf_t v1;
    mpf_prod_of_subs(p1.x, p0.x, p2.y, p0.y, v1);
    mpf_t v2;
    mpf_prod_of_subs(p1.y, p0.y, p2.x, p0.x, v2);
    int p = mpf_cmp(v1, v2);
    mpf_clear(v1);
    mpf_clear(v2);
    return p > 0 ? 1 : p < 0 ? -1 : 0;
}

int turn2d(point_t const& p0, point_t const& p1, point_t const& p2) {
    pair<bool, int> res;
    res = regular_pred(p0, p1, p2);
    if (res.first) {
        return res.second;
    }
    res = interval_pred(p0, p1, p2);
    if (res.first) {
        return res.second;
    }
    return mpf_pred(p0, p1, p2);
}

bool in_bounding_box(point_t const& p, pair<point_t, point_t> const& bb) {
    auto x1 = bb.first.x;
    auto x2 = bb.second.x;
    if (x1 > x2)
        swap(x1, x2);
    auto y1 = bb.first.y;
    auto y2 = bb.second.y;
    if (y1 > y2)
        swap(y1, y2);
    return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2;
}

bool intersects(pair<point_t, point_t> const& o1, pair<point_t, point_t> const& o2) {
    if (o1.first == o1.second) {
        if (o2.first == o2.second) {
            return o1.first == o2.first;
        } else {
            int t = turn2d(o1.first, o2.first, o2.second);
            return t == 0 && in_bounding_box(o1.first, o2);
        }
    } else if (o2.first == o2.second) {
        int t = turn2d(o2.first, o1.first, o1.second);
        return t == 0 && in_bounding_box(o2.first, o1);
    }
    // todo: not all 4 turns necessarily need in some cases
    int t11 = turn2d(o2.first, o2.second, o1.first);
    int t12 = turn2d(o2.first, o2.second, o1.second);
    int t21 = turn2d(o1.first, o1.second, o2.first);
    int t22 = turn2d(o1.first, o1.second, o2.second);
    if (t11 == 0 || t12 == 0) {
        return (t11 == 0 && in_bounding_box(o1.first, o2))
            || (t12 == 0 && in_bounding_box(o1.second, o2))
            || (t11 == 0 && t12 == 0 && in_bounding_box(o2.first, o1));
    }
    if (t11 == t12)
        return false;
    assert(t21 != 0 || t22 != 0);
    if (t21 == 0 || t22 == 0) {
        return (t21 == 0 && in_bounding_box(o2.first, o1))
            || (t22 == 0 && in_bounding_box(o2.second, o1));
    }
    return t21 != t22;
}

int main() {
    mpf_set_default_prec(MPF_PREC_BASE);

    int t;
    scanf("%d", &t);
    auto inp = genTest(t);
    for (size_t i = 0; i < inp.size(); i += 8) {
        printf("%c", intersects(
                make_pair(point_t{inp[i], inp[i + 1]},
                          point_t{inp[i + 2], inp[i + 3]}),
                make_pair(point_t{inp[i + 4], inp[i + 5]},
                          point_t{inp[i + 6], inp[i + 7]})
            ) ?  'Y' : 'N');
    }
    printf("\n");

    return 0;
}
