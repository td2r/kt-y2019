#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

void read_input(int& k, vector<string>& dna) {
    cin >> k;
    string s;
    while (cin >> s) {
        dna.push_back(s);
    }
}

int get_score_of(string const& pattern, vector<string> const& dnas) {
    int score = 0;
    for (auto const& dna : dnas) {
        int dna_score = INT_MAX;
        for (size_t i = 0; i <= dna.length() - pattern.length(); ++i) {
            int cur_score = 0;
            for (int j = 0; j < pattern.length(); ++j) {
                cur_score += (pattern[j] != dna[i + j]);
            }
            dna_score = min(dna_score, cur_score);
        }
        score += dna_score;
    }
    return score;
}

string kmer_ = "";
void get_median(int k, vector<string> const& dna, int& best_score, string& res) {
    if (kmer_.length() == k) {
        int score = get_score_of(kmer_, dna);
        if (score <= best_score) {
            best_score = score;
            res = kmer_;
        }
        return;
    }
    kmer_.push_back(0);
    for (char c : {'A', 'C', 'G', 'T'}) {
        kmer_.back() = c;
        get_median(k, dna, best_score, res);
    }
    kmer_.pop_back();
}

int main() {
    int k;
    vector<string> dna;
    read_input(k, dna);
    int best_score = INT_MAX;
    string best_kmer = "";
    get_median(k, dna, best_score, best_kmer);
    cout << best_kmer << endl;

    return 0;
}
