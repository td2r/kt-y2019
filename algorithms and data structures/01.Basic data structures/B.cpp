#include <cstdlib>
#include <stdio.h>
#include <cstring>


struct Node_t {
    Node_t *prev;
    int a;

    Node_t() : prev(NULL), a() {}
    Node_t(int a) : prev(NULL), a(a) {}
    Node_t(int a, Node_t *prev) : prev(prev), a(a) {}
};


int main() {
    char ch;
    int m, a;
    Node_t *top = NULL;
    scanf("%d", &m);
    while (m--) {
        scanf("\n%c", &ch);
        if (ch == '+') {
            scanf(" %d", &a);
            top = new Node_t(a, top);
        } else {
            printf("%d\n", top->a);
            Node_t *newtop = top->prev;
            free(top);
            top = newtop;
        }
    }
    return 0;
}
