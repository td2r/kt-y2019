#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t; // floating point number

struct object_t {
    vector<int> features;
    int clazz;
};

#ifdef DEBUG
ostream& operator<<(ostream& os, object_t const& o) {
    for (auto const& it : o.features) {
        cout << it << " ";
    }
    cout << o.clazz;
    return os;
}
#endif

static int node_index_global = 1;
struct node_t {
    int index;
    int id; // feature_id in node and class id in leaf
    floating_t b;
    int lv;
    int rv;
    
    explicit node_t(int id)
    : index(node_index_global++)
    , id(id)
    , lv(-1)
    , rv(-1)
    {}
    
    node_t(int id, floating_t b)
    : index(node_index_global++)
    , id(id)
    , b(b)
    {}
    
    void show() const {
        if (lv == -1) {
            cout << "C " << id << endl;
            // printf("C %d\n", id);
        } else {
            cout << fixed << setprecision(1) << "Q " << id + 1 << ' ' << b << ' ' << lv + 1 << ' ' << rv + 1 << endl;
            // printf("Q %d %.1lf %d %d\n", id + 1, b, lv + 1, rv + 1);
        }
    }
};

struct metric_t {
    using metric_function = floating_t(&)(vector<int> const&);
    int k; // # classes
    vector<int> cnt1;
    vector<int> cnt2;
    metric_function value_of;
    
    metric_t(int k, metric_function value_of) : k(k), cnt1(k), cnt2(k), value_of(value_of) {}
    
    void count_classes(vector<int>& cnt, vector<object_t> const& objects) const {
        fill(cnt.begin(), cnt.end(), 0);
        for (auto const& obj : objects) {
            ++cnt[obj.clazz - 1];
        }
    }
    
    pair<floating_t, floating_t> max_split(vector<object_t> const& objects, int const feature_id, int const l, int const r) {
        vector<object_t> sorted_objects(objects.begin() + l, objects.begin() + r);
        sort(sorted_objects.begin(), sorted_objects.end(),
            [feature_id](object_t const& o1, object_t const& o2) {
                return o1.features[feature_id] < o2.features[feature_id];
            });
        fill(cnt1.begin(), cnt1.end(), 0);
        count_classes(cnt2, sorted_objects);
        
        floating_t b = numeric_limits<floating_t>::quiet_NaN();
        floating_t min_score = numeric_limits<floating_t>::infinity();
        
        for (size_t i = 0; i + 1 < sorted_objects.size(); ++i) {
            ++cnt1[sorted_objects[i].clazz - 1];
            --cnt2[sorted_objects[i].clazz - 1];
            if (sorted_objects[i + 1].features[feature_id] == sorted_objects[i].features[feature_id])
                continue;
            floating_t a1 = (floating_t) (i + 1) / (floating_t) sorted_objects.size();
            floating_t a2 = (floating_t) (sorted_objects.size() - i - 1) / (floating_t) sorted_objects.size();
            floating_t score = a1 * value_of(cnt1) + a2 * value_of(cnt2);
            if (score < min_score) {
                b = (floating_t) (sorted_objects[i].features[feature_id] + sorted_objects[i + 1].features[feature_id]) / 2.0;
                min_score = score;
            }
        }
        return make_pair(b, min_score);
    }
};

floating_t gini_metric(vector<int> const& cnt) {
    floating_t cnt_sum = (floating_t) accumulate(cnt.begin(), cnt.end(), 0);
    floating_t value = 1.0;
    for (auto const& c : cnt) {
        floating_t p = (floating_t) c / cnt_sum;
        value -= p * p;
    }
    return value;
}

floating_t entropy_metric(vector<int> const& cnt) {
    floating_t cnt_sum = (floating_t) accumulate(cnt.begin(), cnt.end(), 0);
    floating_t value = 0.0;
    for (auto const& c : cnt) {
        if (c) {
            value += ((floating_t) c / cnt_sum) * log2(cnt_sum / (floating_t) c);
        }
    }
    return value;
}

