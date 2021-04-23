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
    Node *left, *right, *parent;
    int a, b, id;


    Node(int a, int b, int id) :
        left(nullptr),
        right(nullptr),
        parent(nullptr),
        a(a), b(b), id(id) {}

    Node(Node* left, Node* right, Node* parent, int a, int b, int id) :
        left(left),
        right(right),
        parent(parent),
        a(a), b(b), id(id) {}
};




const int MAXN = 3e5 + 1, INF = 2e9;

Node* vertex[MAXN];

int la[MAXN], ra[MAXN], pa[MAXN];

int main() {
    int n;

    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        vertex[i] = new Node(a, b, i);
    }

    sort(vertex, vertex + n, [](Node* v1, Node*v2) {
        return v1->a < v2->a;
    });

    Node* v = new Node(-INF, -INF, -1);
    for (int i = 0; i < n; ++i) {
        while (v->b > vertex[i]->b) {
            v = v->parent;
        }
        if (v->right) {
            vertex[i]->left = v->right;
            v->right->parent = vertex[i];
        }
        v->right = vertex[i];
        vertex[i]->parent = v;
        v = v->right;
    }
    while (v->parent) v = v->parent;
    v->right->parent = nullptr;
    delete v;

    for (int i = 0; i < n; ++i) {
        if (vertex[i]->left) {
            la[vertex[i]->id] = vertex[i]->left->id + 1;
        }
        if (vertex[i]->right) {
            ra[vertex[i]->id] = vertex[i]->right->id + 1;
        }
        if (vertex[i]->parent) {
            pa[vertex[i]->id] = vertex[i]->parent->id + 1;
        }
    }

    printf("YES\n");
    for (int i = 0; i < n; ++i) {
        printf("%d %d %d\n", pa[i], la[i], ra[i]);
    }
    return 0;
}
