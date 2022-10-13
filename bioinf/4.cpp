#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

int const MAXN = 100 + 3;
int const SCORE_SZ = 256;

int so = 11;
int se = 1;
int score[SCORE_SZ][SCORE_SZ];

vector<string> score_strs = {
    "   A  C  D  E  F  G  H  I  K  L  M  N  P  Q  R  S  T  V  W  Y",
    "A  4  0 -2 -1 -2  0 -2 -1 -1 -1 -1 -2 -1 -1 -1  1  0  0 -3 -2",
    "C  0  9 -3 -4 -2 -3 -3 -1 -3 -1 -1 -3 -3 -3 -3 -1 -1 -1 -2 -2",
    "D -2 -3  6  2 -3 -1 -1 -3 -1 -4 -3  1 -1  0 -2  0 -1 -3 -4 -3",
    "E -1 -4  2  5 -3 -2  0 -3  1 -3 -2  0 -1  2  0  0 -1 -2 -3 -2",
    "F -2 -2 -3 -3  6 -3 -1  0 -3  0  0 -3 -4 -3 -3 -2 -2 -1  1  3",
    "G  0 -3 -1 -2 -3  6 -2 -4 -2 -4 -3  0 -2 -2 -2  0 -2 -3 -2 -3",
    "H -2 -3 -1  0 -1 -2  8 -3 -1 -3 -2  1 -2  0  0 -1 -2 -3 -2  2",
    "I -1 -1 -3 -3  0 -4 -3  4 -3  2  1 -3 -3 -3 -3 -2 -1  3 -3 -1",
    "K -1 -3 -1  1 -3 -2 -1 -3  5 -2 -1  0 -1  1  2  0 -1 -2 -3 -2",
    "L -1 -1 -4 -3  0 -4 -3  2 -2  4  2 -3 -3 -2 -2 -2 -1  1 -2 -1",
    "M -1 -1 -3 -2  0 -3 -2  1 -1  2  5 -2 -2  0 -1 -1 -1  1 -1 -1",
    "N -2 -3  1  0 -3  0  1 -3  0 -3 -2  6 -2  0  0  1  0 -3 -4 -2",
    "P -1 -3 -1 -1 -4 -2 -2 -3 -1 -3 -2 -2  7 -1 -2 -1 -1 -2 -4 -3",
    "Q -1 -3  0  2 -3 -2  0 -3  1 -2  0  0 -1  5  1  0 -1 -2 -2 -1",
    "R -1 -3 -2  0 -3 -2  0 -3  2 -2 -1  0 -2  1  5 -1 -1 -3 -3 -2",
    "S  1 -1  0  0 -2  0 -1 -2  0 -2 -1  1 -1  0 -1  4  1 -2 -3 -2",
    "T  0 -1 -1 -1 -2 -2 -2 -1 -1 -1 -1  0 -1 -1 -1  1  5  0 -2 -2",
    "V  0 -1 -3 -2 -1 -3 -3  3 -2  1  1 -3 -2 -2 -3 -2  0  4 -3 -1",
    "W -3 -2 -4 -3  1 -2 -2 -3 -3 -2 -1 -4 -4 -2 -3 -3 -2 -3 11  2",
    "Y -2 -2 -3 -2  3 -3  2 -1 -2 -1 -1 -2 -3 -1 -2 -2 -2 -1  2  7"
};

void init_score() {
    memset(score, 0, sizeof(int) * SCORE_SZ * SCORE_SZ);
    vector<int> letter({});
    for (size_t i = 0; i < score_strs[0].length(); ++i) {
        while (i < score_strs[0].length() && score_strs[0][i] == ' ')
            ++i;
        letter.push_back(score_strs[0][i] - 'A');
    }
    for (size_t i = 0; i < letter.size(); ++i) {
        stringstream ss(score_strs[i + 1]);
        int cur_letter = ss.get() - 'A';
        for (size_t j = 0; j < letter.size(); ++j) {
            ss >> score[cur_letter][letter[j]];
        }
    }
}

int const INF = 1e6;

int n1;
string s1;
int n2;
string s2;

int I[MAXN][MAXN];
int D[MAXN][MAXN];
int M[MAXN][MAXN];
char Ip[MAXN][MAXN];
char Dp[MAXN][MAXN];
char Mp[MAXN][MAXN];

int main() {
    init_score();
    string s1;
    string s2;
    cin >> s1 >> s2;
    n1 = (int) s1.length();
    n2 = (int) s2.length();

    I[0][0] = D[0][0] = M[0][0] = 0;
    if (n1 > 0) I[1][0] = D[1][0] = M[1][0] = -so;
    if (n2 > 0) I[0][1] = D[0][1] = M[0][1] = -so;
    for (int i = 2; i <= n1; ++i) {
        I[i][0] = D[i][0] = M[i][0] = I[i - 1][0] - se;
    }
    for (int j = 2; j <= n2; ++j) {
        I[0][j] = D[0][j] = M[0][j] = D[0][j - 1] - se;
    }

    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            I[i][j] = max(I[i - 1][j] - se, M[i - 1][j] - so);
            D[i][j] = max(D[i][j - 1] - se, M[i][j - 1] - so);
            M[i][j] = max({
                I[i][j],
                D[i][j],
                M[i - 1][j - 1] + score[s1[i - 1] - 'A'][s2[j - 1] - 'A']
            });
        }
    }

    cout << M[n1][n2] << endl;

    int i = n1;
    int j = n2;
    char cur_table = 'M';
    string ans1 = "";
    string ans2 = "";
    while (i != 0 && j != 0) {
        if (cur_table == 'M') {
            if (M[i][j] == I[i][j]) {
                cur_table = 'I';
            } else if (M[i][j] == D[i][j]) {
                cur_table = 'D';
            } else { // == M[i-1][j-1] + score[x][y]
                ans1.push_back(s1[i - 1]);
                ans2.push_back(s2[j - 1]);
                --i, --j;
                // cur_table still M
            }
        } else if (cur_table == 'I') {
            ans1.push_back(s1[i - 1]);
            ans2.push_back('-');
            if (I[i][j] == M[i - 1][j] - so)
                cur_table = 'M';
            --i;
        } else { // cur_table == 'D'
            ans1.push_back('-');
            ans2.push_back(s2[j - 1]);
            if (D[i][j] == M[i][j - 1] - so)
                cur_table = 'M';
            --j;
        }
    }

    while (i != 0) {
        ans2.push_back('-');
        --i;
    }
    while (j != 0) {
        ans1.push_back('-');
        --j;
    }

    reverse(ans1.begin(), ans1.end());
    reverse(ans2.begin(), ans2.end());
    cout << ans1 << endl << ans2 << endl;

    return 0;
}
