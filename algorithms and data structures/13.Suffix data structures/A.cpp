#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 100 + 1;
int const ALPHA = 256;

template <class forwardIterator>
void print_array(string const& name, forwardIterator first, forwardIterator last) {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << ", " << *first++;
    }
    cout << endl;
}

struct node_t {
    int to[ALPHA];

    node_t() {
        memset(to, 255, sizeof(int) * ALPHA);
    }
};

int n;
int m = 1;
int edges = 0;
char s[MAXN];
node_t node[MAXN * MAXN];

void trie_add(char *c, int v = 0) {
    if (*c == 0)
        return;
    if (node[v].to[(int) *c] == -1) {
        node[v].to[(int) *c] = m++;
        ++edges;
    }
    return trie_add(c + 1, node[v].to[(int) *c]);
}

int main() {
    scanf("%s", s);
    n = (int) strlen(s);

    for (int i = 0; i < n; ++i) {
        trie_add(s + i);
    }

    printf("%d %d\n", m, edges);
    for (int i = 0; i < m; ++i) {
        for (char j = 'a'; j <= 'z'; ++j) {
            if (node[i].to[(int) j] != -1) {
                printf("%d %d %c\n", i + 1, node[i].to[(int) j] + 1, j);
            }
        }
    }

    return 0;
}
