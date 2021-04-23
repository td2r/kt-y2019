#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;



const int HTS = 1022869;
const int mod = HTS;
char none_cstring[] = "none";

int string_hash(char* s) {
    int h = 0;
    for (; *s != '\0'; ++s) {
        h = 31 * h + *s;
        h %= mod;
    }
    return h;
}

vector<pair<char*, char*>> hash_table[HTS];

void dbg() {
    printf("\n\n");
    for (int i = 0; i < HTS; ++i) {
        if (hash_table[i].empty()) continue;
        printf("hash_table[%d]:\n", i);
        for (auto &it : hash_table[i]) {
            printf("\t%s %s\n", it.first, it.second);
        }
    }
    printf("\n\n");
}

void put(char* key, char* value) {
    int h = string_hash(key);
    for (auto &it : hash_table[h]) {
        if (strcmp(it.first, key) == 0) {
            delete [] it.second;
            it.second = new char[21];
            strcpy(it.second, value);
            return;
        }
    }
    char* key_cpy = new char[21];
    strcpy(key_cpy, key);
    char* value_cpy = new char[21];
    strcpy(value_cpy, value);
    hash_table[h].emplace_back(mp(key_cpy, value_cpy));
}

void del(char* key) {
    int h = string_hash(key);
    for (auto it = hash_table[h].begin(); it != hash_table[h].end(); ++it) {
        if (strcmp(it->first, key) == 0) {
            delete [] it->first;
            delete [] it->second;
            hash_table[h].erase(it);
            return;
        }
    }
}

char* get(char* key) {
    int h = string_hash(key);
    for (auto &it : hash_table[h]) {
        if (strcmp(it.first, key) == 0) {
            return it.second;
        }
    }
    return none_cstring;
}

int main() {
    freopen("map.in", "r", stdin); freopen("map.out", "w", stdout);
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    char key[21];
    char value[21];
    char command[11];

    while (scanf("%s %s", command, key) != -1) {
        switch (command[0]) {
            case 'p':
                scanf(" %s\n", value);
                put(key, value);
                break;
            case 'd':
                scanf("\n");
                del(key);
                break;
            case 'g':
                scanf("\n");
                printf("%s\n", get(key));
                break;
        }
    }

    fclose(stdin); fclose(stdout);
    return 0;
}
