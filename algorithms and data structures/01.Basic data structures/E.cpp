#include <cstdlib>
#include <stdio.h>
#include <cstring>


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


bool check_seq(Vector<char> &s) {
    Vector<char> st;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        if (s[i] == '(' || s[i] == '[') {
            st.push_back(s[i]);
        } else {
            if (st.empty()) return false;
            if ((st.back() == '(' && s[i] == ')') || (st.back() == '[' && s[i] == ']')) {
                st.pop_back();
            } else {
                return false;
            }
        }
    }
    return st.empty();
}


int main() {
    char c;
    while (scanf("%c", &c) != -1) {
        Vector<char> s;
        s.push_back(c);
        while (scanf("%c", &c) != -1 && c != '\n') {
            s.push_back(c);
        }
        if (check_seq(s)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
    return 0;
}
