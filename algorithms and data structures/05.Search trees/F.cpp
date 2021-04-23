#include <bits/stdc++.h>
#define debug false
#define mp make_pair
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
using namespace std;

template<typename T1, typename T2>
ostream& operator << (ostream &os, pair<T1, T2> &p) {
    os << '<' << p.first << ' ' << p.second << '>';
    return os;
}

template<typename T>
ostream& operator << (ostream &os, vector<T> &v) {
    for (int i = 0; i < v.size(); ++i) {
        if (i) os << ' ';
        os << v[i];
    }
    return os;
}

template<typename T>
void showArray(const string arrName, T* beginIterator, T* endIterator) {
    cout << arrName << " = [";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}

template<class forwardIterator>
void show_container(forwardIterator beginIterator, forwardIterator endIterator) {
    cout << "[";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}

template<class forwardIterator>
void show_container(const string cname,
        forwardIterator beginIterator,
        forwardIterator endIterator) {
    cout << cname << " = [";
    if (beginIterator != endIterator) {
        cout << *beginIterator++;
    }
    while (beginIterator != endIterator) {
        cout << ", " << *beginIterator++;
    }
    cout << "]" << endl;
}




struct Node {
    int x, y, sz;
    Node *left, *right;

    Node() : x(0), y(rand() | (rand() << 16)), sz(1),
        left(nullptr), right(nullptr) {}

    Node(int x) : x(x), y(rand() | (rand() << 16)), sz(1),
        left(nullptr), right(nullptr) {}
};

inline int getsize(Node* root) {
    return root ? root->sz : 0;
}

inline void recalc(Node* root) {
    if (root) root->sz = getsize(root->left) + 1 + getsize(root->right);
}

Node* make_treap(int n) {
    if (!n) return nullptr;

    stack<Node*> st;
    st.push(new Node(INT_MIN));
    st.top()->y = INT_MIN;

    for (int i = 1; i <= n; ++i) {
       Node *node = new Node(0);

       while (st.top()->y > node->y) {
           recalc(st.top());
           st.pop();
       }

       if (st.top()->right) {
           node->left = st.top()->right;
       }
       st.top()->right = node;

       st.push(node);
    }

    while (st.size() > 2) {
       recalc(st.top());
       st.pop();
    }
    Node *root = st.top();
    recalc(root);
    st.pop();
    delete st.top();

    return root;
}

Node* merge(Node* a, Node* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->y < b->y) {
        a->right = merge(a->right, b);
        recalc(a);
        return a;
    } else {
        b->left = merge(a, b->left);
        recalc(b);
        return b;
    }
}

pair<Node*, Node*> split(Node* root, int k) {
    if (!root) return mp(nullptr, nullptr);
    if (getsize(root->left) < k) {
        auto spl = split(root->right, k - getsize(root->left) - 1);
        root->right = spl.first;
        recalc(root);
        return mp(root, spl.second);
    } else {
        auto spl = split(root->left, k);
        root->left = spl.second;
        recalc(root);
        return mp(spl.first, root);
    }
}

Node* insert(Node* root, Node* node, int k) {
    if (!root) return node;
    auto spl = split(root, k - 1);
    return merge(spl.first, merge(node, spl.second));
}

Node* remove(Node* root, int k) {
    if (!root) return root;
    auto spl2 = split(root, k);
    auto spl1 = split(spl2.first, k - 1);
    delete spl1.second;
    return merge(spl1.first, spl2.second);
}

const int MAXM = (1 << 18) + 2;

int parent[MAXM], W = 0;

int find_set(int x) {
    return x == parent[x] ? x : parent[x] = find_set(parent[x]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    parent[a] = b;
}

void print(Node*);
void showTreap(Node*);

Node* insert(int l, int k, Node* root) {
    int w = find_set(l);
    root = remove(root, w);
    root = insert(root, new Node(k), l);
    union_sets(w, w + 1);
    W = max(W, w);
    return root;
}

int showTreap(Node* root, int k) {
    if (!root) return 0;
    int res = showTreap(root->left, k);
    if (res < k) {
        printf("%d ", root->x);
        ++res;
    }
    return showTreap(root->right, k - res) + res;
}


int main() {
    srand(random_device()());

    iota(parent, parent + MAXM, 0);

    int n, m;
    Node *root = make_treap(MAXM);

    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        int l;
        scanf("%d", &l);
        root = insert(l, i, root);
    }

    printf("%d\n", W);
    showTreap(root, W);
    printf("\n");

    return 0;
}
