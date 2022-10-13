#include <bits/stdc++.h>
// #define DEBUG
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef long double floating_t;

floating_t EPS = 1e-9;

floating_t calc_precision(floating_t tp, floating_t fp, floating_t fn) {
    if (tp + fp < EPS)
        return 0.0;
    return (floating_t) tp / (floating_t) (tp + fp);
}

floating_t calc_recall(floating_t tp, floating_t fp, floating_t fn) {
    if (tp + fn < EPS)
        return 0.0;
    return (floating_t) tp / (floating_t) (tp + fn);
}

floating_t f_score(floating_t prec, floating_t recall) {
    if (prec + recall < EPS)
        return 0.0;
    return 2 * prec * recall / (prec + recall);
}

template<typename Iterator>
floating_t w_average(int k, Iterator arr, floating_t const* p) {
    floating_t a = 0.0;
    for (int i = 0; i < k; ++i) {
        a += p[i] * arr[i];
    }
    return a;
}

int const MAXK = 20 + 3;

int cm[MAXK][MAXK];
int TP[MAXK];
int FP[MAXK];
int FN[MAXK];
floating_t prec[MAXK];
floating_t recall[MAXK];
floating_t f[MAXK];

floating_t p[MAXK];

#ifdef DEBUG
template<typename Iterator>
void print_array(int k, Iterator arr, string const& name) {
    cout << name << endl;
    for (int i = 0; i < k; ++i) {
        cout << fixed << setprecision(2) << arr[i] << ' ';
    }
    cout << endl;
}
#endif

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    memset(TP, 0, sizeof(int) * MAXK);
    memset(FP, 0, sizeof(int) * MAXK);
    memset(FN, 0, sizeof(int) * MAXK);
    fill(prec, prec + MAXK, 0.0);
    fill(recall, recall + MAXK, 0.0);
    fill(f, f + MAXK, 0.0);
    fill(p, p + MAXK, 0.0);
    
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            cin >> cm[i][j];
            p[i] += cm[i][j];
        }
    }
    auto all_cnt = accumulate(p, p + k, 0.0);
    for (int i = 0; i < k; ++i) {
        p[i] /= all_cnt;
    }
    
    // print_array(k, p, "p");
    
    for (int c = 0; c < k; ++c) {
        TP[c] = cm[c][c];
        for (int i = 0; i < k; ++i) {
            if (i == c)
                continue;
            FP[c] += cm[i][c];
            FN[c] += cm[c][i];
        }
        prec[c] = calc_precision(TP[c], FP[c], FN[c]);
        recall[c] = calc_recall(TP[c], FP[c], FN[c]);
        f[c] = f_score(prec[c], recall[c]);
    }
    
    // print_array(k, TP, "TP");
    // print_array(k, FP, "FP");
    // print_array(k, FN, "FN");
    // print_array(k, prec, "precision");
    // print_array(k, recall, "recall");
    
    floating_t mean_TP = w_average(k, TP, p);
    floating_t mean_FP = w_average(k, FP, p);
    floating_t mean_FN = w_average(k, FN, p);
    floating_t mean_prec = w_average(k, prec, p);
    floating_t mean_recall = w_average(k, recall, p);
    floating_t mean_f = w_average(k, f, p);
    
        // cout << "mean TP = " << mean_TP << endl;
        // cout << "mean FP = " << mean_FP << endl;
        // cout << "mean FN = " << mean_FN << endl;
        // cout << "mean precision = " << mean_prec << endl;
        // cout << "mean recall = " << mean_recall << endl;
        // cout << "mean f_score = " << mean_f << endl;
    
    cout << fixed << setprecision(42) <<
        f_score(calc_precision(mean_TP, mean_FP, mean_FN), calc_recall(mean_TP, mean_FP, mean_FN)) << endl <<
        f_score(mean_prec, mean_recall) << endl <<
        mean_f << endl;
    
    return 0;
}
