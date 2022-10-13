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

int operator*(point_t const& p1, point_t const& p2) {
    return p1.x * p2.y - p2.x * p1.y;
}

int operator%(point_t const& p1, point_t const& p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

bool operator==(point_t const& p1, point_t const& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=(point_t const& p1, point_t const& p2) {
    return !(p1 == p2);
}

ostream& operator<<(ostream& os, point_t const& p) {
    os << "{" << p.x << "," << p.y << "}";
    return os;
}

inline int orient2d(point_t const& p0,
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

bool in_offcut(point_t const& p, pair<point_t, point_t> const& o) {
    if (orient2d(p, o.first, o.second) != 0)
        return false;
    auto x1 = o.first.x;
    auto x2 = o.second.x;
    if (x1 > x2)
        swap(x1, x2);
    auto y1 = o.first.y;
    auto y2 = o.second.y;
    if (y1 > y2)
        swap(y1, y2);
    return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2;
}

bool intersects(pair<point_t, point_t> const& o1, pair<point_t, point_t> const& o2) {
    if (o1.first == o1.second) {
        if (o2.first == o2.second) {
            return o1.first == o2.first;
        }
        return in_offcut(o1.first, o2);
    } else if (o2.first == o2.second) {
        return in_offcut(o2.first, o1);
    }
    int t1 = turn2d(o1.first, o1.second, o2.first);
    int t2 = turn2d(o1.first, o1.second, o2.second);
    if (t1 == 0 || t2 == 0) {
        return (t1 == 0 && in_offcut(o2.first, o1))
            || (t2 == 0 && in_offcut(o2.second, o1))
			|| (t1 == 0 && t2 == 0 && (in_offcut(o1.first, o2) || in_offcut(o1.second, o2)));
    }
    if (t1 == t2)
        return false;
    t1 = turn2d(o2.first, o2.second, o1.first);
    t2 = turn2d(o2.first, o2.second, o1.second);
	assert(t1 != 0 || t2 != 0);
    if (t1 == 0 || t2 == 0) {
        return (t1 == 0 && in_offcut(o1.first, o2))
            || (t2 == 0 && in_offcut(o1.second, o2));
    }
    return t1 != t2;
}

enum EventType {
    Insert,
    Delete,
};

struct event_t {
    point_t p;
    EventType type;
    int idx;
};

bool operator<(point_t const& p1, point_t const& p2) {
    return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
}

bool operator<(event_t const& e1, event_t const& e2) {
    if (e1.p != e2.p) {
        return e1.p < e2.p;
    } else if (e1.type != e2.type) {
        return e1.type == Insert;
    } else {
        return e1.idx < e2.idx;
    }
}

// y(x, x1, x2, y1, y2) = 
// = y1 + (x - x1) * (y2 - y1) / (x2 - x1)
// = [y1 * (x2 - x1) + (x - x1) * (y2 - y1)] / (x2 - x1)
// = [y1 * (x2 - x) + y2 * (x - x1)] / (x2 - x1)
// function returns pair - numerator and denominator
// denom > 0
pair<int, int> y_by_x(int x, pair<point_t, point_t> const& o) {
    if (o.first.x == o.second.x) {
        return make_pair(o.first.y, 1);
    }
    auto res = make_pair(o.first.y * (o.second.x - x) + o.second.y * (x - o.first.x),
                         o.second.x - o.first.x);
    if (res.second < 0) {
        res.first = -res.first;
        res.second = -res.second;
    }
    return res;
}

ll vector_prod(int x1, int y1, int x2, int y2) {
    return (ll) x1 * y2 - (ll) x2 * y1;
}

ll vector_prod(pair<int, int> const& p1, pair<int, int> const& p2) {
    return vector_prod(p1.first, p1.second, p2.first, p2.second);
}

ll vector_prod(point_t const& p1, point_t const& p2) {
    return vector_prod(p1.x, p1.y, p2.x, p2.y);
}

int main() {
    int n;
    scanf("%d", &n);
    vector<pair<point_t, point_t>> offcuts(n);
    set<event_t> q;
    int st_x;
    auto status_cmp = [&](int const& i1, int const& i2) {
        auto y_cmp = vector_prod(y_by_x(st_x, offcuts[i1]), y_by_x(st_x, offcuts[i2]));
        if (y_cmp != 0) // y1 != y2
            return y_cmp < 0; // y1 < y2;
        auto p = vector_prod(offcuts[i1].second - offcuts[i1].first,
                             offcuts[i2].second - offcuts[i2].first);
        return p > 0 || p == 0 && i1 < i2;
    };
    set<int, decltype(status_cmp)> st(status_cmp);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d %d", &offcuts[i].first.x, &offcuts[i].first.y,
                &offcuts[i].second.x, &offcuts[i].second.y);
        if (!(offcuts[i].first < offcuts[i].second)) {
            swap(offcuts[i].first, offcuts[i].second);
        }
        q.insert(event_t{offcuts[i].first, Insert, i});
        q.insert(event_t{offcuts[i].second, Delete, i});
    }
    while (!q.empty()) {
        auto event = *q.begin();
        q.erase(q.begin());
        // for task of finding ALL intersections st_x should be updated BEFORE insertion and AFTER deletion
        st_x = event.p.x;
        if (event.type == Insert) {
            auto ret = st.insert(event.idx);
            assert(ret.second);
            auto it = ret.first;
            auto neigh_it = it;
            if (it != st.begin()) { // check neighbour below
                --neigh_it;
                if (intersects(offcuts[*it], offcuts[*neigh_it])) {
                    printf("YES\n%d %d\n", *it + 1, *neigh_it + 1);
                    return 0;
                }
            }
            neigh_it = it;
            ++neigh_it;
            if (neigh_it != st.end()) { // check neighbour above
                if (intersects(offcuts[*it], offcuts[*neigh_it])) {
                    printf("YES\n%d %d\n", *it + 1, *neigh_it + 1);
                    return 0;
                }
            }
        } else { // event.type == Delete
            auto it = st.find(event.idx);
            assert(it != st.end());
            assert(*it == event.idx);
            auto neigh_above = it;
            ++neigh_above;
            if (it != st.begin() && neigh_above != st.end()) { // neighbours below and above both exists
                auto neigh_below = it;
                --neigh_below;
                if (intersects(offcuts[*neigh_below], offcuts[*neigh_above])) {
                    printf("YES\n%d %d\n", *neigh_below + 1, *neigh_above + 1);
                    return 0;
                }
            }
            st.erase(it);
            assert(st.find(event.idx) == st.end());
        }
    }
    printf("NO\n");
    return 0;
}
