#include <bits/stdc++.h>
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



struct Node {
    pair<int, int> key;
    Node *parent;
    Node *child;
    Node *sibling;
    Node *rsibling;
    int degree;

    Node(pair<int, int> key) :
        key(key),
        parent(nullptr),
        child(nullptr),
        sibling(nullptr),
        rsibling(nullptr),
        degree(0) {}

    Node* add_sibling(Node *v) {
        sibling = v;
        if (v) {
            v->rsibling = this;
        }
        return v;
    }

    Node* add_rsibling(Node *v) {
        rsibling = v;
        if (v) {
            v->sibling = this;
        }
        return v;
    }

    Node* add_child(Node *v) {
        assert(v);
        v->add_sibling(child);
        v->rsibling = nullptr;
        v->parent = this;
        child = v;
        ++degree;
        return v;
    }

    void child_free() {
        child = nullptr;
        degree = 0;
    }
};

ostream& operator << (ostream &os, Node *v) {
    if (!v) {
        os << "null";
        return os;
    }
    os << "{";
    os << "key = " << v->key;
    os << ", parent = ";
    if (v->parent) os << v->parent->key;
    else        os << "null";
    os << ", child = ";
    if (v->child) os << v->child->key;
    else       os << "null";
    os << ", sibling = ";
    if (v->sibling) os << v->sibling->key;
    else         os << "null";
    os << ", rsibling = ";
    if (v->rsibling) os << v->rsibling->key;
    else          os << "null";
    os << ", degree = " << v->degree;
    os << "}";
    return os;
}

void show_heap(Node *heap) {
    assert(heap);
    cout << "heap " << -(heap->key.second) << endl;
    queue<Node*> q;
    for (Node *v = heap->child; v; v = v->sibling) {
        q.push(v);
    }
    while (!q.empty()) {
        Node *node = q.front();
        q.pop();
        cout << "  " << node << endl;
        for (Node *v = node->child; v; v = v->sibling) {
            q.push(v);
        }
    }
    cout << endl;
}


const int MAXN = 1e3 + 1;
const int MAXM = 1e6 + 1;

Node *node_by_index[MAXM];
Node *heap_by_index[MAXN];


Node* make_heap(int i) {
    return new Node(mp(INT_MIN, -i));
}

int find_heap(Node *v) {
    while (v->parent) {
        v = v->parent;
    }
    return -(v->key.second);
}

int get_min(Node *heap) {
    int ret = INT_MAX;
    for (Node *v = heap->child; v; v = v->sibling) {
        ret = min(ret, v->key.first);
    }
    return ret;
}

Node* merge(Node *h1, Node *h2) {
    int n = h1->degree + h2->degree;
    Node **a = new Node*[n];
    Node *v1 = h1->child;
    Node *v2 = h2->child;
    n = 0;
    while (v1 || v2) {
        Node *v;
        if (v1 && (!v2 || v1->degree < v2->degree)) {
            v = v1;
            v1 = v1->sibling;
        } else {
            v = v2;
            v2 = v2->sibling;
        }
        if (n > 0 && a[n - 1]->degree == v->degree) {
            if (a[n - 1]->key < v->key) {
                a[n - 1]->add_child(v);
            } else {
                v->add_child(a[n - 1]);
                a[n - 1] = v;
            }
        } else {
            a[n++] = v;
        }
    }
    h1->child_free();
    h2->child_free();
    while (n) {
        h1->add_child(a[--n]);
    }
    delete a;
    return h1;
}

void insert(Node *heap, Node *v) {
    heap_by_index[0]->add_child(v);
    merge(heap, heap_by_index[0]);
}

void delete_min(Node *heap) {
    Node *h = heap->child;
    for (Node *v = heap->child; v; v = v->sibling) {
        if (v->key < h->key) {
            h = v;
        }
    }
    if (h->rsibling) {
        h->rsibling->add_sibling(h->sibling);
    } else {
        heap->child = h->sibling;
        if (h->sibling) {
            h->sibling->add_rsibling(h->rsibling);
        }
    }
    --(heap->degree);
    for (Node *v = h->child; v; v = v->rsibling) {
        swap(v->sibling, v->rsibling);
        h->child = v;
    }
    merge(heap, h);
    node_by_index[h->key.second] = nullptr;
    delete h;
}

Node* siftUp(Node *v) {
    Node *p = v->parent;
    while (v->key < p->key) {
        swap(node_by_index[v->key.second], node_by_index[p->key.second]);
        swap(v->key, p->key);
        v = p;
        p = v->parent;
    }
    return v;
}

Node* siftDown(Node *v) {
    Node *tiny = v->child;
    for (Node *kid = v->child; kid; kid = kid->sibling) {
        if (kid->key < tiny->key) {
            tiny = kid;
        }
    }
    while (tiny && v->key > tiny->key) {
        swap(node_by_index[v->key.second], node_by_index[tiny->key.second]);
        swap(v->key, tiny->key);
        v = tiny;
        tiny = v->child;
        for (Node *kid = v->child; kid; kid = kid->sibling) {
            if (kid->key < tiny->key) {
                tiny = kid;
            }
        }
    }
    return v;
}

void change_key(Node *v, int newval) {
    if (newval < v->key.first) {
        v->key.first = newval;
        siftUp(v);
    } else {
        v->key.first = newval;
        siftDown(v);
    }
}

void delete_node(Node *v) {
    Node *h = v->parent;
    while (h->key.second > 0) {
        swap(node_by_index[v->key.second], node_by_index[h->key.second]);
        swap(v->key, h->key);
        v = h;
        h = h->parent;
    }
    if (v->rsibling) {
        v->rsibling->add_sibling(v->sibling);
    } else {
        h->child = v->sibling;
        if (v->sibling) {
            v->sibling->add_rsibling(v->rsibling);
        }
    }
    --(h->degree);
    for (Node *kid = v->child; kid; kid = kid->rsibling) {
        swap(kid->sibling, kid->rsibling);
        v->child = kid;
    }
    merge(h, v);
    node_by_index[v->key.second] = nullptr;
    delete v;
}

int main() {
    int n, m;

    scanf("%d %d", &n, &m);
    for (int i = 0; i <= n; ++i) {
        heap_by_index[i] = make_heap(i);
    }

    int q = 1;
    int param[3];
    while (m--) {
        scanf("%d %d", &param[0], &param[1]);
        if (param[0] == 0 || param[0] == 1 || param[0] == 3)
            scanf("%d", &param[2]);
        if (param[0] == 0) {
            node_by_index[q] = new Node(mp(param[2], q));
            insert(heap_by_index[param[1]], node_by_index[q]);
            ++q;
        } else if (param[0] == 1) {
            merge(heap_by_index[param[2]], heap_by_index[param[1]]);
        } else if (param[0] == 2) {
            delete_node(node_by_index[param[1]]);
        } else if (param[0] == 3) {
            change_key(node_by_index[param[1]], param[2]);
        } else if (param[0] == 4) {
            printf("%d\n", get_min(heap_by_index[param[1]]));
        } else {
            delete_min(heap_by_index[param[1]]);
        }
    }
    return 0;
}
