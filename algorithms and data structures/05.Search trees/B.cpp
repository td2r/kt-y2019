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
    int x, h;
    Node *left, *right, *parent;

    Node(int x) : x(x), h(1), left(nullptr), right(nullptr), parent(nullptr) {}
};

inline int geth(Node* root) {
    return root ? root->h : 0;
}

inline int getdiff(Node* root) {
    return root ? geth(root->left) - geth(root->right) : 0;
}

inline void recalc(Node* root) {
    if (root) {
        root->h = max(geth(root->left), geth(root->right)) + 1;
    }
}

Node* get_min(Node* root) {
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

Node* get_max(Node* root) {
    while (root && root->right) {
        root = root->right;
    }
    return root;
}

Node* find(Node* root, int k) {
    while (root && root->x != k) {
        root = k < root->x ? root->left : root->right;
    }
    return root;
}

Node* find_next(Node* root, int k) {
    if (!root) return root;
    if (k >= root->x) {
        return find_next(root->right, k);
    } else {
        Node* v = find_next(root->left, k);
        return v ? v : root;
    }
}

Node* find_prev(Node* root, int k) {
    if (!root) return root;
    if (k <= root->x) {
        return find_prev(root->left, k);
    } else {
        Node* v = find_prev(root->right, k);
        return v ? v : root;
    }
}



void rotateLeft(Node* a) {
    Node *p = a->parent;
    Node *b = a->right;

    a->right = b->left;
    if (b->left) b->left->parent = a;

    b->left = a;
    a->parent = b;

    b->parent = p;
    if (p) {
        if (p->left == a) p->left = b;
        else p->right = b;
    }

    recalc(a);
    recalc(b);
}

void rotateRight(Node* a) {
    Node *p = a->parent;
    Node *b = a->left;

    a->left = b->right;
    if (b->right) b->right->parent = a;

    b->right = a;
    a->parent = b;

    b->parent = p;
    if (p) {
        if (p->left == a) p->left = b;
        else p->right = b;
    }

    recalc(a);
    recalc(b);
}

Node* balance(Node* root) {
    recalc(root);
    if (!root || abs(getdiff(root)) < 2) return root;
    if (getdiff(root) == -2) {
        if (getdiff(root->right) == 1) {
            rotateRight(root->right);
            rotateLeft(root);
            return root->parent;
        } else {
            rotateLeft(root);
            return root->parent;
        }
    } else {
        if (getdiff(root->left) == -1) {
            rotateLeft(root->left);
            rotateRight(root);
            return root->parent;
        } else {
            rotateRight(root);
            return root->parent;
        }
    }
}

Node* insert(Node* root, int k) {
    // if (find(root, k)) return root;
    // if (!root) return new Node(k);
    // Node *node = k < root->x ? root->left : root->right;
    // while (node) {
    //     root = node;
    //     node = k < root->x ? root->left : root->right;
    // }
    //
    // node = new Node(k);
    // node->parent = root;
    // if (k < root->x) root->left = node;
    // else root->right = node;
    //
    // while (root->parent) {
    //     recalc(root);
    //     root = balance(root)->parent;
    // }
    // recalc(root);
    // return balance(root);




    if (!root) return new Node(k);
    if (k < root->x) {
        root->left = insert(root->left, k);
        root->left->parent = root;
    } else if (root->x < k) {
        root->right = insert(root->right, k);
        root->right->parent = root;
    }
    return balance(root);
}

Node* remove(Node* root, int k) {
    if (!root) return root;
    if (k < root->x) {
        root->left = remove(root->left, k);
    } else if (root->x < k) {
        root->right = remove(root->right, k);
    } else {
        if (root->right) {
            Node* node = get_min(root->right);
            swap(root->x, node->x);
            root->right = remove(root->right, k);
            if (root->right) root->right->parent = root;
        } else if (root->left) {
            Node* node = get_max(root->left);
            swap(root->x, node->x);
            root->left = remove(root->left, k);
            if (root->left) root->right->left = root;
        } else {
            delete root;
            return nullptr;
        }
    }
    return balance(root);
}


void print(Node* root) {
    if (!root) printf("0, 0 0 0, h = -1\n");
    printf("%d, %d %d %d, h = %d\n", root->x, root->parent ? root->parent->x : 0,
            root->left ? root->left->x : 0, root->right ? root->right->x : 0,
            geth(root));
}

void showTree(Node* root) {
    if (!root) return;
    showTree(root->left);
    print(root);
    showTree(root->right);
}

int main() {
    int x;
    char s[10];
    Node *root = nullptr;

    while (scanf("%s %d", s, &x) != -1) {
        if (s[0] == 'i') {
            root = insert(root, x);
        } else if (s[0] == 'd') {
            root = remove(root, x);
        } else if (s[0] == 'e') {
            printf("%s\n", find(root, x) ? "true" : "false");
        } else if (s[0] == 'n') {
            Node *v = find_next(root, x);
            if (v) printf("%d\n", v->x);
            else printf("none\n");
        } else {
            Node *v = find_prev(root, x);
            if (v) printf("%d\n", v->x);
            else printf("none\n");
        }
        if (debug) {
            printf("Tree:\n");
            showTree(root);
        }
    }

    return 0;
}
