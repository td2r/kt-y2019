#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <utility>
#include <numeric>
#define mp make_pair
#define uint unsigned int
#define ll long long
#define ull unsigned long long
using namespace std;


void sorting_net(vector<int> &a, vector<pair<int, int>> &cmps) {
    int n = (int)a.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            cmps.emplace_back(mp(a[i], a[j]));
        }
    }
}


int main() {
    int n;
    while (scanf("%d", &n) != -1 && n) {
        int x = -1, y = -1;
        vector<pair<int, int>> cmps;
        vector<int> A0, A1;
        for (int i = 0, b; i < n; ++i) {
            scanf("%d", &b);
            if (!b) {
                A0.emplace_back(i);
                y = i;
            } else {
                A1.emplace_back(i);
                if (x == -1) {
                    x = i;
                }
            }
        }
        if (x == -1 || y == -1 || y < x) {
            printf("-1\n");
        } else {
            sorting_net(A0, cmps);
            sorting_net(A1, cmps);
            vector<int> A(n);
            iota(A.begin(), A.end(), 0);
            A.erase(A.begin() + y);
            A.erase(A.begin() + x);
            sorting_net(A, cmps);
            iota(A0.begin(), A0.end(), 0);
            sorting_net(A0, cmps);
            iota(A1.begin(), A1.end(), (int)A0.size());
            sorting_net(A1, cmps);
            printf("%zu\n", cmps.size());
            for (pair<int, int> &i : cmps) {
                printf("%d %d\n", i.first + 1, i.second + 1);
            }
        }
    }
    return 0;
}
