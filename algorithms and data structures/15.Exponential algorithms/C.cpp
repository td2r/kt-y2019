#pragma GCC target("avx")
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

// author Naumov Ivan, ITMO M32391, 2021

#include <bits/stdc++.h>
#define DEBUG false
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 2'000 + 1;
int const MAXK = 14 + 2;
int const MAXMASK = (1 << MAXK);
int const MAXVAL = MAXK / 2 * (MAXN - MAXK) + (MAXK / 2) * (MAXK / 2) + (MAXK / 2) * (MAXK / 2 - 1) / 2;
int const MAXPERM = 12870;

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
    int prev;
    int next;

    node_t()
    : prev(0)
    , next(0)
    {}
};

int n, m, k = 0, h;
int a[MAXN];
bool used[MAXN];
int absent[MAXK];
int pos[MAXK];
int out_value[MAXK][MAXK];
int cnt1[MAXPERM][MAXVAL];
int cnt2[MAXPERM][MAXVAL];
int mask_by_ind1[MAXPERM];
int ind_by_mask1[MAXMASK];
int mask_by_ind2[MAXPERM];
int ind_by_mask2[MAXMASK];
int perm_cnt1 = 0;
int perm_cnt2 = 0;

int mask = 0;
int value = 0;
int sz = 0;
int perm[MAXK];
node_t node[MAXK];
bool occur[MAXVAL];
int fenw[MAXK];

int sum(int r) {
    int res = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1) {
        res += fenw[r];
    }
    return res;
}

void inc(int i, int delta) {
    for (; i < MAXK; i = (i | (i + 1))) {
        fenw[i] += delta;
    }
}

int sum(int l, int r) {
    return sum(r) - sum(l - 1);
}

void poll_node(int v) {
    node[node[v].prev].next = node[v].next;
    node[node[v].next].prev = node[v].prev;
}

void put_back(int v) {
    node[node[v].prev].next = v;
    node[node[v].next].prev = v;
}

void generate_permutation1() {
    if (sz == h) {
        occur[value] = true;
        ++cnt1[ind_by_mask1[mask]][value];
        return;
    }
    int old_value = value;
    for (int v = node[0].next; v != 0; v = node[v].next) {
        int i = v - 1;
        poll_node(v);
        perm[sz++] = i;
        inc(i, 1);
        value += out_value[i][sz - 1];
        value += (k - 1 - i) - sum(i + 1, k - 1);
        mask |= (1 << i);

        generate_permutation1();

        mask ^= (1 << i);
        value = old_value;
        inc(i, -1);
        --sz;
        put_back(v);
    }
}

void generate_permutation2() {
    if (sz == k - h) {
        ++cnt2[ind_by_mask2[mask]][value];
        return;
    }
    int old_value = value;
    for (int v = node[0].next; v != 0; v = node[v].next) {
        int i = v - 1;
        poll_node(v);
        perm[sz++] = i;
        inc(i, 1);
        value += sum(i - 1);
        value += out_value[i][h + sz - 1];
        mask |= (1 << i);

        generate_permutation2();

        mask ^= (1 << i);
        value = old_value;
        inc(i, -1);
        --sz;
        put_back(v);
    }
}

int bits_cnt(int x) {
    int res = 0;
    while (x) {
        res += (x & 1);
        x >>= 1;
    }
    return res;
}

int main() {
    scanf("%d %d", &n, &m);
    if (m > n * (n - 1) / 2) {
        printf("0\n");
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        scanf("%d", &a[i]);
        if (a[i] == 0) {
            pos[k++] = i;
        }
        used[a[i]] = true;
    }
    k = 0;
    for (int i = 1; i <= n; ++i) {
        if (!used[i]) {
            absent[k++] = i;
        }
    }
    h = k / 2;

    // print_array("absent", absent, absent + k);
    // print_array("indices", pos, pos + k);

    for (int i = 0; i < k; ++i) {
        int x = absent[i];
        int less_x = 0;
        int greater_x = n - x;
        for (int j = 0; j < k; ++j) {
            if (absent[j] > x) {
                --greater_x;
            }
        }
        int ind = 0;
        for (int j = 0; j < n; ++j) {
            if (a[j] == 0) {
                out_value[i][ind++] = less_x + greater_x;
            } else {
                less_x += (a[j] < x);
                greater_x -= (a[j] > x);
            }
        }
    }

    for (int i = 0; i <= k; ++i) {
        node[i].prev = i - 1;
        node[i].next = i + 1;
    }
    node[0].prev = k;
    node[k].next = 0;

    for (int i = 0; i < (1 << k); ++i) {
        if (bits_cnt(i) == h) {
            mask_by_ind1[perm_cnt1] = i;
            ind_by_mask1[i] = perm_cnt1++;
        }
        if (bits_cnt(i) == k - h) {
            mask_by_ind2[perm_cnt2] = i;
            ind_by_mask2[i] = perm_cnt2++;
        }
    }

    generate_permutation1();
    generate_permutation2();

    for (int i = 0; i < n; ++i) {
        if (a[i] == 0) continue;
        for (int j = i + 1; j < n; ++j) {
            if (a[j] == 0) continue;
            m -= (a[i] < a[j]);
        }
    }

    ll ans = 0;
    vector<int> occurred;
    for (int i = 0; i < MAXVAL; ++i) {
        if (occur[i] && m - MAXVAL < i && i <= m) {
            occurred.emplace_back(i);
        }
    }
    for (int i = 0; i < perm_cnt1; ++i) {
        int mask1 = mask_by_ind1[i];
        int mask2 = (1 << k) - 1 - mask1;
        int j = ind_by_mask2[mask2];
        if (bits_cnt(mask1) != h)
            continue;
        for (int const& val : occurred) {
            if (cnt2[j][m - val] != 0) {
                ans += cnt1[i][val] * cnt2[j][m - val];
            }
        }
    }
    printf("%lld\n", ans);

    return 0;
}
