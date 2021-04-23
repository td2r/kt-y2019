#include <cstdlib>
#include <stdio.h>
#include <cstring>
typedef unsigned int uint;


const int ALPHA = 256;

struct Node_t {
    int term;
    Node_t *to[ALPHA];

    Node_t() : term(-2) {
        memset(to, 0, sizeof(Node_t*) * ALPHA);
    }
};


template<typename T>
struct Vector {
    int sz, capacity;
    T *arr;

    Vector() : sz(0), capacity(1) {
        arr = (T*)malloc(sizeof(T));
    }

    bool empty() const {
        return sz == 0;
    }
    int size() const {
        return sz;
    }
    void expand() {
        capacity *= 2;
        T *buff = (T*)malloc(sizeof(T) * capacity);
        memcpy(buff, arr, sizeof(T) * sz);
        free(arr);
        arr = buff;
    }
    void narrow_down() {
        capacity /= 2;
        T *buff = (T*)malloc(sizeof(T) * capacity);
        memcpy(buff, arr, sizeof(T) * sz);
        free(arr);
        arr = buff;
    }
    void push_back(T x) {
        *(arr + sz) = x;
        ++sz;
        if (sz == capacity) {
            expand();
        }
    }
    void pop_back() {
        --sz;
        if (sz && sz * 2 < capacity) {
            narrow_down();
        }
    }
    T back() {
        return *(arr + sz - 1);
    }
    T back_n_pop() {
        --sz;
        T x = *(arr + sz);
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
    T& operator[](int ind) {
        return *(arr + ind);
    }
};


template<typename T>
struct Queue {
    Vector<T> st1, st2;

    Queue() : st1(), st2() {}

    void push(T x) {
        st1.push_back(x);
    }
    T front() {
        if (st2.empty()) {
            while (!st1.empty()) {
                st2.push_back(st1.back_n_pop());
            }
        }
        return st2.back();
    }
    void pop() {
        if (st2.empty()) {
            while (!st1.empty()) {
                st2.push_back(st1.back_n_pop());
            }
        }
        st2.pop_back();
    }
    T front_n_pop() {
        if (st2.empty()) {
            while (!st1.empty()) {
                st2.push_back(st1.back_n_pop());
            }
        }
        return st2.back_n_pop();
    }
};


const int mod = 65536;


void add_string(Node_t *root, Vector<char> &s, int val) {
    int len = s.size();
    for (int i = 1; i < len; ++i) {
        if (!root->to[(int)s[i]]) {
            root->to[(int)s[i]] = new Node_t();
        }
        root = root->to[(int)s[i]];
    }
    root->term = val;
}

int find_string(Node_t *root, Vector<char> &s, int i) {
    int len = s.size();
    for (; i < len; ++i) {
        root = root->to[(int)s[i]];
    }
    return root->term;
}


int main() {
    // freopen("inp.txt", "r", stdin);
    char c;
    Node_t *root = new Node_t();
    int reg[26];
    Queue<int> q;
    Vector<Vector<char>> program;
    while (scanf("%c", &c) != -1) {
        Vector<char> line;
        line.push_back(c);
        while (scanf("%c", &c) != -1 && c != '\n') {
            line.push_back(c);
        }
        if (!line.empty()) {
            if (line[0] == ':') {
                add_string(root, line, program.size());
            }
            program.push_back(line);
        }
    }
    for (int i = 0; i < program.size(); ++i) {
        int x, y;
        switch (program[i][0]) {
        case '+':
            x = q.front_n_pop();
            y = q.front_n_pop();
            q.push((x + y) % mod);
            break;
        case '-':
            x = q.front_n_pop();
            y = q.front_n_pop();
            q.push((x - y + mod) % mod);
            break;
        case '*':
            x = q.front_n_pop();
            y = q.front_n_pop();
            q.push((int)(((uint)x * y) % mod));
            break;
        case '/':
            x = q.front_n_pop();
            y = q.front_n_pop();
            if (y == 0) y = 1;
            q.push(x / y);
            break;
        case '%':
            x = q.front_n_pop();
            y = q.front_n_pop();
            if (y == 0) y = 1;
            q.push(x % y);
            break;
        case '>':
            reg[program[i][1] - 'a'] = q.front_n_pop();
            break;
        case '<':
            q.push(reg[program[i][1] - 'a']);
            break;
        case 'P':
            if (program[i].size() == 1) {
                printf("%d\n", q.front_n_pop());
            } else {
                printf("%d\n", reg[program[i][1] - 'a']);
            }
            break;
        case 'C':
            if (program[i].size() == 1) {
                printf("%c", char(q.front_n_pop() % 256));
            } else {
                printf("%c", char(reg[program[i][1] - 'a'] % 256));
            }
            break;
        case ':':
            break;
        case 'J':
            i = find_string(root, program[i], 1);
            break;
        case 'Z':
            if (reg[program[i][1] - 'a'] == 0) {
                i = find_string(root, program[i], 2);
            }
            break;
        case 'E':
            if (reg[program[i][1] - 'a'] == reg[program[i][2] - 'a']) {
                i = find_string(root, program[i], 3);
            }
            break;
        case 'G':
            if (reg[program[i][1] - 'a'] > reg[program[i][2] - 'a']) {
                i = find_string(root, program[i], 3);
            }
            break;
        case 'Q':
            i = program.size();
            break;
        default:
            x = 0;
            for (int j = 0; j < program[i].size(); ++j) {
                x = x * 10 + program[i][j] - '0';
            }
            q.push(x % mod);
            break;
        }
    }
    // fclose(stdin);
    return 0;
}
