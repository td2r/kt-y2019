#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

void from_input(string& s, vector<string>& p, int& d) {
    getline(cin, s);
    string tmp_;
    getline(cin, tmp_);
    stringstream ss(tmp_);
    while (ss >> tmp_) {
        p.push_back(tmp_);
    }
    cin >> d;
}

vector<char> nucleotide({'A', 'C', 'G', 'T'});

int nucl_id(char nucl) {
    switch (nucl) {
        case 'A':
            return 0;
        case 'C':
            return 1;
        case 'G':
            return 2;
        case 'T':
            return 3;
    }
}

struct node_t {
    int nxt[4];
    set<int> indices;

    explicit node_t() {
        memset(nxt, 255, sizeof(int) * 4);
    }
};

void trie_collect(vector<node_t> const& trie,
                  string const& p,
                  int d,
                  multiset<int>& collect,
                  int node = 0,
                  int pos = 0)
{
    if (pos == p.length()) {
        collect.insert(trie[node].indices.begin(), trie[node].indices.end());
        return;
    }
    for (int i = 0; i < nucleotide.size(); ++i) {
        if (trie[node].nxt[i] == -1)
            continue;
        if (nucleotide[i] == p[pos])
            trie_collect(trie, p, d, collect, trie[node].nxt[i], pos + 1);
        else if (d > 0) {
            trie_collect(trie, p, d - 1, collect, trie[node].nxt[i], pos + 1);
        }
    }
}

int main() {
    string s;
    vector<string> patterns;
    int d;
    from_input(s, patterns, d);

    int maxl = -1;
    for (auto const& p : patterns) {
        maxl = max(maxl, (int) p.length());
    }

    vector<node_t> trie(1);
    for (int i = 0; i < s.length(); ++i) {
        int node = 0;
        for (int j = 0; j < maxl && i + j < s.length(); ++j) {
            if (trie[node].nxt[nucl_id(s[i + j])] == -1) {
                trie[node].nxt[nucl_id(s[i + j])] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].nxt[nucl_id(s[i + j])];
            trie[node].indices.insert(i);
        }
    }

    multiset<int> indices;
    for (auto const& pattern : patterns) {
        trie_collect(trie, pattern, d, indices);
    }
    for (auto const& i : indices) {
        cout << i << ' ';
    }
    cout << endl;

    return 0;
}
