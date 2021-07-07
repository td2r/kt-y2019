#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 100'000 + 1;
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

bool down(int L, int R) {
    assert(node[cur].depth >= R - L);
    int ch = (int) s[R] - 'a';
    int cur_parent = node[cur].parent;
    if (node[cur].depth > R - L) {
        int edgeShift = R - L - node[cur_parent].depth;
        int letterOnEdge = (int) s[node[cur].index + edgeShift] - 'a';
        if (letterOnEdge != ch) {
            node[m] = node_t(R - L, node[cur].index, cur_parent);
            node[cur_parent].link[(int) s[node[cur].index] - 'a'] = m;
            node[cur].parent = m;
            node[cur].index += edgeShift;
            node[m].link[letterOnEdge] = cur;
            cur = m++;
        }
    }
    if (node[cur].depth == R - L) {
        if (node[cur].link[ch] == -1) {
            node[m] = node_t(INF, R, cur);
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
    bool quest = false;
    while (r < n) {
        if (s[r] == '?') {
            quest = true;
            if (node[cur].depth > r - l) {
                s[r] = s[node[cur].index + r - l - node[node[cur].parent].depth];
            } else {
                int ch = 0;
                while (ch < ALPHA && node[cur].link[ch] == -1) {
                    ++ch;
                }
                ch %= ALPHA;
                s[r] = (char) (ch + 'a');
            }
        }
        while (l <= r) {
            bool created = down(l, r);
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
                down(l, l + node[cur].depth);
            }
        }
        ans += leaves;
        if (quest) {
            printf("%c %lld\n", s[r], ans);
            quest = false;
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

bool find_str(char const* str, int v, int pos, int len) {
    if (pos == len) {
        return true;
    }
    int ch = (int) str[pos] - 'a';
    int to = node[v].link[ch];
    if (to == -1) {
        return false;
    }
    int i = node[to].index;
    int remains = min(node[to].depth - node[v].depth, len - pos);
    while (remains > 0 && i < n && s[i] == str[pos]) {
        ++i, ++pos, --remains;
    }
    if (remains > 0) {
        return false;
    }
    return find_str(str, to, pos, len);
}

int main() {
    init_suffix_tree();

    scanf("%d\n", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%c\n", &s[i]);
    }

    build_suffix_tree();

    return 0;
}
