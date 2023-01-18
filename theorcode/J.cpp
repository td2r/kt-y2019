#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

void print_bits(uint32_t x, uint32_t n = 32) {
    for (uint32_t i = 0; i < n; ++i) {
        cout << ((x >> (n - 1 - i)) & 1);
    }
}

vector<uint32_t> gf_pow;
vector<uint32_t> gf_log;
vector<vector<uint32_t>> cyclotomic_class;
vector<uint32_t> ci_of_elem;
vector<vector<uint32_t>> min_poly;
vector<uint32_t> g;
vector<vector<uint32_t>> mul_table;
vector<uint32_t> rev_table;

inline uint32_t gf_mul(uint32_t a1, uint32_t a2) {
    // if (!a1 || !a2)
    //     return 0;
    // return gf_pow[(gf_log[a1] + gf_log[a2]) % gf_pow.size()];
    return mul_table[a1][a2];
}

// inline uint32_t gf_mul1(uint32_t l1, uint32_t a2) {
//     if (!a2)
//         return 0;
//     return gf_pow[(l1 + gf_log[a2]) % gf_pow.size()];
// }

// inline uint32_t gf_reverse_log(uint32_t l) {
//     return (gf_pow.size() - l) % gf_pow.size();
// }

inline uint32_t gf_reverse(uint32_t a) {
    // return gf_pow[gf_reverse_log(gf_log[a])];
    return rev_table[a];
}

vector<uint32_t> mul_buffer;

void mul_poly_by(vector<uint32_t>& lhs, vector<uint32_t> const& rhs) {
    fill(mul_buffer.begin(), mul_buffer.end(), 0);
    mul_buffer.resize(lhs.size() + rhs.size() - 1, 0);
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] == 0)
            continue;
        // uint32_t log_i = gf_log[lhs[i]];
        for (size_t j = 0; j < rhs.size(); ++j) {
            mul_buffer[i + j] ^= gf_mul(lhs[i], rhs[j]);
        }
    }
    lhs.swap(mul_buffer);
}

void build_gf(uint32_t n, uint32_t p) {
    gf_pow.resize(n);
    gf_log.resize(n + 1, UINT32_MAX);
    uint32_t ai = 1;
    for (int i = 0; i < n; ++i) {
        gf_pow[i] = ai;
        gf_log[ai] = i;
        ai <<= 1;
        if (ai > n) {
            ai ^= p;
        }
    }
    mul_table.resize(n + 1);
    mul_table[0].resize(n + 1, 0);
    for (uint32_t i = 1; i <= n; ++i) {
        mul_table[i].resize(n + 1, 0);
        uint32_t gf_log_i = gf_log[i];
        for (uint32_t j = 1; j <= n; ++j) {
            mul_table[i][j] = gf_pow[(gf_log_i + gf_log[j]) % n];
        }
    }
    rev_table.resize(n + 1);
    rev_table[1] = 1;
    for (uint32_t i = 2; i <= n; ++i) {
        rev_table[i] = gf_pow[(n - gf_log[i]) % n];
    }

    // for (size_t i = 0; i < gf_pow.size(); ++i) {
    //     cout << "gf_pow[" << i << "] = " << gf_pow[i] << endl;
    // }
    // for (size_t i = 0; i < gf_log.size(); ++i) {
    //     cout << "gf_log[" << i << "] = " << gf_log[i] << endl;
    // }
    // cout << endl;
}

void build_cyclotomic_classes(uint32_t n) {
    ci_of_elem.resize(n, UINT32_MAX);
    for (uint32_t i = 0; i < n; ++i) {
        if (ci_of_elem[i] != UINT32_MAX)
            continue;
        uint32_t ind = cyclotomic_class.size();
        cyclotomic_class.emplace_back();
        uint32_t j = i;
        while (ci_of_elem[j] == UINT32_MAX) {
            cyclotomic_class[ind].emplace_back(j);
            ci_of_elem[j] = ind;
            j = (j << 1) % n;
        }
    }

    // for (size_t i = 0; i < cyclotomic_class.size(); ++i) {
    //     cout << "cyclotomic_class[" << i << "]:";
    //     for (auto const& j : cyclotomic_class[i]) {
    //         cout << ' ' << j;
    //     }
    //     cout << endl;
    // }
    // for (uint32_t i = 0; i < n; ++i) {
    //     cout  << "ci_of_elem[" << i << "] = " << ci_of_elem[i] << endl;
    // }
    // cout << endl;
}

void build_minimal_polys() {
    min_poly.resize(cyclotomic_class.size(), {1});
    for (size_t i = 0; i < min_poly.size(); ++i) {
        for (uint32_t const& j : cyclotomic_class[i]) {
            mul_poly_by(min_poly[i], {gf_pow[j], 1});
        }
    }

    // for (size_t i = 0; i < min_poly.size(); ++i) {
    //     cout << "M[" << i << "]: " << min_poly[i][0];
    //     if (min_poly[i].size() > 1) {
    //         cout << " + " << min_poly[i][1] <<  "x";
    //     }
    //     for (size_t j = 2; j < min_poly[i].size(); ++j) {
    //         cout << " + " << min_poly[i][j] << "x^" << j;
    //     }
    //     cout << endl;
    // }
    // cout << endl;
}

void build_g(uint32_t d) {
    set<uint32_t> classes;
    for (uint32_t i = 1; i < d; ++i) {
        classes.insert(ci_of_elem[i]);
    }
    g = {1};
    for (uint32_t i : classes) {
        mul_poly_by(g, min_poly[i]);
    }

    // cout << "g: " << g[0];
    // if (g.size() > 1) {
    //     cout << " + " << g[1] << "x";
    // }
    // for (size_t i = 2; i < g.size(); ++i) {
    //     cout << " + " << g[i] << "x^" << i;
    // }
    // cout << endl << endl;
}

