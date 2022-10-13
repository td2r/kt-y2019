#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

struct point_t {
    int x;
    int y;

    point_t operator+(point_t const& other) const {
        return {x + other.x, y + other.y};
    }

    point_t operator-(point_t const& other) const {
        return {x - other.x, y - other.y};
    }

    ll operator*(point_t const& other) const {
        return (ll) x * other.y - (ll) other.x * y;
    }

    bool operator==(point_t const& other) const {
        return x == other.x && y == other.y;
    }

    point_t operator*(int const k) const {
        return {k * x, k * y};
    }
};

point_t operator*(int const k, point_t const& p) {
    return p * k;
}

ostream& operator<<(ostream& os, point_t const& p) {
    os << "{" << p.x << "," << p.y << "}";
    return os;
}

ll rotate(point_t const& p, point_t const& p1, point_t const& p2) {
    return (p1 - p) * (p2 - p);
}

int rotate_sgn(point_t const& p, point_t const& p1, point_t const& p2) {
    ll r = rotate(p, p1, p2);
    return r == 0 ? 0 : r < 0 ? -1 : 1;
}

bool in_segment(point_t const& p, point_t const& p1, point_t const& p2) {
    int x1 = p1.x;
    int x2 = p2.x;
    if (x1 > x2)
        swap(x1, x2);
    int y1 = p1.y;
    int y2 = p2.y;
    if (y1 > y2)
        swap(y1, y2);
    return rotate_sgn(p, p1, p2) == 0 && (x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2);
}

bool strictly_in_segment(point_t const& p, point_t const& p1, point_t const& p2) {
    if (p == p1 || p == p2)
        return false;
    int x1 = p1.x;
    int x2 = p2.x;
    if (x1 > x2)
        swap(x1, x2);
    int y1 = p1.y;
    int y2 = p2.y;
    if (y1 > y2)
        swap(y1, y2);
    return /*rotate_sgn(p, p1, p2) == 0 && */x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2;
}

bool intersects_not_connected(point_t const& p1,
                              point_t const& p2,
                              point_t const& p3,
                              point_t const& p4)
{
    int r1 = rotate_sgn(p3, p4, p1);
    int r2 = rotate_sgn(p3, p4, p2);
    int r3 = rotate_sgn(p1, p2, p3);
    int r4 = rotate_sgn(p1, p2, p4);
    if (r1 == 0 || r2 == 0 || r3 == 0 || r4 == 0) {
        return r1 == 0 && strictly_in_segment(p1, p3, p4) ||
               r2 == 0 && strictly_in_segment(p2, p3, p4) ||
               r3 == 0 && strictly_in_segment(p3, p1, p2) ||
               r4 == 0 && strictly_in_segment(p4, p1, p2) ||
               p1 == p3 && p2 == p4 ||
               p1 == p4 && p2 == p3;
    } else {
        return r1 != r2 && r3 != r4;
    }
}

int const MAXN = 4000 + 3;

int n;
point_t p[MAXN];

// all coordinates multiplied by 2
bool is_point_inside_polygon(point_t pt) {
    bool res = false;
    for (int i = 1; i <= n; ++i) {
        point_t p1 = 2 * p[i - 1];
        point_t p2 = 2 * p[i];
        if (in_segment(pt, p1, p2)) {
            return true;
        }
        if (p1.y > p2.y) {
            swap(p1, p2);
        }
        if (p1.y < pt.y && pt.y <= p2.y && rotate_sgn(pt, p1, p2) == 1) {
            res = !res;
        }
    }
    return res;
}

bool is_edge_inside_polygon(int p1_id, int p2_id) {
    // printf("is_edge_inside_polygon(%d, %d)\n", p1_id + 1, p2_id + 1);
    point_t const& p1 = p[p1_id];
    point_t const& p2 = p[p2_id];
    for (int i = 1; i <= n; ++i) {
        if (intersects_not_connected(p[i - 1], p[i], p1, p2)) {
            // printf("intersects with edge (%d, %d)\n", i, i + 1);
            return false;
        }
    }
    return is_point_inside_polygon(p1 + p2);
}

int main() {
    scanf("%d", &n);
    vector<int> st;
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &p[i].x, &p[i].y);
    }
    p[n] = p[0];

    for (int i = 0; i < n; ++i) {
        while (st.size() >= 2 && is_edge_inside_polygon(st[st.size() - 2], i)) {
            printf("%d %d\n", st[st.size() - 2], i);
            st.pop_back();
        }
        st.emplace_back(i);
    }
    assert(st.size() <= 3);
    return 0;
}
