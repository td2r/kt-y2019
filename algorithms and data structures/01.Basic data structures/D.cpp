#include <cstdlib>
#include <stdio.h>
#include <cstring>


struct Node_t {
    Node_t *prev;
    int a;

    Node_t() : prev(NULL), a() {}
    Node_t(int a) : prev(NULL), a(a) {}
    Node_t(Node_t *prev, int a) : prev(prev), a(a) {}
};


int main() {
    char ch;
    int m, a;
    Node_t *fnode = NULL, *bnode = NULL;
    scanf("%d", &m);
    while (m--) {
        scanf("\n%c", &ch);
        if (ch == '+') {
            scanf(" %d", &a);
            Node_t *new_bnode = new Node_t(a);
            if (bnode) {
                bnode->prev = new_bnode;
            } else {
                fnode = new_bnode;
            }
            bnode = new_bnode;
        } else {
            printf("%d\n", fnode->a);
            Node_t *new_fnode = fnode->prev;
            if (!new_fnode) {
                bnode = NULL;
            }
            free(fnode);
            fnode = new_fnode;
        }
    }
    return 0;
}
