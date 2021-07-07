#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
    file_raii(string const & filename) {
        freopen((filename + ".in").c_str(), "r", stdin);
        // freopen((filename + ".out").c_str(), "w", stdout);
    }

    file_raii(int const problem_number) {
        freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
        freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
    }

    ~file_raii() noexcept {
        // fclose(stdout);
        fclose(stdin);
    }
};

template <class forwardIterator>
void print_array(forwardIterator first,
                 forwardIterator last,
                 string const& name = "",
                 string const& separator = ", ") {
    cout << name << " : ";
    if (first == last) {
        cout << "[]" << endl;
        return;
    }
    cout << *first++;
    while (first != last) {
        cout << separator << *first++;
    }
    cout << endl;
}

int read_uint() {
    char c;
    int res = 0;
    while ((c = (char) getchar()) != -1) {
        if (c == ' ' || c == '\n') {
            return res;
        }
        res = 10 * res + c - '0';
    }
    return res;
}

int const MAXN = 1'000'000 + 3;
int const ALPHA = 'z' - 'a' + 1;

int timer = 0;
struct node_t {
    vector<int> term;
    bool used;
    char parent_char;
    node_t* parent_node;
    node_t* link[ALPHA];
    node_t* suffix_link;
    int index;

    node_t()
    : term()
    , used(false)
    , parent_char(0)
    , parent_node(NULL)
    , suffix_link(NULL)
    , index(timer++)
    {
        memset(link, 0, sizeof(node_t*) * ALPHA);
    }

    void print() {
        printf("node {\n");
        printf("\tindex = %d\n", index);
        printf("\tlinks {\n");
        for (int i = 0; i < ALPHA; ++i) {
            if (link[i] != NULL) {
                printf("\t\t\"%c\": %d\n", i + 'a', link[i]->index);
            }
        }
        printf("\t}\n");
        printf("}\n");
    }
};

int n;
char s[MAXN];
vector<node_t*> bfs;
node_t* root = new node_t();

void init_trie() {
    node_t* pr_root = new node_t();
    fill(pr_root->link, pr_root->link + ALPHA, root);
    root->suffix_link = pr_root;
}

void add_word(int i) {
    node_t* node = root;
    char* ch = s;
    while (*ch) {
        int x = *ch - 'a';
        if (node->link[x] == NULL) {
            node_t* next = new node_t();
            node->link[x] = next;
            next->parent_char = (char) x;
            next->parent_node = node;
        }
        node = node->link[x];
        ch++;
    }
    node->term.emplace_back(i);
}

void run_bfs() {
    size_t i = 0;
    bfs.emplace_back(root);
    while (i < bfs.size()) {
        for (int j = 0; j < ALPHA; ++j) {
            if (bfs[i]->link[j]) {
                bfs.emplace_back(bfs[i]->link[j]);
            }
        }
        ++i;
    }
    bfs.erase(bfs.begin());
}

int main() {
    init_trie();

    scanf("%d\n", &n);
    for (int i = 0; i < n; ++i) {
        scanf("%s\n", s);
        add_word(i);
    }

    run_bfs();

    for (node_t* node : bfs) {
        int ch = node->parent_char;
        node_t* suffix_node = node->parent_node->suffix_link;
        while (suffix_node->link[ch] == NULL) {
            suffix_node = suffix_node->suffix_link;
        }
        node->suffix_link = suffix_node->link[ch];
    }

    scanf("%s", s);
    node_t* node = root;
    for (char* ch = s; *ch; ++ch) {
        int x = *ch - 'a';
        while (node->link[x] == NULL) {
            node = node->suffix_link;
        }
        node = node->link[x];
        node->used = true;
    }

    vector<bool> used(n, false);
    for (int i = (int) bfs.size() - 1; i >= 0; --i) {
        if (bfs[i]->used) {
            bfs[i]->parent_node->used = true;
            bfs[i]->suffix_link->used = true;
            for (int const& j : bfs[i]->term) {
                used[j] = true;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%s\n", used[i] ? "YES" : "NO");
    }

    return 0;
}
