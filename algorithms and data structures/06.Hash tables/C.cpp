#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;




struct Node {
    Node* prev;
    Node* next;
    char* key;
    char* value;

    Node(Node* prev, Node* next, char* key, char* value) : prev(prev), next(next) {
        this->key = new char[21];
        strcpy(this->key, key);
        this->value = new char[21];
        strcpy(this->value, value);
    }

    void set_value(char* new_value) {
        delete [] value;
        value = new char[21];
        strcpy(value, new_value);
    }

    void pull_out() {
        if (prev) { prev->next = next; }
        if (next) { next->prev = prev; }
        prev = next = nullptr;
    }

    ~Node() {
        delete [] key;
        delete [] value;
    }
};

void print(const Node* node) {
    printf("%s\n", node ? node->value : "none");
}

const int HTS = 1022869;
const int mod = HTS;

int string_hash(char* s) {
    int h = 0;
    for (; *s != '\0'; ++s) {
        h = 31 * h + *s;
        h %= mod;
    }
    return h;
}

Node* tail = nullptr;
vector<Node*> hash_table[HTS];

void dbg() {
    printf("\n\n");
    for (int i = 0; i < HTS; ++i) {
        if (hash_table[i].empty()) continue;
        printf("hash_table[%d]:\n", i);
        for (auto &it : hash_table[i]) {
            printf("\tkey = %s, value = %s, prev = %s, next = %s\n",
                    it->key, it->value, (it->prev ? it->prev->value : "none"),
                    (it->next ? it->next->value : "none"));
        }
    }
    printf("\n\n");
}

inline auto find(const vector<Node*>& bucket, char* key) {
    return find_if(bucket.begin(), bucket.end(), [&](Node* const &x) {
        return strcmp(x->key, key) == 0;
    });
}

void del(char* key) {
    vector<Node*>& bucket = hash_table[string_hash(key)];
    auto it = find(bucket, key);
    if (it != bucket.end()) {
        Node* tmp = *it;
        bucket.erase(it);
        if (tmp == tail) { tail = tail->prev; }
        tmp->pull_out();
        delete tmp;
    }
}

void put(char* key, char* value) {
    vector<Node*>& bucket = hash_table[string_hash(key)];
    auto it = find(bucket, key);
    if (it == bucket.end()) {
        Node* new_tail = new Node(tail, nullptr, key, value);
        bucket.emplace_back(new_tail);
        if (tail) { tail->next = new_tail; }
        tail = new_tail;
    } else {
        (*it)->set_value(value);
    }
}

Node* get(char* key) {
    const vector<Node*>& bucket = hash_table[string_hash(key)];
    auto it = find(bucket, key);
    return it == bucket.end() ? nullptr : *it;
}

Node* prev(char* key) {
    Node* node = get(key);
    return node ? node->prev : nullptr;
}

Node* next(char* key) {
    Node* node = get(key);
    return node ? node->next : nullptr;
}

int main() {
    freopen("linkedmap.in", "r", stdin); freopen("linkedmap.out", "w", stdout);
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    char key[21];
    char value[21];
    char command[11];

    while (scanf("%s ", command) != -1) {
        if (command[0] == 'p' && command[1] == 'u') {
            scanf("%s %s\n", key, value);
            put(key, value);
        } else {
            scanf("%s\n", key);
            switch (command[0]) {
                case 'd':
                    del(key);
                    break;
                case 'g':
                    print(get(key));
                    break;
                case 'p':
                    print(prev(key));
                    break;
                case 'n':
                    print(next(key));
                    break;
            }
        }
    }

    fclose(stdin); fclose(stdout);
    return 0;
}
