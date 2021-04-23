#include <cstdlib>
#include <stdio.h>
#include <cstring>

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline void swap(int &a, int &b) {
    int c = b;
    b = a;
    a = c;
}


struct Vector {
    int sz, capacity, *arr, *marr;

    Vector() : sz(0), capacity(1) {
        arr = (int*)malloc(sizeof(int));
        marr = (int*)malloc(sizeof(int));
    }

    bool empty() const {
        return sz == 0;
    }
    int size() const {
        return sz;
    }
    void expand() {
        capacity *= 2;
        int *buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, arr, sizeof(int) * sz);
        free(arr);
        arr = buff;
        buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, marr, sizeof(int) * sz);
        free(marr);
        marr = buff;
    }
    void narrow_down() {
        capacity /= 2;
        int *buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, arr, sizeof(int) * sz);
        free(arr);
        arr = buff;
        buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, marr, sizeof(int) * sz);
        free(marr);
        marr = buff;
    }
    void push_back(int x) {
        *(arr + sz) = x;
        *(marr + sz) = sz == 0 ? x : min(*(marr + sz - 1), x);
        ++sz;
        if (sz == capacity) {
            expand();
        }
    }
    int back() {
        return *(arr + sz - 1);
    }
    int getMin() {
        return *(marr + sz - 1);
    }
    void pop_back() {
        --sz;
        if (sz && 4 * sz < capacity) {
            narrow_down();
        }
    }
    int back_n_pop() {
        --sz;
        int x = *(arr + sz);
        if (sz && 4 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
    int getMin_n_pop() {
        --sz;
        int x = *(marr + sz);
        if (sz && 4 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
};


struct Queue {
    Vector st1, st2;
    int sz;

    Queue() : st1(), st2(), sz(0) {}

    bool empty() const {
        return sz == 0;
    }
    int size() const {
        return sz;
    }
    void push(int x) {
        st1.push_back(x);
        ++sz;
    }
    int front() {
        if (st2.empty()) {
            while (!st1.empty()) {
                st2.push_back(st1.back_n_pop());
            }
        }
        return st2.back();
    }
    int getMin() {
        if (st2.empty()) {
            return st1.getMin();
        } else if (st1.empty()) {
            return st2.getMin();
        } else {
            return min(st1.getMin(), st2.getMin());
        }
    }
    void pop() {
        if (st2.empty()) {
            while (!st1.empty()) {
                st2.push_back(st1.back_n_pop());
            }
        }
        st2.pop_back();
        --sz;
    }
};


int main() {
    long long sum = 0;
    int n, m, k, a, b, c, t[2] = {0, 0};
    Queue q;
    scanf("%d %d %d\n%d %d %d\n", &n, &m, &k, &a, &b, &c);
    int *x = (int*)malloc(sizeof(int) * k);
    for (int i = 0; i < k; ++i) {
        scanf("%d", &x[i]);
    }
    t[0] = x[k - 2];
    t[1] = x[k - 1];
    for (int i = 0; i < n; ++i) {
        if (i < k) {
            q.push(x[i]);
        } else {
            t[0] = a * t[0] + b * t[1] + c;
            swap(t[0], t[1]);
            q.push(t[1]);
        }
        if (q.size() == m) {
            sum += q.getMin();
            q.pop();
        }
    }
    printf("%lld\n", sum);
    return 0;
}
