#include <bits/stdc++.h>
#define debug false
#define mp make_pair
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


int str_hash(const char* s, const int mod = 1022869) {
    int h = 0;
    for (; *s != '\0'; ++s) h = (31 * h + *s) % mod;
    return h;
}

const int M = 10;
const int N = 1 << M;

int main() {
    // freopen("multimap.in", "r", stdin); freopen("multimap.out", "w", stdout);
    // freopen("data.in", "r", stdin); freopen("data.out", "w", stdout);

    vector<string> arr;

    for (int i = 0; i < N; ++i) {
        bitset<M> bits(i);
        string s;
        for (int j = 0; j < M; ++j) {
            s.append(bits[j] ? "Aa" : "BB");
        }
        arr.emplace_back(s);
    }

    int k;
    scanf("%d", &k);
    for (int i = 0; i < k; ++i) {
        printf("%s\n", arr[i].c_str());
    }


    // fclose(stdin); fclose(stdout);
    return 0;
}
