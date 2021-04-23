#include <bits/stdc++.h>
#define debug true
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
    Node *left, *right;
    int x, y;
    ll sum;


    Node(int x) :
        left(nullptr), right(nullptr),
        x(x), y(rand() | (rand() << 16)), sum(x) {}
};


ll getsum(Node* root) {
    return root ? root->sum : 0;
}


void recalc(Node* root) {
    if (!root) return;
    root->sum = getsum(root->left) + root->x + getsum(root->right);
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
    if (root->x <= k) {
        auto spl = split(root->right, k);
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

Node* find(Node* root, int k) {
    if (!root) return root;
    if (k < root->x) {
        return find(root->left, k);
    } else if (k == root->x) {
        return root;
    } else {
        return find(root->right, k);
    }
}

Node* insert(Node* root, Node* node) {
    if (!root) return node;
    if (root->y < node->y) {
        if (node->x < root->x) {
            root->left = insert(root->left, node);
        } else {
            root->right = insert(root->right, node);
        }
        root->sum += node->x;
        return root;
    } else {
        auto spl = split(root, node->x);
        node->left = spl.first;
        node->right = spl.second;
        recalc(node);
        return node;
    }
}

ll sum(Node* root, int l, int r) {
    auto spl1 = split(root, l - 1);
    auto spl2 = split(spl1.second, r);
    ll res = getsum(spl2.first);
    spl1.second = merge(spl2.first, spl2.second);
    root = merge(spl1.first, spl1.second);
    return res;
}


int main() {
    const ll mod = 1e9;
    int q;
    ll y = 0;
    Node* root = nullptr;

    scanf("%d", &q);
    while (q--) {
        char c;
        int a, b;
        scanf("\n%c %d", &c, &a);
        if (c == '+') {
            if (!find(root, (int)((a + y) % mod))) {
                root = insert(root, new Node((int)((a + y) % mod)));
            }
            y = 0;
        } else {
            scanf(" %d", &b);
            y = sum(root, a, b);
            printf("%lld\n", y);
        }
    }

    return 0;
}
