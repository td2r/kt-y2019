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
    Node *left, *right;
    int value;

    Node() {}
    Node(int value) :
        left(nullptr),
        right(nullptr),
        value(value) {}
    Node(Node* left, Node* right, int value) :
        left(left),
        right(right),
        value(value) {}
};

Node* get_min(Node* root) {
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

Node* insert(Node* root, int x) {
    if (!root) {
        return new Node(x);
    }
    if (x < root->value) {
        root->left = insert(root->left, x);
    } else if (x > root->value) {
        root->right = insert(root->right, x);
    }
    return root;
}

Node* delete_key(Node* root, int x) {
    if (!root) {
        return root;
    }
    if (x < root->value) {
        root->left = delete_key(root->left, x);
    } else if (x > root->value) {
        root->right = delete_key(root->right, x);
    } else {
        if (root->left && root->right) {
            root->value = get_min(root->right)->value;
            root->right = delete_key(root->right, root->value);
        } else {
            Node* trash = root;;
            if (root->left) {
                root = root->left;
            } else {
                root = root->right;
            }
            delete trash;
        }
    }
    return root;
}

Node* find(Node* root, int x) {
    if (!root) return root;
    if (x < root->value) {
        return find(root->left, x);
    } else if (x > root->value) {
        return find(root->right, x);
    } else {
        return root;
    }
}

Node* find_next(Node* root, int x) {
    if (!root) return root;
    if (x >= root->value) {
        return find_next(root->right, x);
    } else {
        Node* v = find_next(root->left, x);
        if (v) return v;
        else return root;
    }
}

Node* find_prev(Node* root, int x) {
    if (!root) return root;
    if (x <= root->value) {
        return find_prev(root->left, x);
    } else {
        Node* v = find_prev(root->right, x);
        if (v) return v;
        else return root;
    }
}

int main() {
    Node* root = nullptr;
    string s;
    int x;
    while (cin >> s >> x) {
        if (s == "insert") {
            root = insert(root, x);
        } else if (s == "delete") {
            root = delete_key(root, x);
        } else if (s == "exists") {
            if (find(root, x)) cout << "true" << endl;
            else cout << "false" << endl;
        } else if (s == "next") {
            Node* v = find_next(root, x);
            if (v) cout << v->value << endl;
            else cout << "none" << endl;
        } else {
            Node* v = find_prev(root, x);
            if (v) cout << v->value << endl;
            else cout << "none" << endl;
        }
    }
    return 0;
}
