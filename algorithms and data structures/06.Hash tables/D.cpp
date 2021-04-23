#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;




const int MTL = 21;
const int HSS = 53;
const int HMS = 122869;


int str_hash(const char *s, const int mod) {
    int h = 0;
    for (; *s != '\0'; ++s) h = (31 * h + *s) % mod;
    return h;
}



struct hash_set {
    size_t sz;
    vector<char*> buckets[HSS];

    hash_set() : sz(0) {}

    bool empty() {
        return sz == 0;
    }

    auto find_value(const char* s, vector<char*>& bucket) {
        return find_if(bucket.begin(), bucket.end(),
            [&](char* const& a) {
                return strcmp(s, a) == 0;
            });
    }

    void put(const char* value) {
        auto& bucket = buckets[str_hash(value, HSS)];
        auto it = find_value(value, bucket);
        if (it == bucket.end()) {
            bucket.emplace_back(strcpy(new char[strlen(value)], value));
            ++sz;
        }
    }

    void del(const char* value) {
        auto& bucket = buckets[str_hash(value, HSS)];
        auto it = find_value(value, bucket);
        if (it != bucket.end()) {
            delete [] *it;
            bucket.erase(it);
            --sz;
        }
    }

    void print() {
        printf("%zd", sz);
        for (auto& bucket : buckets) {
            for (auto& s : bucket) {
                printf(" %s", s);
            }
        }
        printf("\n");
    }

    ~hash_set() {
        for (auto& bucket : buckets) {
            for (auto& s : bucket) {
                delete [] s;
            }
        }
        sz = 0;
    }
};


struct hash_map {
    vector<pair<char*, hash_set*>> buckets[HMS];

    auto find_key(const char* s, vector<pair<char*, hash_set*>>& bucket) {
        return find_if(bucket.begin(), bucket.end(),
            [&](const pair<char*, hash_set*>& a) {
                return strcmp(s, a.first) == 0;
            });
    }

    void put(const char* key, const char* value) {
        auto& bucket = buckets[str_hash(key, HMS)];
        size_t i = find_key(key, bucket) - bucket.begin();
        if (i == bucket.size()) {
            bucket.emplace_back(mp(strcpy(new char[strlen(key)], key), new hash_set()));
        }
        bucket[i].second->put(value);
    }

    void del(const char* key, const char* value) {
        auto& bucket = buckets[str_hash(key, HMS)];
        auto it = find_key(key, bucket);
        if (it != bucket.end()) {
            it->second->del(value);
            if (it->second->empty()) {
                delete [] it->first;
                delete it->second;
                bucket.erase(it);
            }
        }
    }

    void del_all(const char* key) {
        auto& bucket = buckets[str_hash(key, HMS)];
        auto it = find_key(key, bucket);
        if (it != bucket.end()) {
            delete [] it->first;
            delete it->second;
            bucket.erase(it);
        }
    }

    hash_set* get(const char* key) {
        auto& bucket = buckets[str_hash(key, HMS)];
        auto it = find_key(key, bucket);
        return it == bucket.end() ? nullptr : it->second;
    }
};


hash_map t;

int main() {
    freopen("multimap.in", "r", stdin); freopen("multimap.out", "w", stdout);
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    char key[MTL];
    char value[MTL];
    char command[MTL];

    while (scanf("%s %s", command, key) != -1) {
        if (strcmp(command, "put") == 0) {
            scanf(" %s\n", value);
            t.put(key, value);
        } else if (strcmp(command, "delete") == 0) {
            scanf(" %s\n", value);
            t.del(key, value);
        } else if (strcmp(command, "deleteall") == 0) {
            scanf("\n");
            t.del_all(key);
        } else {
            scanf("\n");
            hash_set* hs = t.get(key);
            if (hs) {
                hs->print();
            } else {
                printf("0\n");
            }
        }
    }

    fclose(stdin); fclose(stdout);
    return 0;
}
