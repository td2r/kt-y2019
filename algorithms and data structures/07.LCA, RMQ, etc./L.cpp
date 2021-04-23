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

int const MAXN = (1 << 17) + 3;
int const n = (1 << 17);

vector<unsigned> t[2 * MAXN];

void merge(vector<unsigned> const &a, vector<unsigned> const &b, vector<unsigned> &c) {
    c.resize(a.size() + b.size());
    size_t i = 0;
    size_t j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] < b[j]) {
            c[i + j] = a[i];
            ++i;
        } else {
            c[i + j] = b[j];
            ++j;
        }
    }
    while (i < a.size()) {
        c[i + j] = a[i];
        ++i;
    }
    while (j < b.size()) {
        c[i + j] = b[j];
        ++j;
    }
}

unsigned count_in_range(vector<unsigned> const &v, unsigned x, unsigned y) {
    return upper_bound(v.begin(), v.end(), y) - lower_bound(v.begin(), v.end(), x);
}

unsigned get(unsigned l, unsigned r, unsigned x, unsigned y) {
    l += n - 1;
    r += n - 1;
    unsigned lres = 0;
    unsigned rres = 0;
    while (l < r) {
        if (l % 2 == 0) {
            lres += count_in_range(t[l], x, y);
        }
        l /= 2;
        if (r % 2 == 1) {
            rres += count_in_range(t[r], x, y);
        }
        r = r / 2 - 1;
    }
    if (l == r) {
        lres += count_in_range(t[l], x, y);
    }
    return lres + rres;
}


unsigned a, b;
unsigned cur = 0;
unsigned nextRand17() {
    cur = cur * a + b;
    return (cur >> 15);
}
unsigned nextRand24() {
    cur = cur * a + b;
    return (cur >> 8);
}

void show_tree(int v, int tl, int tr) {
    if (tl + 1 < tr) {
        int tm = (tl + tr) / 2;
        show_tree(2 * v + 1, tl, tm);
        show_tree(2 * v + 2, tm, tr);
    }
    cout << '[' << tl << ", " << tr << ") : ";
    for (auto &x : t[v]) {
        cout << x << ' ';
    } cout << "\n";
}

int main() {
	// file_raii author_ivan_naumov("data");

    int q;
    cin >> q >> a >> b;

    for (int i = 0; i < n; ++i) {
        t[n - 1 + i].emplace_back(nextRand24());
    }

    for (int i = n - 2; i >= 0; --i) {
        merge(t[2 * i + 1], t[2 * i + 2], t[i]);
    }

    unsigned answer = 0;
    while (q--) {
        auto l = nextRand17();
        auto r = nextRand17();
        if (l > r) swap(l, r);
        auto x = nextRand24();
        auto y = nextRand24();
        if (x > y) swap(x, y);
        unsigned c = get(l, r, x, y);
        b += c;
        answer += c;
    }
    cout << answer << endl;

	return 0;
}