int mode_by_class(int const l, int const r, vector<object_t> const& objects, metric_t& metric) {
    vector<int> cnt(metric.k, 0);
    for (int i = l; i < r; ++i) {
        ++cnt[objects[i].clazz - 1];
    }
    int d = -1;
    int max_cnt = -1;
    for (size_t i = 0; i < cnt.size(); ++i) {
        if (cnt[i] > max_cnt) {
            d = i;
            max_cnt = cnt[i];
        }
    }
    return d + 1;
}

bool all_objects_from_same_class(int const l, int const r, vector<object_t> const& objects) {
    int d = objects[l].clazz;
    for (int i = l + 1; i < r; ++i) {
        if (objects[i].clazz != d) {
            return false;
        }
    }
    return true;
}

// returns index in vertices of root node
int build_tree_helper(vector<node_t>& vertices,
                      vector<object_t>& objects,
                      metric_t& metric,
                      int d,
                      int l,
                      int r)
{
    int vi = (int) vertices.size();
    if (d == 0) {
        vertices.push_back(node_t(mode_by_class(l, r, objects, metric)));
    } else if (all_objects_from_same_class(l, r, objects)) {
        vertices.push_back(node_t(objects[l].clazz));
    } else {
        int features_cnt = objects[l].features.size();
        floating_t b = numeric_limits<floating_t>::quiet_NaN();
        floating_t min_score = numeric_limits<floating_t>::infinity();
        int feature_id = -1;
        for (int i = 0; i < features_cnt; ++i) {
            auto split = metric.max_split(objects, i, l, r);
            if (split.second < min_score) {
                b = split.first;
                min_score = split.second;
                feature_id = i;
            }
        }
        // assert(!isnan(b));
        if (isnan(b)) {
            // cout << "isnan(b)" << endl;
            // cout << "depth = " << d << endl;
            // cout << "objects:" << endl;
            // for (int i = l; i < r; ++i) {
                // cout << objects[i] << endl;
            // }
            // assert(false);
            vertices.push_back(node_t(mode_by_class(l, r, objects, metric)));
        } else {
            sort(objects.begin() + l, objects.begin() + r,
                [feature_id](object_t const& o1, object_t const& o2) {
                    return o1.features[feature_id] < o2.features[feature_id];
                });
            int x = upper_bound(objects.begin() + l, objects.begin() + r, b,
                [feature_id](floating_t const& feature_value, object_t const& o) {
                    return feature_value < o.features[feature_id];
                }) - objects.begin();
            assert(l < x);
            assert(x < r);
            vertices.push_back(node_t(feature_id, b));
            vertices[vi].lv = build_tree_helper(vertices, objects, metric, d - 1, l, x);
            vertices[vi].rv = build_tree_helper(vertices, objects, metric, d - 1, x, r);
        }
    }
    return vi;
}

vector<node_t> build_tree(vector<object_t>& objects, metric_t& metric, int const max_depth) {
    vector<node_t> vertices;
    build_tree_helper(vertices, objects, metric, max_depth, 0, (int) objects.size());
    return vertices;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int m;
    int k;
    int h;
    cin >> m >> k >> h;
    // scanf("%d %d %d\n", &m, &k, &h);
    int n;
    cin >> n;
    // scanf("%d\n", &n);
    vector<object_t> objects(n);
    for (int i = 0; i < n; ++i) {
        auto& obj = objects[i];
        obj.features.resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> obj.features[j];
            // scanf("%d ", &obj.features[j]);
        }
        cin >> obj.clazz;
        // scanf("%d\n", &obj.clazz);
    }
    auto metric = metric_t(k, n <= 1000 ? entropy_metric : gini_metric);
    auto vertices = build_tree(objects, metric, h);
    cout << vertices.size() << endl;
    // printf("%zd\n", vertices.size());
    for (auto const& v : vertices) {
        v.show();
    }
    return 0;
}
