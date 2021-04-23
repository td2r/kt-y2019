#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;



const int HTS = 1e6;

vector<int> hash_table[HTS];

void insert(const int x) {
    int y = abs(x) % HTS;
    if (find(hash_table[y].begin(), hash_table[y].end(), x) == hash_table[y].end()) {
        hash_table[y].emplace_back(x);
    }
}

void remove(const int x) {
    int y = abs(x) % HTS;
    auto it = find(hash_table[y].begin(), hash_table[y].end(), x);
    if (it != hash_table[y].end()) {
        hash_table[y].erase(it);
    }
}

bool exists(const int x) {
    int y = abs(x) % HTS;
    return find(hash_table[y].begin(), hash_table[y].end(), x) != hash_table[y].end();
}

int main() {
    freopen("set.in", "r", stdin); freopen("set.out", "w", stdout);
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    int x;
    char command[10];
    while (scanf("%s %d\n", command, &x) != -1) {
        switch (command[0]) {
            case 'i':
                insert(x);
                break;
            case 'd':
                remove(x);
                break;
            case 'e':
                printf("%s\n", exists(x) ? "true" : "false");
                break;
        }
    }

    fclose(stdin); fclose(stdout);
    return 0;
}
