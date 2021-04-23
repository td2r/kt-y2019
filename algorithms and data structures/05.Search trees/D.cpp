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
    int x, y, sz;


    Node(int x) :
        left(nullptr), right(nullptr),
        x(x), y(rand() | (rand() << 16)), sz(1) {}
};


int getsize(Node* root) {
    return root ? root->sz : 0;
}


void recalc(Node* root) {
    if (!root) return;
    root->sz = getsize(root->left) + 1 + getsize(root->right);
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

Node* find_kth(Node* root, int k) {
    if (!root) return root;
    if (k <= getsize(root->left)) {
        return find_kth(root->left, k);
    } else if (k == getsize(root->left) + 1) {
        return root;
    } else {
        return find_kth(root->right, k - getsize(root->left) - 1);
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
        ++root->sz;
        return root;
    } else {
        auto spl = split(root, node->x);
        node->left = spl.first;
        node->right = spl.second;
        recalc(node);
        return node;
    }
}

Node* remove(Node* root, int k) {
    if (!root) return root;
    if (k < root->x) {
        root->left = remove(root->left, k);
        --root->sz;
        return root;
    } else if (root->x < k) {
        root->right = remove(root->right, k);
        --root->sz;
        return root;
    } else {
        Node* new_root = merge(root->left, root->right);
        delete root;
        return new_root;
    }
}


int main() {
    int q, n = 0;
    Node* root = nullptr;

    scanf("%d", &q);

    while (q--) {
        int t, k;
        scanf("%d %d", &t, &k);
        if (t == 1) {
            root = insert(root, new Node(k));
            ++n;
        } else if (t == 0) {
            printf("%d\n", find_kth(root, n - k + 1)->x);
        } else {
            root = remove(root, k);
            --n;
        }
    }
    return 0;
}
