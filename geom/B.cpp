#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int sqr(int x) {
    return x * x;
}

int sgn(ll x) {
    return x == 0 ? x : x < 0 ? -1 : 1;
}

// is vertex angle A acute
bool is_acute(int xa, int ya, int xb, int yb, int xc, int yc) {
    return sqr(xb - xc) + sqr(yb - yc) -
        sqr(xa - xb) - sqr(ya - yb) -
        sqr(xa - xc) - sqr(ya - yc) < 0; // cos theorem
};

ll vector_product(ll x, ll y, ll x1, ll y1, ll x2, ll y2) {
    return (x1 - x) * (y2 - y) - (x2 - x) * (y1 - y);
}

long double dot_dot_dist(int x1, int y1, int x2, int y2) {
    return hypotl((long double) (x1 - x2), (long double) (y1 - y2));
}

long double dot_segment_dist(int x, int y, int x1, int y1, int x2, int y2) {
    if (is_acute(x1, y1, x, y, x2, y2) && is_acute(x2, y2, x, y, x1, y1)) {
        return abs(vector_product(x, y, x1, y1, x2, y2)) / dot_dot_dist(x1, y1, x2, y2);
    } else {
        return min(dot_dot_dist(x, y, x1, y1), dot_dot_dist(x, y, x2, y2));
    }
}

long double dot_ray_dist(int x, int y, int x1, int y1, int x2, int y2) {
    if (is_acute(x1, y1, x, y, x2, y2)) {
        return abs(vector_product(x, y, x1, y1, x2, y2)) / dot_dot_dist(x1, y1, x2, y2);
    } else {
        return dot_dot_dist(x, y, x1, y1);
    }
}

long double dot_line_dist(int x, int y, int x1, int y1, int x2, int y2) {
    return abs(vector_product(x, y, x1, y1, x2, y2)) / dot_dot_dist(x1, y1, x2, y2);
}

bool in_segment(int x, int y, int x1, int y1, int x2, int y2) {
    if (x1 > x2 || x1 == x2 && y1 > y2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    return x1 <= x && x <= x2 && y1 <= y && y <= y2;
}

long double segment_segment_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int ra = sgn(vector_product(x3, y3, x4, y4, x1, y1));
    int rb = sgn(vector_product(x3, y3, x4, y4, x2, y2));
    int rc = sgn(vector_product(x1, y1, x2, y2, x3, y3));
    int rd = sgn(vector_product(x1, y1, x2, y2, x4, y4));
    bool intersect = false;
    if (ra == 0 && rb == 0 && rc == 0 && rd == 0) {
        intersect = (in_segment(x1, y1, x3, y3, x4, y4) || in_segment(x2, y2, x3, y3, x4, y4));
    } else {
        intersect = ((ra == 0 || rb == 0 || ra != rb) && (rc == 0 || rd == 0 || rc != rd));
    }
    if (intersect) {
        return 0;
    } else {
        return min({
                dot_segment_dist(x1, y1, x3, y3, x4, y4),
                dot_segment_dist(x2, y2, x3, y3, x4, y4),
                dot_segment_dist(x3, y3, x1, y1, x2, y2),
                dot_segment_dist(x4, y4, x1, y1, x2, y2)
                });
    }
}

bool in_ray(int x, int y, int x1, int y1, int x2, int y2) {
    return sgn(x - x1) == sgn(x2 - x1) && sgn(y - y1) == sgn(y2 - y1) ||
        x == x1 && y == y1;
}

long double segment_ray_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int ra = sgn(vector_product(x3, y3, x4, y4, x1, y1));
    int rb = sgn(vector_product(x3, y3, x4, y4, x2, y2));
    int rc = sgn(vector_product(x1, y1, x2, y2, x3, y3));
    bool intersect = false;
    if (ra == 0 && rb == 0 && rc == 0) {
        intersect = (in_ray(x1, y1, x3, y3, x4, y4) || in_ray(x2, y2, x3, y3, x4, y4));
    } else if (ra == 0) {
        intersect = (rb == rc);
    } else if (rb == 0) {
        intersect = (ra != rc);
    } else if (rc == 0) {
        intersect = in_segment(x3, y3, x1, y1, x2, y2);
    } else {
        intersect = (ra != rb && rc != ra);
    }
    if (intersect) {
        return 0;
    } else {
        return min({
                dot_ray_dist(x1, y1, x3, y3, x4, y4),
                dot_ray_dist(x2, y2, x3, y3, x4, y4),
                dot_segment_dist(x3, y3, x1, y1, x2, y2)
                });
    }
}

