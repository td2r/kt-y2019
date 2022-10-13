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

namespace std {
    template<>
    struct hash<point_t> {
        size_t operator()(point_t const& p) const {
            return 31 * (size_t) p.x + (size_t) p.y;
        }
    };
}

// dist ** 2
inline int sdist(point_t const& p1, point_t const& p2) {
    point_t const d = p2 - p1;
    return d % d;
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

inline ll hyperboloid_z(point_t const& p) {
    ll x = p.x;
    ll y = p.y;
    return x * x + y * y;
}

inline __int128 orient3d(point_t const& p0,
                         point_t const& p1,
                         point_t const& p2,
                         point_t const& p3)
{
    __int128 z0 = hyperboloid_z(p0);
    __int128 x1 = p1.x - p0.x;
    __int128 y1 = p1.y - p0.y;
    __int128 z1 = hyperboloid_z(p1) - z0;
    __int128 x2 = p2.x - p0.x;
    __int128 y2 = p2.y - p0.y;
    __int128 z2 = hyperboloid_z(p2) - z0;
    __int128 x3 = p3.x - p0.x;
    __int128 y3 = p3.y - p0.y;
    __int128 z3 = hyperboloid_z(p3) - z0;
    return x1 * y2 * z3
            - x1 * y3 * z2
            + y1 * z2 * x3
            - y1 * z3 * x2
            + z1 * x2 * y3
            - z1 * x3 * y2;
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

vector<face_t> delone(vector<point_t>& points) {
    random_shuffle(points.begin(), points.end());
    random_shuffle(points.begin(), points.end());
    random_shuffle(points.begin(), points.end());
    vector<face_t> faces;
    delone_init(points, faces);
    if (faces.size() == 1) {
        swap(points[0], points[faces[0].p[0]]);
        sort(points.begin() + 1, points.end(), [&](point_t const& p1, point_t const& p2) {
            int const det = turn2d(points[0], p1, p2);
            return det == 1 || det == 0 && sdist(points[0], p1) <= sdist(points[0], p2);
        });
        faces.resize(points.size() - 2);
        for (size_t i = 1; i + 1 < points.size(); ++i) {
            faces[i - 1].p[0] = 0;
            faces[i - 1].p[1] = i;
            faces[i - 1].p[2] = i + 1;
            faces[i - 1].adj[0] = i - 1;
            faces[i - 1].adj[1] = i;
            faces[i - 1].adj[2] = i - 2;
        }
        faces[0].adj[2] = 0;
        faces.back().adj[1] = faces.size() - 1;
        return faces;
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
    return faces;
}

void build_triangulation_graph(
        vector<face_t> const& faces,
        vector<point_t> const& points,
        unordered_map<point_t, int> const& original_point_index,
        vector<vector<int>>& g)
{
    g.resize(points.size());
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
        int const v1 = original_point_index.find(points[f.p[0]])->second;
        int const v2 = original_point_index.find(points[f.p[1]])->second;
        int const v3 = original_point_index.find(points[f.p[2]])->second;
        g[v1].emplace_back(v2);
        g[v1].emplace_back(v3);
        g[v2].emplace_back(v1);
        g[v2].emplace_back(v3);
        g[v3].emplace_back(v1);
        g[v3].emplace_back(v2);

        for (int ci = 0; ci < 3; ++ci) {
             if (!used[f.adj[ci]]) {
                 q.push(f.adj[ci]);
                 used[f.adj[ci]] = true;
             }
        }
    }
    for (size_t i = 0; i < g.size(); ++i) {
        sort(g[i].begin(), g[i].end());
        auto const it = unique(g[i].begin(), g[i].end());
        g[i].resize(it - g[i].begin());
    }
}

// Prim's algorithm
void build_mst(vector<vector<int>> const& g,
                             vector<point_t> const& points,
                             vector<int>& p)
{
    // p.clear();
    p.resize(g.size(), -1);
    vector<bool> in_mst(p.size(), false);
    in_mst[0] = true;
    vector<ll> min_w(p.size(), LLONG_MAX);
    min_w[0] = 0;
    set<pair<int, int>> q({make_pair(min_w[0], 0)});
    while (!q.empty()) {
        int const v = q.begin()->second;
        q.erase(q.begin());
        in_mst[v] = true;
        for (int const& u : g[v]) {
            if (in_mst[u])
                continue;
            ll w = sdist(points[v], points[u]);
            if (w < min_w[u]) {
                q.erase(make_pair(min_w[u], u));
                min_w[u] = w;
                p[u] = v;
                q.insert(make_pair(min_w[u], u));
            }
        }
    }
}

int binlog(size_t n) {
    size_t res = 1;
    while (n) {
        n >>= 1;
        ++res;
    }
    return res;
}

void calculate_binary_lifting(vector<int> const& p,
                              vector<point_t> const& points,
                              vector<int>& depth,
                              vector<vector<pair<int, ll>>>& up)
{
    assert(p[0] == -1);

    depth.resize(p.size(), -1);
    depth[0] = 0;
    up.resize(p.size());
    up[0].resize(binlog(p.size()), make_pair(0, 0));
    for (size_t i = 1; i < up.size(); ++i) {
        up[i].resize(up[0].size());
        up[i][0] = make_pair(p[i], sdist(points[i], points[p[i]]));
        if (p[i] == 0) {
            depth[i] = 1;
        }
    }
    for (size_t j = 1; j < up[0].size(); ++j) {
        for (size_t i = 1; i < up.size(); ++i) {
            if (depth[i] == -1) {
                int const ad = depth[up[i][j - 1].first]; // ancestor's depth
                if (ad != -1)
                    depth[i] = ad + (1 << (j - 1));
            }
            ll value = up[i][j - 1].second;
            int a = up[i][j - 1].first;
            value = max(value, up[a][j - 1].second);
            a = up[a][j - 1].first;
            up[i][j] = make_pair(a, value);
        }
    }
    for (size_t i = 0; i < depth.size(); ++i) {
        assert(depth[i] != -1);
    }
}

int main() {
    srand(random_device()());

    int n;
    scanf("%d", &n);

    vector<int> depth;
    vector<vector<pair<int, ll>>> up;
    {
        vector<int> p; // tree
        vector<point_t> points(n);
        {
            vector<vector<int>> g;
            {
                unordered_map<point_t, int> original_point_index;
                for (int i = 0; i < n; ++i) {
                    scanf("%d %d", &points[i].x, &points[i].y);
                    original_point_index[points[i]] = i;
                }

                auto faces = delone(points);
                build_triangulation_graph(faces, points, original_point_index, g);
                for (auto const& it : original_point_index) {
                    points[it.second] = it.first;
                }
#ifdef DEBUG
                cout << "graph" << endl;
                for (size_t i = 0; i < g.size(); ++i) {
                    for (int const& j : g[i]) {
                        cout << i + 1 << " " << j + 1 << endl;
                    }
                }
#endif
            }
            build_mst(g, points, p);
#ifdef DEBUG
            cout << "tree" << endl;
            for (size_t i = 0; i < p.size(); ++i) {
                cout << i + 1 << " " << p[i] + 1 << endl;
            }
#endif
        }
        calculate_binary_lifting(p, points, depth, up);
#ifdef DEBUG
        cout << "binary_lifting" << endl;
        for (size_t i = 0; i < depth.size(); ++i) {
            cout << "depth[" << i + 1 << "] = " << depth[i] << endl;
        }
        for (size_t j = 0; j < up[0].size(); ++j) {
            for (size_t i = 0; i < up.size(); ++i) {
                cout << "up[" << i + 1 << "][" << j << "] = "
                    << up[i][j].first + 1 << ", " << up[i][j].second << endl;
            }
        }
#endif
    }

    int q;
    scanf("%d", &q);
    while (q--) {
        int u, v;
        scanf("%d %d", &u, &v);
        --u; --v;
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        ll ans = 0;
        int dd = depth[v] - depth[u]; // depth difference
        for (int i = 0; dd; ++i) {
            if (dd & 1) {
                ans = max(ans, up[v][i].second);
                v = up[v][i].first;
            }
            dd >>= 1;
        }
        for (int i = up[0].size() - 1; i >= 0; --i) {
            if (up[u][i].first != up[v][i].first) {
                ans = max({ans, up[u][i].second, up[v][i].second});
                u = up[u][i].first;
                v = up[v][i].first;
            }
        }
        if (u != v) {
            ans = max({ans, up[u][0].second, up[v][0].second});
            assert(up[u][0].first == up[v][0].first);
        }
        printf("%.42Lf\n", sqrtl(ans));
    }
    return 0;
}
