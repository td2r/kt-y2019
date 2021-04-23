#include <cstdlib>
#include <stdio.h>
#include <cstring>


struct Stack {
    int sz, capacity, *p;

    Stack() : sz(0), capacity(1) {
        p = (int*)malloc(sizeof(int));
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
        return (int)*(p + sz - 1);
    }
    void pop() {
        --sz;
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
    }
    int get_n_pop() {
        --sz;
        int x = (int)*(p + sz);
        if (sz && 2 * sz < capacity) {
            narrow_down();
        }
        return x;
    }
};


int main() {
    char ch;
    int m, a;
    Stack st;
    scanf("%d", &m);
    while (m--) {
        scanf("\n%c", &ch);
        if (ch == '+') {
            scanf(" %d", &a);
            st.push(a);
        } else {
            printf("%d\n", st.top());
            st.pop();
        }
    }
    return 0;
}
