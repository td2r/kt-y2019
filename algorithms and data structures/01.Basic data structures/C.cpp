#include <cstdlib>
#include <stdio.h>
#include <cstring>


struct Stack {
    int sz, capacity, *p;

    Stack() : sz(0), capacity(1) {
        p = (int*)malloc(sizeof(int));
    }

    bool empty() const {
        return sz == 0;
    }
    void expand() {
        capacity *= 2;
        int *buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, p, sizeof(int) * sz);
        free(p);
        p = buff;
    }
    void narrow_down() {
        capacity /= 2;
        int *buff = (int*)malloc(sizeof(int) * capacity);
        memcpy(buff, p, sizeof(int) * sz);
        free(p);
        p = buff;
    }
    void push(int x) {
        *(p + sz) = x;
        ++sz;
        if (sz == capacity) {
            expand();
        }
    }
    int top() const {
        return *(p + sz - 1);
    }
    void pop() {
        --sz;
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
    }
    int top_n_pop() {
        --sz;
        int x = *(p + sz);
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
};


int main() {
    char ch;
    int m, a;
    Stack st1, st2;
    scanf("%d", &m);
    while (m--) {
        scanf("\n%c", &ch);
        if (ch == '+') {
            scanf(" %d", &a);
            st1.push(a);
        } else {
            if (st2.empty()) {
                while (!st1.empty()) {
                    st2.push(st1.top_n_pop());
                }
            }
            printf("%d\n", st2.top_n_pop());
        }
    }
    return 0;
}