vector<uint32_t> mod_buffer;

void encode(vector<uint32_t>& v) {
    uint32_t r = g.size() - 1;
    copy(v.begin(), v.end(), mod_buffer.begin());
    for (int i = mod_buffer.size() - 1; i >= r; --i) {
        if (mod_buffer[i]) {
            for (uint32_t j = 0; j <= r; ++j) {
                mod_buffer[i - j] ^= g[r - j];
            }
        }
    }
    copy(mod_buffer.begin(), mod_buffer.begin() + r, v.begin());
}

vector<uint32_t> syndrome;
vector<uint32_t> A;
vector<uint32_t> B;
vector<uint32_t> T;

uint32_t decode(vector<uint32_t>& y) {
    uint32_t d = syndrome.size();
    memset(syndrome.data(), 0, sizeof(decltype(syndrome)::value_type) * d);
    for (uint32_t i = 1; i < d; ++i) {
        uint32_t alpha_i = gf_pow[i];
        uint32_t xi = 1;
        for (uint32_t yi : y) {
            if (yi) {
                syndrome[i] ^= xi;
            }
            xi = gf_mul(xi, alpha_i);
        }
    }
    uint32_t L = 0;
    // A = {1};
    A.resize(1);
    A[0] = 1;
    // B = {1};
    B.resize(1);
    B[0] = 1;
    for (uint32_t i = 1; i < d; ++i) {
        uint32_t delta = 0;
        if (i & 1) {
            for (uint32_t j = 0; j <= L; ++j) {
                delta ^= gf_mul(A[j], syndrome[i - j]);
            }
        }
        B.insert(B.begin(), 0);
        if (delta) {
            // uint32_t delta_log = gf_log[delta];
            T.resize(max(A.size(), B.size()));
            memcpy(T.data(), A.data(), sizeof(decltype(A)::value_type) * A.size());
            memset(T.data() + A.size(), 0,
                    sizeof(decltype(T)::value_type) * (T.size() - A.size()));
            for (size_t j = 0; j < B.size(); ++j) {
                T[j] ^= gf_mul(delta, B[j]);
            }
            if (2 * L <= i - 1) {
                uint32_t r_delta = gf_reverse(delta);
                for (size_t j = 0; j < A.size(); ++j) {
                    A[j] = gf_mul(r_delta, A[j]);
                }
                B.swap(A);
                L = i - L;
            }
            A.swap(T);
        }
    }
    if (A.size() - 1 == L) {
        for (uint32_t i = 0; i < gf_pow.size(); ++i) {
            uint32_t val = 0;
            uint32_t r_ai = gf_reverse(gf_pow[i]);
            uint32_t xi = 1;
            for (auto const& Ai : A) {
                val ^= gf_mul(xi, Ai);
                xi = gf_mul(xi, r_ai);
            }
            if (!val) {
                y[i] ^= 1;
            }
        }
        return 0;
    } else {
        return 1;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    uint32_t n;
    uint32_t p;
    uint32_t d;
    cin >> n >> p >> d;

    build_gf(n, p);
    build_cyclotomic_classes(n);
    build_minimal_polys();
    build_g(d);

    uint32_t r = g.size() - 1;
    uint32_t k = n - r;
    vector<uint32_t> encoded(n);
    vector<uint32_t> decoded(n);
    mod_buffer.resize(n);
    syndrome.resize(d);
    A.reserve(d);
    B.reserve(d);
    T.reserve(d);
    cout << k << endl;
    for (auto const& it : g) {
        cout << it << ' ';
    }
    cout << endl;

    string s;
    random_device rd;
    mt19937 rng(rd());
    uniform_real_distribution ugen(0.0, 1.0);
    uniform_int_distribution<random_device::result_type> random_bit(0, 1);
    while (cin >> s) {
        if (s[0] == 'E') {
            fill(encoded.begin(), encoded.begin() + r, 0);
            for (int i = 0; i < k; ++i) {
                cin >> encoded[r + i];
            }
            encode(encoded);
            for (auto const& it : encoded) {
                cout << it << ' ';
            }
            cout << endl;
        } else if (s[0] == 'D') {
            for (uint32_t i = 0; i < n; ++i) {
                cin >> decoded[i];
            }
            if (decode(decoded)) {
                cout << "FAILURE" << endl;
            } else {
                for (auto const& it : decoded) {
                    cout << it << ' ';
                }
                cout << endl;
            }
        } else if (s[0] == 'S') {
            double pr;
            uint32_t iterations_max;
            uint32_t errors_max;
            cin >> pr >> iterations_max >> errors_max;
            uint32_t iters = 0;
            uint32_t errs = 0;
            while (iters < iterations_max && errs < errors_max) {
                memset(encoded.data(), 0, sizeof(decltype(encoded)::value_type) * r);
                generate(encoded.begin() + r, encoded.end(),
                        [&]() { return random_bit(rng); });
                encode(encoded);
                memcpy(decoded.data(), encoded.data(),
                        sizeof(decltype(encoded)::value_type) * encoded.size());
                for (size_t i = 0; i < decoded.size(); ++i) {
                    if (ugen(rng) <= pr) {
                        decoded[i] ^= 1;
                    }
                }
                decode(decoded);
                for (size_t i = 0; i < encoded.size(); ++i) {
                    if (encoded[i] != decoded[i]) {
                        ++errs;
                        break;
                    }
                }
                ++iters;
            }
            cout << fixed << setprecision(42) << (double) errs / (double) iters << endl;
        }
    }

    fclose(stdout);
    fclose(stdin);

    return 0;
}
