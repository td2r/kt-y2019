#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

// int const MAXN = 15'728'640 + 1;
int const MAXN = 200'000 + 1;
int const ALPHA = 'z' - 'a' + 1;
int const INF = INT_MAX;

template <class forwardIterator>
void print_array(string const& name, forwardIterator first, forwardIterator last) {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

struct node_t {
    int depth;
    int index;
    int parent;
    int suflink;
    int link[ALPHA];

    node_t()
    : depth(0)
    , index(-1)
    , parent(-1)
    , suflink(-1)
    {
        memset(link, -1, sizeof(int) * ALPHA);
    }

    node_t(int depth, int index, int parent)
    : depth(depth)
    , index(index)
    , parent(parent)
    , suflink(-1)
    {
        memset(link, -1, sizeof(int) * ALPHA);
    }

    void print(int i) const {
        printf("node[%d] = { depth=%d, index=%d, parent=%d, suflink=%d\n",
                i + 1, depth, index + 1, parent + 1, suflink + 1);
    }
};

int n;
char s[MAXN];
int m;
node_t node[2 * MAXN];
int cur;
int no_suflink;
int l, r;

ll ans = 0;
int leaves = 0;

bool down(int l, int r, int& cur) {
    assert(node[cur].depth >= r - l);
    int ch = (int) s[r] - 'a';
    int cur_parent = node[cur].parent;
    if (node[cur].depth > r - l) {
        int edgeShift = r - l - node[cur_parent].depth;
        int letterOnEdge = (int) s[node[cur].index + edgeShift] - 'a';
        if (letterOnEdge != ch) {
            node[m] = node_t(r - l, node[cur].index, cur_parent);
            node[cur_parent].link[(int) s[node[cur].index] - 'a'] = m;
            node[cur].parent = m;
            node[cur].index += edgeShift;
            node[m].link[letterOnEdge] = cur;
            cur = m++;
        }
    }
    if (node[cur].depth == r - l) {
        if (node[cur].link[ch] == -1) {
            node[m] = node_t(INF, r, cur);
            node[cur].link[ch] = m;
            cur = m++;
            ++leaves;
            return true;
        } else {
            cur = node[cur].link[ch];
        }
    }
    return false;
}

void build_suffix_tree() {
    while (r < n) {
        while (l <= r) {
            bool created = down(l, r, cur);
            if (no_suflink != -1) {
                node[no_suflink].suflink = node[cur].parent;
                no_suflink = -1;
            }
            if (!created) {
                break;
            }
            cur = node[cur].parent;
            if (node[cur].suflink == -1) {
                no_suflink = cur;
                cur = node[cur].parent;
            }
            cur = node[cur].suflink;
            l++;
            while (node[cur].depth < r - l) {
                down(l, l + node[cur].depth, cur);
            }
        }
        ++r;
    }
}

void init_suffix_tree() {
    cur = 0;
    no_suflink = -1;
    node[0] = node_t();
    node[0].suflink = 0;
    m = 1;
    l = 0;
    r = 0;
}

int main() {
    init_suffix_tree();

    scanf("%s", s);
    int len = strlen(s);

    for (n = 1; n <= len; ++n) {
        build_suffix_tree();
        ans += leaves;
        printf("%lld\n", ans);
    }

    return 0;
}
