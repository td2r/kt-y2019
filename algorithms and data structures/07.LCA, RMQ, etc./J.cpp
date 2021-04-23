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

struct rectangle {
    int x1;
    int y1;
    int x2;
    int y2;

    rectangle() : x1(0), y1(0), x2(0), y2(0) {}

    rectangle(int x1, int y1, int x2, int y2)
        : x1(x1), y1(y1), x2(x2), y2(y2) {}

    void scan() {
        scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
    }

    void print() const {
        printf("%d %d %d %d\n", x1, y1, x2, y2);
    }
};

bool operator < (rectangle const &r1, rectangle const &r2) {
    return r1.x1 < r2.x1;
}

int const MAXN = 65'536 * 2;

rectangle rect[MAXN];
int to_add[2 * MAXN];
vector<int> inc[MAXN];
vector<int> decr[MAXN];
vector<int> allx(MAXN);
vector<int> ally(MAXN);
pair<int, int> t[2 * MAXN];

void build(int v, int tl, int tr) {
    if (tl + 1 == tr) {
        t[v] = make_pair(0, tl);
    } else {
        int tm = (tl + tr) / 2;
        build(2 * v + 1, tl, tm);
        build(2 * v + 2, tm, tr);
        t[v] = max(t[2 * v + 1], t[2 * v + 2]);
    }
}

void push(int v, int tl, int tr) {
    if (to_add[v] != 0) {
        if (tl + 1 < tr) {
            t[2 * v + 1].first += to_add[v];
            t[2 * v + 2].first += to_add[v];
            to_add[2 * v + 1] += to_add[v];
            to_add[2 * v + 2] += to_add[v];
        }
        to_add[v] = 0;
    }
}

void add(int v, int tl, int tr, int l, int r, int a) {
    push(v, tl, tr);
    if (l <= tl && tr <= r) {
        t[v].first += a;
        to_add[v] += a;
    } else if (!(r <= tl || tr <= l)) {
        int tm = (tl + tr) / 2;
        add(2 * v + 1, tl, tm, l, r, a);
        add(2 * v + 2, tm, tr, l, r, a);
        t[v] = max(t[2 * v + 1], t[2 * v + 2]);
    }
}

pair<int, int> get(int v, int tl, int tr, int l, int r) {
    push(v, tl, tr);
    if (l <= tl && tr <= r) {
        return t[v];
    } else if (r <= tl || l <= tr) {
        return make_pair(INT_MIN, INT_MIN);
    } else {
        int tm = (tl + tr) / 2;
        return max(get(2 * v + 1, tl, tm, l, r),
                get(2 * v + 2, tm, tr, l, r));
    }
}

inline int to_new(vector<int> const &axis, int c) {
    return lower_bound(axis.begin(), axis.end(), c) - axis.begin();
}

int main() {
	// file_raii author_ivan_naumov("data");

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        rect[i].scan();
        allx.emplace_back(rect[i].x1);
        allx.emplace_back(rect[i].x2);
        ally.emplace_back(rect[i].y1);
        ally.emplace_back(rect[i].y2);
    }

    sort(allx.begin(), allx.end());
    sort(ally.begin(), ally.end());
    allx.resize(unique(allx.begin(), allx.end()) - allx.begin());
    ally.resize(unique(ally.begin(), ally.end()) - ally.begin());
    allx.shrink_to_fit();
    ally.shrink_to_fit();

    for (int i = 0; i < n; ++i) {
        rect[i].x1 = to_new(allx, rect[i].x1);
        rect[i].y1 = to_new(ally, rect[i].y1);
        rect[i].x2 = to_new(allx, rect[i].x2);
        rect[i].y2 = to_new(ally, rect[i].y2);
        inc[rect[i].x1].emplace_back(i);
        decr[rect[i].x2].emplace_back(i);
    }

    int maxx = (int)allx.size();
    int maxy = (int)ally.size();
    build(0, 0, maxy);
    int max_cover = -1;
    int ansx = INT_MIN;
    int ansy = INT_MIN;
    for (int i = 0; i < maxx; ++i) {
        for (int j : inc[i]) {
            add(0, 0, maxy, rect[j].y1, rect[j].y2 + 1, 1);
        }

        if (t[0].first > max_cover) {
            max_cover = t[0].first;
            ansx = i;
            ansy = t[0].second;
        }

        for (int j : decr[i]) {
            add(0, 0, maxy, rect[j].y1, rect[j].y2 + 1, -1);
        }
    }

    printf("%d\n%d %d\n", max_cover, allx[ansx], ally[ansy]);

	return 0;
}
