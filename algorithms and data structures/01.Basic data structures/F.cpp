#include <cstdlib>
#include <stdio.h>
#include <cstring>


template<typename T>
struct Vector {
    int sz, capacity;
    T *p;

    Vector() : sz(0), capacity(1) {
        p = (T*)malloc(sizeof(T));
    }

    bool empty() const {
        return sz == 0;
    }
    void expand() {
        capacity *= 2;
        T *buff = (T*)malloc(sizeof(T) * capacity);
        memcpy(buff, p, sizeof(T) * sz);
        free(p);
        p = buff;
    }
    void narrow_down() {
        capacity /= 2;
        T *buff = (T*)malloc(sizeof(T) * capacity);
        memcpy(buff, p, sizeof(T) * sz);
        free(p);
        p = buff;
    }
    void push_back(T x) {
        *(p + sz) = x;
        ++sz;
        if (sz == capacity) {
            expand();
        }
    }
    T back() const {
        return *(p + sz - 1);
    }
    void pop_back() {
        --sz;
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
    }
    T back_n_pop() {
        --sz;
        T x = *(p + sz);
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
    T& operator[](int ind) {
        return *(p + ind);
    }
    int size() const {
        return sz;
    }
};


char s[20];


int main() {
    Vector<int> v;
    while (scanf("%s", s) != -1) {
        if (s[0] == '+') {
            v.push_back(v.back_n_pop() + v.back_n_pop());
        } else if (s[0] == '-') {
            int b = v.back_n_pop();
            int a = v.back_n_pop();
            v.push_back(a - b);
        } else if (s[0] == '*') {
            v.push_back(v.back_n_pop() * v.back_n_pop());
        } else {
            v.push_back(std::atoi(s));
        }
    }
    printf("%d\n", v.back());
    if (v.size() != 1) {
        // printf("WARNING v.size() = %d\n", v.size());
        return 1;
    }
    return 0;
}
