#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;


struct file_raii {
	file_raii(string const & filename) {
		freopen((filename + ".in").c_str(), "r", stdin);
		freopen((filename + ".out").c_str(), "w", stdout);
	}

	file_raii(int const problem_number) {
		freopen(("problem" + to_string(problem_number) + ".in").c_str(), "r", stdin);
		freopen(("problem" + to_string(problem_number) + ".out").c_str(), "w", stdout);
	}

	~file_raii() {
		fclose(stdout);
		fclose(stdin);
	}
};

int const MAXN = 500'000 + 3;
int const MAXL = 19 + 3;

int h[MAXN];
int jmp[MAXL][MAXN];

int main() {
	// file_raii author_ivan_naumov("data");

    int m;
    scanf("%d", &m);
    while (m--) {
        int a, b;
        char s[20];
        scanf("%s %d %d", s, &a, &b);
        --a, --b;
        if (s[0] == 'A') {
            h[b] = h[a] + 1;
            jmp[0][b] = a;
            for (int i = 1; i < MAXL; ++i) {
                jmp[i][b] = jmp[i - 1][jmp[i - 1][b]];
            }
        } else {
            while (h[a] > h[b]) {
                a = jmp[0][a];
            }
            while (h[b] > h[a]) {
                b = jmp[0][b];
            }
            for (int i = MAXL - 1; i >= 0; --i) {
                if (jmp[i][a] != jmp[i][b]) {
                    a = jmp[i][a];
                    b = jmp[i][b];
                }
            }
            printf("%d\n", (a == b ? a : jmp[0][a]) + 1);
        }
    }

	return 0;
}
