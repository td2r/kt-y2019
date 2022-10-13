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

// point_t operator-(point_t const& p) {
//     return {-p.x, -p.y};
// }

point_t operator+(point_t const& p1, point_t const& p2) {
    return point_t{p1.x + p2.x, p1.y + p2.y};
}

point_t operator-(point_t const& p1, point_t const& p2) {
    return point_t{p1.x - p2.x, p1.y - p2.y};
}

ll operator*(point_t const& p1, point_t const& p2) {
    return (ll) p1.x * p2.y - (ll) p2.x * p1.y;
}

ll operator%(point_t const& p1, point_t const& p2) {
    return (ll) p1.x * p2.x + (ll) p1.y * p2.y;
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

namespace std {
    template<>
    struct hash<point_t> {
        size_t operator()(point_t const& p) const {
            return (((ll) p.x << 32) | ((ll) p.y));
        }
    };
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

inline ll hyperboloid_z(point_t const& p) {
    ll x = p.x;
    ll y = p.y;
    return x * x + y * y;
}

inline __int128 orient3d_with_z(point_t const& p0, ll const p0z,
                                point_t const& p1, ll const p1z,
                                point_t const& p2, ll const p2z,
                                point_t const& p3, ll const p3z)
{
    __int128 x1 = p1.x - p0.x;
    __int128 y1 = p1.y - p0.y;
    __int128 z1 = p1z - p0z;
    __int128 x2 = p2.x - p0.x;
    __int128 y2 = p2.y - p0.y;
    __int128 z2 = p2z - p0z;
    __int128 x3 = p3.x - p0.x;
    __int128 y3 = p3.y - p0.y;
    __int128 z3 = p3z - p0z;
    return x1 * y2 * z3
            - x1 * y3 * z2
            + y1 * z2 * x3
            - y1 * z3 * x2
            + z1 * x2 * y3
            - z1 * x3 * y2;
}

inline __int128 orient3d(point_t const& p0,
                         point_t const& p1,
                         point_t const& p2,
                         point_t const& p3)
{
    return orient3d_with_z(p0, hyperboloid_z(p0),
                           p1, hyperboloid_z(p1),
                           p2, hyperboloid_z(p2),
                           p3, hyperboloid_z(p3));
}

inline int turn3d(point_t const& p0,
                  point_t const& p1,
                  point_t const& p2,
                  point_t const& p3)
{
    __int128 det = orient3d(p0, p1, p2, p3);
    return det < 0 ? -1 : det == 0 ? 0 : 1;
}

struct face_t {
    int p[3]; // indices
    int adj[3]; // adj[i] - index of face that lies opposite point with index p[i]
    vector<int> killers; // indices
};

ostream& operator<<(ostream& os, face_t const& f) {
    os << "face("
        << "points{" << f.p[0] + 1 << ", " << f.p[1] + 1 << ", " << f.p[2] + 1 << "}, "
        << "adjacent{" << f.adj[0] + 1 << ", " << f.adj[1] + 1 << ", " << f.adj[2] + 1 << "}, ";
    os << "killers{";
    string delim = "";
    for (int const& i : f.killers) {
        os << delim << i;
        delim = ", ";
    }
    os << "}";
    return os;
}

void orient_face(face_t& f, point_t const& p, vector<point_t> const& points) {
    if (turn3d(points[f.p[0]], points[f.p[1]], points[f.p[2]], p) == -1) {
       swap(f.p[1], f.p[2]);
       swap(f.adj[1], f.adj[2]);
    }
}

bool is_face_up_hull(face_t const& f, vector<point_t> const& points) {
    return turn2d(points[f.p[0]], points[f.p[1]], points[f.p[2]]) == -1;
}

// first find 3 vertices
// p0 - most bottom-then-left point
// p1 - most clockwise point from p0
// p2 - most counterwise point from p0
// p3 - any of most remote from plane (p0, p1, p2)
//
// if p0, p1, p2, p3 lies on single plane,
// faces filled with a single face with only correct value p[0]:
// single index of most bottom-then-left point (p0)
//
// otherwise it filled with 4 initial faces
void delone_init(vector<point_t> const& p, vector<face_t>& faces) {
    int i0 = 0;
    for (size_t i = 1; i < p.size(); ++i) {
        if (p[i].y < p[i0].y || p[i].y == p[i0].y && p[i].x < p[i0].x) {
            i0 = i;
        }
    }
    int i1 = 0;
    int i2 = 0;
    for (size_t i = 0; i < p.size(); ++i) {
        int t1 = turn2d(p[i0], p[i1], p[i]);
        if (t1 == -1 || t1 == 0 && sdist(p[i0], p[i]) > sdist(p[i0], p[i1])) {
            i1 = i;
        }
        int t2 = turn2d(p[i0], p[i2], p[i]);
        if (t2 == 1 || t2 == 0 && sdist(p[i0], p[i]) > sdist(p[i0], p[i2])) {
            i2 = i;
        }
    }
    int i3 = 0;
    __int128 det_max = abs(orient3d(p[i0], p[i1], p[i2], p[i3]));
    for (size_t i = 0; i < p.size(); ++i) {
        __int128 det = abs(orient3d(p[i0], p[i1], p[i2], p[i]));
        if (det > det_max) {
            i3 = i;
            det_max = det;
        }
    }
    if (det_max == 0) {
        faces.resize(1);
        faces[0].p[0] = i0;
        return;
    }

    faces.resize(4);
    faces[0].p[0] = i0;
    faces[0].p[1] = i1;
    faces[0].p[2] = i2;
    faces[0].adj[0] = 2;
    faces[0].adj[1] = 3;
    faces[0].adj[2] = 1;
    orient_face(faces[0], p[i3], p);

    faces[1].p[0] = i0;
    faces[1].p[1] = i1;
    faces[1].p[2] = i3;
    faces[1].adj[0] = 2;
    faces[1].adj[1] = 3;
    faces[1].adj[2] = 0;
    orient_face(faces[1], p[i2], p);

    faces[2].p[0] = i1;
    faces[2].p[1] = i2;
    faces[2].p[2] = i3;
    faces[2].adj[0] = 3;
    faces[2].adj[1] = 1;
    faces[2].adj[2] = 0;
    orient_face(faces[2], p[i0], p);

    faces[3].p[0] = i0;
    faces[3].p[1] = i2;
    faces[3].p[2] = i3;
    faces[3].adj[0] = 2;
    faces[3].adj[1] = 1;
    faces[3].adj[2] = 0;
    orient_face(faces[3], p[i1], p);
}

bool is_conflicts(int const pi,
                  int const fi,
                  vector<point_t> const& points,
                  vector<face_t> const& faces)
{
    face_t const& face = faces[fi];
    return turn3d(points[face.p[0]],
                  points[face.p[1]],
                  points[face.p[2]],
                  points[pi]
                  ) == -1;
}

int find_conflict_face(int const pi,
                       vector<point_t> const& points,
                       vector<face_t> const& faces)
{
    int cfi = -1;
    for (int i = 0; i < 4; ++i) {
        if (is_conflicts(pi, i, points, faces)) {
            cfi = i;
            break;
        }
    }
    if (cfi == -1)
        return -1;
    while (!faces[cfi].killers.empty()) {
        bool still_conflicts = false;
        for (int const& i : faces[cfi].killers) {
            if (is_conflicts(pi, i, points, faces)) {
                cfi = i;
                still_conflicts = true;
                break;
            }
        }
        if (!still_conflicts)
            return -1;
        
    }
    return cfi;
}

void restore_points_indices(vector<point_t>& points, unordered_map<point_t, int> const& index) {
    for (auto const& it : index) {
        points[it.second] = it.first;
    }
}

void delone_up_graph(vector<point_t>& points, vector<vector<int>>& res) {
    // res.clear();
    res.resize(points.size());
    if (points.size() == 2) {
        res[0].emplace_back(1);
        res[1].emplace_back(0);
        return;
    }

    unordered_map<point_t, int> original_point_index;
    for (size_t i = 0; i < points.size(); ++i) {
        original_point_index[points[i]] = i;
    }
    random_shuffle(points.begin(), points.end());
    random_shuffle(points.begin(), points.end());
    random_shuffle(points.begin(), points.end());
    vector<face_t> faces;
    delone_init(points, faces);
    if (faces.size() == 1) { // all points on signle plane
        swap(points[0], points[faces[0].p[0]]);
        sort(points.begin() + 1, points.end(), [&](point_t const& p1, point_t const& p2) {
            int const det = turn2d(points[0], p1, p2);
            return det == 1 || det == 0 && sdist(points[0], p1) <= sdist(points[0], p2);
        });
        if (turn2d(points[0], points[1], points[2]) == 0) { // all points on single line
            int i1 = original_point_index[points[0]];
            int i2 = original_point_index[points.back()];
            res[i1].emplace_back(i2);
            res[i2].emplace_back(i1);
        } else { // all points on single circle
            int i0 = original_point_index[points[0]];
            int i_cur = original_point_index[points[1]];
            for (size_t i = 1; i + 1 < points.size(); ++i) {
                int i_next = original_point_index[points[i + 1]];
                res[i0].emplace_back(i_cur);
                res[i_cur].emplace_back(i0);
                res[i_cur].emplace_back(i_next);
                res[i_next].emplace_back(i_cur);
                i_cur = i_next;
            }
            res[i0].emplace_back(i_cur);
            res[i_cur].emplace_back(i0);
        }
        restore_points_indices(points, original_point_index);
        return;
    }
    vector<int> waitin(points.size(), -1);
    for (size_t i = 0; i < points.size(); ++i) {
        int cfi = find_conflict_face(i, points, faces); // conflict face index
        if (cfi == -1)
            continue;
        vector<int> killers;
        set<int> victims;
        queue<int> q;
        q.push(cfi);
        victims.insert(cfi);
        while (!q.empty()) {
            int const fi = q.front(); // face index
            q.pop();
            for (int ci = 0; ci < 3; ++ci) { // corner index
                int const nfi = faces[fi].adj[ci]; // neighbour face index
                if (is_conflicts(i, nfi, points, faces)) {
                    if (victims.insert(nfi).second) {
                        q.push(nfi);
                    }
                } else {
                    for (int nfoci = 0; nfoci < 3; ++nfoci) { // neighbour face's opposite corner index
                        if (faces[nfi].adj[nfoci] == fi) {
                            faces[nfi].adj[nfoci] = faces.size();
                            break;
                        }
                    }
                    killers.emplace_back(faces.size());

                    faces.emplace_back();
                    faces.back().p[0] = faces[fi].p[0];
                    faces.back().p[1] = faces[fi].p[1];
                    faces.back().p[2] = faces[fi].p[2];
                    faces.back().p[ci] = i;
                    faces.back().adj[ci] = nfi;
                    set<int> basement({0, 1, 2}); // corner indices without ci
                    basement.erase(ci);
                    for (int const& nfci : basement) { // new face's corner index
                        int const& bpi = faces.back().p[nfci]; // basement point index
                        if (waitin[bpi] == -1) {
                            waitin[bpi] = faces.size() - 1;
                        } else {
                            int const bnfi = waitin[bpi]; // basement's neighbour face index
                            waitin[bpi] = -1;
                            int c1 = 0;
                            while (c1 == nfci || faces.back().p[c1] == i) {
                                ++c1;
                            }
                            int c2 = 0;
                            while (faces[bnfi].p[c2] == i || faces[bnfi].p[c2] == bpi) {
                                ++c2;
                            }
                            faces.back().adj[c1] = bnfi;
                            faces[bnfi].adj[c2] = faces.size() - 1;
                        }
                    }
                    orient_face(faces.back(), points[faces[fi].p[ci]], points);
                }
            }
        }
        for (int const& fi : victims) {
            faces[fi].killers.insert(faces[fi].killers.end(), killers.begin(), killers.end());
        }
    }
    // faces traversal
    int sfi = 0;
    while (!faces[sfi].killers.empty()) {
        sfi = faces[sfi].killers[0];
    }
    queue<int> q;
    vector<bool> used(faces.size(), false);
    q.push(sfi);
    used[sfi] = true;
    while (!q.empty()) {
        face_t const& f = faces[q.front()];
        q.pop();
        if (is_face_up_hull(f, points)) {
            int const v1 = original_point_index.find(points[f.p[0]])->second;
            int const v2 = original_point_index.find(points[f.p[1]])->second;
            int const v3 = original_point_index.find(points[f.p[2]])->second;
            res[v1].emplace_back(v2);
            res[v1].emplace_back(v3);
            res[v2].emplace_back(v1);
            res[v2].emplace_back(v3);
            res[v3].emplace_back(v1);
            res[v3].emplace_back(v2);
        }

        for (int ci = 0; ci < 3; ++ci) {
             if (!used[f.adj[ci]]) {
                 q.push(f.adj[ci]);
                 used[f.adj[ci]] = true;
             }
        }
    }
    restore_points_indices(points, original_point_index);
}

struct halfplane {
    ll A;
    ll B;
    ll C;

    ll subst(point_t const& p) const {
        return A * p.x + B * p.y + C;
    }
};

ostream& operator<<(ostream& os, halfplane const& hp) {
    os << "half plane " << hp.A << " " << hp.B << " " << hp.C;
    return os;
}

ll turn_hps(halfplane const& hp1, halfplane const& hp2) {
    return hp1.A * hp2.B - hp2.A * hp1.B;
}

pair<long double, long double> intersect(halfplane const& hp1, halfplane const& hp2) {
    long double x = (long double) hp1.B * hp2.C - (long double) hp2.B * hp1.C;
    long double y = (long double) hp1.A * hp2.C - (long double) hp2.A * hp1.C;
    long double w = (long double) hp1.A * hp2.B - (long double) hp2.A * hp1.B;
    return make_pair(x / w, y / w);
}

long double real_triangle_square(pair<long double, long double> const& p1,
                                 pair<long double, long double> const& p2,
                                 pair<long double, long double> const& p3)
{
    long double vx = p2.first - p1.first;
    long double vy = p2.second - p1.second;
    long double ux = p3.first - p1.first;
    long double uy = p3.second - p1.second;
    return abs(vx * uy - ux * vy) / 2;
}

bool pred(halfplane const& hp1, halfplane const& hp2, halfplane const& hp3, bool& e) {
    __int128 x = (__int128) hp1.B * hp3.C - (__int128) hp3.B * hp1.C;
    __int128 y = -((__int128) hp1.A * hp3.C - (__int128) hp3.A * hp1.C);
    __int128 w = (__int128) hp1.A * hp3.B - (__int128) hp3.A * hp1.B;
    __int128 s = hp2.A * x + hp2.B * y + hp2.C * w;
    if (s > 0 && w <= 0 || s == 0 && w <= 0) {
        e = true;
        // cout << "empty intersection" << endl;
        // cout << hp1 << endl << hp2 << endl << hp3 << endl;
        // cout << "w = " << ((ll) w) << endl << "t = " << ((ll) s) << endl;
        // cout << "x = " << ((long double) x / w) << endl;
        // cout << "y = " << ((long double) y / w) << endl;
    }
    return s > 0 || (s == 0 && w > 0);
}

long double bound_farthest_point_diagram(point_t const& p0,
                                         vector<int> const& edges,
                                         vector<point_t> const& points,
                                         point_t const& bb1,
                                         point_t const& bb2)
{
    if (edges.empty()) {
        return 0;
    }
    deque<halfplane> hps(edges.size());
    // 0 - bottom side
    // 1 - right side
    // 2 - up side
    // 3 - left side
    bool dead_side[4] = {false, false, false, false};
    for (size_t i = 0; i < edges.size(); ++i) {
        auto const v = points[edges[i]] - p0;
        hps[i].A = 2 * v.x;
        hps[i].B = 2 * v.y;
        auto const mid = points[edges[i]] + p0;
        hps[i].C = -(mid % v);
        assert(hps[i].A * mid.x / 2 + hps[i].B * mid.y / 2 + hps[i].C == 0);
        if (hps[i].A == 0) {
            if (hps[i].B > 0) {
                dead_side[0] = true;
            } else {
                dead_side[2] = true;
            }
        } else if (hps[i].B == 0) {
            if (hps[i].A < 0) {
                dead_side[1] = true;
            } else {
                dead_side[3] = true;
            }
        }
    }
    halfplane rect_hps[4] = {
        halfplane{0, 1, -bb1.y},
        halfplane{-1, 0, bb2.x},
        halfplane{0, -1, bb2.y},
        halfplane{1, 0, -bb1.x}
    };
    for (int i = 0; i < 4; ++i) {
        if (dead_side[i])
            continue;
        bool inserted = false;
        for (size_t j = 1; j < hps.size(); ++j) {
            if (turn_hps(hps[j - 1], rect_hps[i]) > 0 && turn_hps(rect_hps[i], hps[j]) > 0) {
                hps.insert(hps.begin() + j, rect_hps[i]);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            if (turn_hps(hps.back(), rect_hps[i]) > 0) {
                hps.push_back(rect_hps[i]);
            } else {
                hps.insert(hps.begin(), rect_hps[i]);
            }
        }
    }
    bool empty_intersection = false;
    deque<halfplane> deq;
    for (auto const& hp : hps) {
        while (deq.size() >= 2 && pred(deq[deq.size() - 2], deq.back(), hp, empty_intersection)) {
            deq.pop_back();
        }
        while (deq.size() >= 2 && pred(hp, deq[0], deq[1], empty_intersection)) {
            deq.pop_front();
        }
        if (deq.size() < 2 || !pred(deq.back(), hp, deq[0], empty_intersection)) {
            deq.push_back(hp);
        }
        if (empty_intersection) {
            return 0;
        }
    }
    assert(deq.size() >= 3);
    long double res = 0;
    auto pt0 = intersect(deq.back(), deq[0]);
    auto ppt = intersect(deq[0], deq[1]);
    for (size_t i = 1; i + 1 < deq.size(); ++i) {
        auto pt = intersect(deq[i], deq[i + 1]);
        res += real_triangle_square(pt0, ppt, pt);
        ppt = pt;
    }
    assert(!isnan(res));
    return res;
}

int main() {
    srand(random_device()());

    int x1, y1, x2, y2;
    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
    int n;
    scanf("%d", &n);
    if (n == 1) {
        printf("%.42Lf\n", (long double) 1.0);
        return 0;
    }

    vector<point_t> points(n);
    vector<vector<int>> g;
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }
    delone_up_graph(points, g);

    for (size_t i = 0; i < g.size(); ++i) {
        point_t const& pi = points[i];
        sort(g[i].begin(), g[i].end(), [&](int const& i1, int const& i2) {
            return turn2d(pi, points[i1], points[i2]) == 1;
        });
        g[i].resize(unique(g[i].begin(), g[i].end()) - g[i].begin());
    }

    long double bb_square = (ll) (x2 - x1) * (ll) (y2 - y1);
    for (size_t i = 0; i < g.size(); ++i) {
        auto square = bound_farthest_point_diagram(points[i], g[i], points,
                point_t{x1, y1},
                point_t{x2, y2});
        printf("%.42Lf\n", square / bb_square);
    }

    return 0;
}
