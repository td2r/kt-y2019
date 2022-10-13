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
};

ostream& operator<<(ostream& os, point_t const& p) {
    os << "{" << p.x << "," << p.y << "}";
    return os;
}

struct edge_t {
    point_t p1;
    point_t p2;
    int next;
    int prev;
    int k1id = -1;
    int k2id = -1;

    edge_t() {}

    edge_t(point_t const& p1, point_t const& p2)
    : p1(p1)
    , p2(p2)
    {}
};

ostream& operator<<(ostream& os, edge_t const& e) {
    os << "edge" <<
        "(p1=" << e.p1 <<
        ",p2=" << e.p2 <<
        ",next=" << e.next <<
        ",prev=" << e.prev <<
        ",k1id=" << e.k1id <<
        ",k2id=" << e.k2id <<
        ")";
    return os;
}

ll sdist(point_t const& p1, point_t const& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

ll vector_product(point_t const& p1, point_t const& p2) {
    return (ll) p1.x * p2.y - (ll) p2.x * p1.y;
}

ll rotate(point_t const& p, point_t const& p1, point_t const& p2) {
    return vector_product(p1 - p, p2 - p);
}

int rotate_sgn(point_t const& p, point_t const& p1, point_t const& p2) {
    ll sq = rotate(p, p1, p2);
    return sq == 0 ? 0 : sq < 0 ? -1 : 1;
}

bool conflicts(point_t const& p, edge_t const& e) {
    int x1 = e.p1.x;
    int x2 = e.p2.x;
    if (x1 > x2) {
        swap(x1, x2);
    }
    int y1 = e.p1.y;
    int y2 = e.p2.y;
    if (y1 > y2) {
        swap(y1, y2);
    }
    int turn = rotate_sgn(e.p1, e.p2, p);
    return !(turn == 1 || turn == 0 &&
            x1 <= p.x && p.x <= x2 &&
            y1 <= p.y && p.y <= y2);
}

void process_vertex(point_t const& p, vector<edge_t>& edges) {
    int bad_edge_id = conflicts(p, edges[0]) ? 0 :
                       conflicts(p, edges[1]) ? 1 :
                       conflicts(p, edges[2]) ? 2 : -1;
    if (bad_edge_id == -1)
        return;
    while (edges[bad_edge_id].k1id != -1) {
        int k1 = edges[bad_edge_id].k1id;
        int k2 = edges[bad_edge_id].k2id;
        if (conflicts(p, edges[k1])) {
            bad_edge_id = k1;
        } else if (conflicts(p, edges[k2])) {
            bad_edge_id = k2;
        } else {
            return;
        }
    }
    vector<int> bad_edges_ids({bad_edge_id});
    int e_id = edges[bad_edge_id].prev;
    while (conflicts(p, edges[e_id])) {
        bad_edges_ids.emplace_back(e_id);
        e_id = edges[e_id].prev;
    }
    edges.emplace_back(edges[e_id].p2, p);
    edges.back().prev = e_id;
    edges[e_id].next = edges.size() - 1;

    e_id = edges[bad_edge_id].next;
    while (conflicts(p, edges[e_id])) {
        bad_edges_ids.emplace_back(e_id);
        e_id = edges[e_id].next;
    }
    edges.emplace_back(p, edges[e_id].p1);
    edges.back().next = e_id;
    edges[e_id].prev = edges.size() - 1;

    edges[edges.size() - 2].next = edges.size() - 1;
    edges.back().prev = edges.size() - 2;

    for (int const& id : bad_edges_ids) {
        edges[id].k1id = edges.size() - 2;
        edges[id].k2id = edges.size() - 1;
    }
}

void show_step(int step_id, vector<edge_t> const& edges) {
    cout << "step " << step_id << endl;
    for (size_t i = 0; i < edges.size(); ++i) {
        cout  << i << ": " << edges[i] << endl;
    }
    cout << endl;
}

void quick_hull(int const n, point_t* p, vector<point_t>& res) {
    random_shuffle(p, p + n);
    random_shuffle(p, p + n);
    random_shuffle(p, p + n);
    vector<edge_t> edges(3);
    edges[0].p1 = p[0];
    edges[1].p1 = p[0];
    edges[2].p1 = p[0];
    for (int i = 0; i < n; ++i) {
        if (p[i].y < edges[0].p1.y || p[i].y == edges[0].p1.y && p[i].x < edges[0].p1.x) {
            edges[0].p1 = p[i];
        }
    }
    for (int i = 0; i < n; ++i) {
        int rot = rotate_sgn(edges[0].p1, p[i], edges[1].p1);
        if (rot == 1 || rot == 0 && sdist(edges[0].p1, p[i]) > sdist(edges[0].p1, edges[1].p1)) {
            edges[1].p1 = p[i];
        }
        rot = rotate_sgn(edges[0].p1, edges[2].p1, p[i]);
        if (rot == 1 || rot == 0 && sdist(edges[0].p1, p[i]) > sdist(edges[0].p1, edges[2].p1)) {
            edges[2].p1 = p[i];
        }
    }
    edges[0].next = 1;
    edges[1].next = 2;
    edges[2].next = 0;
    edges[0].p2 = edges[1].p1;
    edges[1].p2 = edges[2].p1;
    edges[2].p2 = edges[0].p1;
    edges[0].prev = 2;
    edges[1].prev = 0;
    edges[2].prev = 1;

#ifdef DEBUG
    show_step(0, edges);
#endif
    for (int i = 0; i < n; ++i) {
        process_vertex(p[i], edges);
#ifdef DEBUG
        show_step(i + 1, edges);
#endif
    }

    int start_edge = 0;
    while (edges[start_edge].k1id != -1) {
        start_edge = edges[start_edge].k1id;
    }
    int e_id = start_edge;
    do {
        res.push_back(edges[e_id].p1);
        e_id = edges[e_id].next;
    } while (e_id != start_edge);
}

int const MAXN = 100000 + 3;

int n;
point_t p[MAXN];

int main() {
    srand(random_device()());
    int x;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &p[i].x, &p[i].y);
    }
    vector<point_t> hull;
    quick_hull(n, p, hull);
    printf("%zd\n", hull.size());
    for (point_t const& p : hull) {
        printf("%d %d\n", p.x, p.y);
    }
    ll square = 0;
    for (size_t i = 2; i < hull.size(); ++i) {
        square += rotate(hull[0], hull[i - 1], hull[i]);
    }
    printf("%lld.%s\n", square / 2, (square % 2) ? "5" : "0");
    return 0;
}
