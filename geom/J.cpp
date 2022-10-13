#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int gcd(int a, int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

double frac_to_double(int num, int denom) {
    if (!num) {
        return 0.;
    }
    if (denom < 0) {
        num = -num;
        denom = -denom;
    }
    auto g = gcd(abs(num), denom);
    num /= g;
    denom /= g;
    return (double) num / (double) denom;
}

template<typename T>
struct point_t {
    T x;
    T y;
};

template<typename T>
bool operator==(point_t<T> const& p1, point_t<T> const& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

template<typename T>
bool operator!=(point_t<T> const& p1, point_t<T> const& p2) {
    return !(p1 == p2);
}

template<typename T>
point_t<T> operator-(point_t<T> const& p) {
    return point_t<T>{-p.x, -p.y};
}

template<typename T>
point_t<T> operator-(point_t<T> const& p1, point_t<T> const& p2) {
    return point_t<T>{p1.x - p2.x, p1.y - p2.y};
}

template<typename T>
T operator*(point_t<T> const& p1, point_t<T> const& p2) {
    return p1.x * p2.y - p1.y * p2.x;
}

constexpr double const VPROD_EPS = 4 * numeric_limits<double>::epsilon();

template<>
double operator*(point_t<double> const& p1, point_t<double> const& p2) {
    double v1 = p1.x * p2.y;
    double v2 = p1.y * p2.x;
    double e = (abs(v1) + abs(v2)) * VPROD_EPS;
    double p = v1 - v2;
    assert(!isnan(e));
    assert(!isnan(p));
    assert(p >= e || p <= -e);
    return p;
}

#ifdef DEBUG
template<typename T>
ostream& operator<<(ostream& os, point_t<T> const& p) {
    os << fixed << setprecision(0) << "{ " << p.x << ", " << p.y << " }";
    return os;
}
#endif

struct halfedge_t;
struct vertex_t;
struct line_t;

struct halfedge_t {
    vertex_t* const origin;
    halfedge_t* next;
    halfedge_t* prev;
    halfedge_t* twin;

    explicit halfedge_t(vertex_t* const origin)
    : origin(origin)
    , next(nullptr)
    , prev(nullptr)
    , twin(nullptr)
    {}
};

void connect(halfedge_t* e1, halfedge_t* e2) {
    assert(!e1->next);
    assert(!e2->prev);
    e1->next = e2;
    e2->prev = e1;
}

void disconnect(halfedge_t* e1, halfedge_t* e2) {
    assert(e1->next == e2);
    assert(e2->prev == e1);
    e1->next = nullptr;
    e2->prev = nullptr;
}

struct vertex_t {
    point_t<double> point;

private:
    struct direction_cmp {
        bool operator()(point_t<int> const& p1, point_t<int> const& p2) const {
            return p1 * p2 > 0;
        }

        static bool leq(point_t<int> const& p1, point_t<int> const& p2) {
            return p1 * p2 >= 0;
        }
    };

    map<point_t<int>, halfedge_t*, direction_cmp> up_edges;
    map<point_t<int>, halfedge_t*, direction_cmp> low_edges;

public:
    explicit vertex_t(point_t<double> point) : point(point) {}

    void init_up_edges(halfedge_t* e, point_t<int> const& dir) {
        up_edges.insert(make_pair(dir, e));
    }

    void init_low_edges(halfedge_t* e, point_t<int> const& dir) {
        low_edges.insert(make_pair(dir, e));
    }

    void init_edges(halfedge_t* e_f, halfedge_t* e_b, point_t<int> const& dir) {
        init_up_edges(e_f, dir);
        init_low_edges(e_b, -dir);
    }

    halfedge_t* find_halfedge(point_t<int> const& dir) const {
        auto const& edges_map =
            !up_edges.empty() && direction_cmp::leq(up_edges.begin()->first, dir)
            ? up_edges : low_edges;
        auto it = edges_map.find(dir);
        assert(it != edges_map.end());
        return it->second;
    }

    void add_halfedge(halfedge_t* e, point_t<int> const& dir) {
        assert(e->origin == this);
        bool from_up = direction_cmp()(up_edges.begin()->first, dir);
        auto& edges_map = from_up ? up_edges : low_edges;
        auto& other_edges_map = from_up ? low_edges : up_edges;
        auto ins = edges_map.insert(make_pair(dir, e));
        assert(ins.second);

        auto it_prev = ins.first;
        --it_prev;
        halfedge_t* e_prev = it_prev->second;

        auto it_next = ins.first;
        ++it_next;
        halfedge_t* e_next = (it_next != edges_map.end()) ?
            it_next->second : other_edges_map.begin()->second;

        if (e_prev->prev) {
            assert(e_next->twin->next);
            assert(e_next->twin->next == e_prev);
            assert(e_prev->prev == e_next->twin);
            disconnect(e_next->twin, e_prev);
        }
        connect(e_next->twin, e);
        connect(e->twin, e_prev);
    }

    halfedge_t* get_edge() const {
        auto const& edges_map = !up_edges.empty() ? up_edges : low_edges;
        return edges_map.begin()->second;
    }
};

struct line_t {
    int A;
    int B;
    int C;

    line_t() {}

    line_t(point_t<int> const& p1, point_t<int> const& p2) {
        int dx = p1.x - p2.x;
        int dy = p1.y - p2.y;
        if (dx < 0 || dx == 0 && dy < 0) {
            dx = -dx;
            dy = -dy;
        }
        A = -dy;
        B = dx;
        C = -(A * p1.x + B * p1.y);
    }

    point_t<int> forw() const {
        return point_t<int>{B, -A};
    }

    point_t<int> backw() const {
        return -forw();
    }
};

bool intersects(line_t const& l1, line_t const& l2) {
    return l1.A * l2.B - l1.B * l2.A != 0;
}

point_t<double> intersection(line_t const& l1, line_t const& l2) {
    int d = l1.A * l2.B - l1.B * l2.A;
    int d1 = l1.B * l2.C - l1.C * l2.B;
    int d2 = -l1.A * l2.C + l1.C * l2.A;
    return point_t<double>{frac_to_double(d1, d), frac_to_double(d2, d)};
}

struct vertex_cmp {
    bool operator()(vertex_t* v1, vertex_t* v2) const {
        auto const& p1 = v1->point;
        auto const& p2 = v2->point;
        return p1.x < p2.x || p1.x == p2.x && p1.y < p2.y;
    }
};

// void traverse_dcel_dbg(halfedge_t* e_start) {
//     set<halfedge_t*> used;
//     queue<halfedge_t*> q({ e_start });
//     while (!q.empty()) {
//         halfedge_t* e = q.front();
//         q.pop();
//         if (used.find(e) != used.end())
//             continue;
//         used.insert(e);
//         deque<point_t<double>> deq({e->origin->point});
//         for (halfedge_t* ei = e->next; ei && used.find(ei) == used.end(); ei = ei->next) {
//             deq.push_back(ei->origin->point);
//             used.insert(ei);
//             q.push(ei->twin);
//         }
//         for (halfedge_t* ei = e->prev; ei && used.find(ei) == used.end(); ei = ei->prev) {
//             deq.push_front(ei->origin->point);
//             used.insert(ei);
//             q.push(ei->twin);
//         }
//         for (auto const& p : deq) {
//             cout << p << " ";
//         } cout << endl;
//     }
// }

void traverse_dcel(halfedge_t* e_start) {
    unordered_set<vertex_t*> vertices;
    unordered_set<halfedge_t*> used;
    queue<halfedge_t*> q({ e_start });
    vector<double> squares;
    while (!q.empty()) {
        halfedge_t* e = q.front();
        q.pop();
        if (used.find(e) != used.end())
            continue;
        used.insert(e);
        vector<vertex_t*> v({ e->origin });
        halfedge_t* ei = e->next;
        while (ei && ei->origin != v[0]) {
            v.emplace_back(ei->origin);
            used.insert(ei);
            q.push(ei->twin);
            ei = ei->next;
        }
        if (!ei) {
            ei = e->prev;
            while (ei) {
                v.emplace_back(ei->origin);
                used.insert(ei);
                q.push(ei->twin);
                ei = ei->prev;
            }
        } else {
            squares.emplace_back(0);
            for (size_t i = 1; i + 1 < v.size(); ++i) {
                squares.back() += fabs((v[i]->point - v[0]->point) * (v[i + 1]->point - v[0]->point)) / 2.0;
            }
            assert(squares.back() > 1e-8);
        }
        vertices.insert(v.begin(), v.end());
    }
    for (auto const& it : vertices) {
        delete it;
    }
    for (auto const& it : used) {
        delete it;
    }
    sort(squares.begin(), squares.end());
    printf("%zd\n", squares.size());
    for (auto const& s : squares) {
        printf("%.42f\n", s);
    }
}

constexpr double const D_INF = numeric_limits<double>::infinity();

int main() {
    int n;
    scanf("%d", &n);
    vector<line_t> lines(n);
    vector<set<vertex_t*, vertex_cmp>> points_on_line(n);

    for (int i = 0; i < n; ++i) {
        point_t<int> p1;
        point_t<int> p2;
        scanf("%d %d %d %d", &p1.x, &p1.y, &p2.x, &p2.y);
        lines[i] = line_t(p1, p2);
        points_on_line[i].insert(new vertex_t(point_t<double>{-D_INF, -D_INF}));
        for (int j = 0; j < i; ++j) {
            if (!intersects(lines[j], lines[i]))
                continue;
            auto point = intersection(lines[j], lines[i]);
            vertex_t tmp_v(point);
            auto it = points_on_line[j].find(&tmp_v);
            if (it != points_on_line[j].end()) {
                points_on_line[i].insert(*it);
            } else {
                vertex_t* v0 = new vertex_t(point);
                halfedge_t* e01 = new halfedge_t(v0);
                halfedge_t* e02 = new halfedge_t(v0);
                v0->init_edges(e02, e01, lines[j].forw());

                it = points_on_line[j].insert(v0).first;
                --it;
                vertex_t* v1 = *it;
                ++it;
                ++it;
                vertex_t* v2 = *it;

                halfedge_t* e12 = v1->find_halfedge(lines[j].forw());
                halfedge_t* e21 = e12->twin;
                halfedge_t* e_from2 = e12->next;
                halfedge_t* e_from1 = e21->next;

                if (e_from1) {
                    disconnect(e21, e_from1);
                }
                e21->twin = e02;

                if (e_from1) {
                    connect(e01, e_from1);
                }
                e01->twin = e12;

                if (e_from2) {
                    disconnect(e12, e_from2);
                }
                e12->twin = e01;

                if (e_from2) {
                    connect(e02, e_from2);
                }
                e02->twin = e21;

                // e12 became e10
                // e21 became e20

                points_on_line[i].insert(v0);
            }
        }
        points_on_line[i].insert(new vertex_t(point_t<double>{D_INF, D_INF}));
        if (points_on_line[i].size() == 2) {
            auto it_prev = points_on_line[i].begin();
            auto it = points_on_line[i].begin();
            ++it;
            halfedge_t* e1 = new halfedge_t(*it_prev);
            halfedge_t* e2 = new halfedge_t(*it);
            e1->twin = e2;
            e2->twin = e1;
            (*it_prev)->init_up_edges(e1, lines[i].forw());
            (*it)->init_up_edges(e2, lines[i].backw());
        } else {
            auto const forw_dir = lines[i].forw();
            auto const backw_dir = lines[i].backw();
            auto it_prev = points_on_line[i].begin();
            auto it = points_on_line[i].begin();
            ++it;
            /* from -inf to vertices[1] */ {
                halfedge_t* e1 = new halfedge_t(*it_prev);
                halfedge_t* e2 = new halfedge_t(*it);
                e1->twin = e2;
                e2->twin = e1;
                (*it_prev)->init_up_edges(e1, forw_dir);
                (*it)->add_halfedge(e2, backw_dir);
                ++it_prev;
                ++it;
            }
            for (size_t j = 1; j + 2 < points_on_line[i].size(); ++j) {
                halfedge_t* e1 = new halfedge_t(*it_prev);
                halfedge_t* e2 = new halfedge_t(*it);
                e1->twin = e2;
                e2->twin = e1;
                (*it_prev)->add_halfedge(e1, forw_dir);
                (*it)->add_halfedge(e2, backw_dir);
                ++it_prev;
                ++it;
            }
            /* from vertices[-2] to +inf */ {
                halfedge_t* e1 = new halfedge_t(*it_prev);
                halfedge_t* e2 = new halfedge_t(*it);
                e1->twin = e2;
                e2->twin = e1;
                (*it_prev)->add_halfedge(e1, forw_dir);
                (*it)->init_low_edges(e2, backw_dir);
            }
        }
        // cout << "added line number " << i + 1 << endl;
        // traverse_dcel_dbg((*points_on_line[0].begin())->get_edge());
        // cout << endl;
    }

    traverse_dcel((*points_on_line[0].begin())->get_edge());

    return 0;
}