long double segment_line_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int ra = sgn(vector_product(x3, y3, x4, y4, x1, y1));
    int rb = sgn(vector_product(x3, y3, x4, y4, x2, y2));
    if (ra == 0 || rb == 0 || ra != rb) {
        return 0;
    } else {
        return min(dot_line_dist(x1, y1, x3, y3, x4, y4),
                   dot_line_dist(x2, y2, x3, y3, x4, y4));
    }
}

long double ray_ray_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int x5 = x1 + (x4 - x3);
    int y5 = y1 + (y4 - y3);
    int r132 = sgn(vector_product(x1, y1, x3, y3, x2, y2));
    int r125 = sgn(vector_product(x1, y1, x2, y2, x5, y5));
    int r135 = sgn(vector_product(x1, y1, x3, y3, x5, y5));
    bool intersect = false;
    if (r132 == 0 && r125 == 0 && r135 == 0) {
        intersect = (in_ray(x1, y1, x3, y3, x4, y4) || in_ray(x3, y3, x1, y1, x2, y2));
    } else if (r132 == 0) {
        intersect = in_ray(x3, y3, x1, y1, x2, y2);
    } else if (r135 == 0) {
        intersect = in_ray(x1, y1, x3, y3, x4, y4);
    } else {
        intersect = (r132 == r125 && r132 == r135);
    }
    if (intersect) {
        return 0;
    } else {
        return min(dot_ray_dist(x1, y1, x3, y3, x4, y4),
                   dot_ray_dist(x3, y3, x1, y1, x2, y2));
    }
}

long double ray_line_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int x5 = x1 + (x4 - x3);
    int y5 = y1 + (y4 - y3);
    int r2 = sgn(vector_product(x1, y1, x5, y5, x2, y2));
    int r3 = sgn(vector_product(x1, y1, x5, y5, x3, y3));
    if (r3 == 0 || r2 == r3) {
        return 0;
    } else {
        return dot_line_dist(x1, y1, x3, y3, x4, y4);
    }
}

long double line_line_dist(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    int x5 = x1 + (x4 - x3);
    int y5 = y1 + (y4 - y3);
    if (vector_product(x1, y1, x2, y2, x5, y5) != 0) {
        return 0;
    } else {
        return dot_line_dist(x1, y1, x3, y3, x4, y4);
    }
}

int main() {
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x4, y4;
    cin >>
        x1 >> y1 >>
        x2 >> y2 >>
        x3 >> y3 >>
        x4 >> y4;
    cout << fixed << setprecision(20) <<
        dot_dot_dist(x1, y1, x3, y3) << endl <<
        dot_segment_dist(x1, y1, x3, y3, x4, y4) << endl <<
        dot_ray_dist(x1, y1, x3, y3, x4, y4) << endl <<
        dot_line_dist(x1, y1, x3, y3, x4, y4) << endl <<
        
        dot_segment_dist(x3, y3, x1, y1, x2, y2) << endl <<
        segment_segment_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl <<
        segment_ray_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl <<
        segment_line_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl <<

        dot_ray_dist(x3, y3, x1, y1, x2, y2) << endl <<
        segment_ray_dist(x3, y3, x4, y4, x1, y1, x2, y2) << endl <<
        ray_ray_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl <<
        ray_line_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl <<

        dot_line_dist(x3, y3, x1, y1, x2, y2) << endl <<
        segment_line_dist(x3, y3, x4, y4, x1, y1, x2, y2) << endl <<
        ray_line_dist(x3, y3, x4, y4, x1, y1, x2, y2) << endl <<
        line_line_dist(x1, y1, x2, y2, x3, y3, x4, y4) << endl;
    return 0;
}
