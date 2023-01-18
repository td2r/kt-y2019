#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;

uint32_t const NO_PREV = -1;
double const DOUBLE_INF = numeric_limits<double>::infinity();

inline bool xor_bool(bool lhs, bool rhs) {
    return lhs != rhs;
}

void print_bits(uint32_t mask) {
    for (uint32_t i = 0; i < 32; ++i) {
        uint32_t j = 31 - i;
        printf("%u", (mask >> j) & 1);
        // cout << ((mask & (1 << j)) >> j);
    }
    printf("\n");
}

// index in 0-indexation
inline uint32_t insert_bit(uint32_t mask, uint32_t index, uint32_t value) {
    uint32_t lpart = ((mask >> index) << (index + 1));
    uint32_t filter_mask = (1 << index) - 1;
    uint32_t rpart = (mask & filter_mask);
    // printf("mask = ");
    // print_bits(mask);
    // printf("lpart = ");
    // print_bits(lpart);
    // printf("filter_mask = ");
    // print_bits(filter_mask);
    // printf("rpart = ");
    // print_bits(rpart);
    // printf("result = ");
    // print_bits(lpart | (value << index) | rpart);
    return (lpart | (value << index) | rpart);
}

inline void sub_from(vector<bool>& lhs, vector<bool> const& rhs, int from, int to) {
    for (int i = from; i < to; ++i) {
        lhs[i] = xor_bool(lhs[i], rhs[i]);
    }
}

inline void read_G(vector<vector<bool>>& G, int n, int k) {
    int tmp_int;
    for (int i = 0; i < k; ++i) {
        G[i].resize(n);
        for (int j = 0; j < n; ++j) {
            scanf("%d", &tmp_int);
            G[i][j] = tmp_int;
        }
    }
}

inline void print_G(vector<vector<bool>>& G) {
    for (size_t i = 0; i < G.size(); ++i) {
        for (size_t j = 0; j < G[i].size(); ++j) {
            printf("%d ", (int) G[i][j]);
        }
        printf("\n");
    }
}

inline void print_line() {
    printf("=================================================================\n");
}

inline void to_MSF(vector<vector<bool>>& G, int n, int k,
                   vector<int>& activates_in,
                   vector<int>& deactivates_in)
{
    int row = 0;
    for (int col = 0; row < k && col < n; ++col) {
        int non_zero_row = -1;
        for (int i = row; i < k; ++i) {
            if (G[i][col]) {
                non_zero_row = i;
                break;
            }
        }
        if (non_zero_row == -1)
            continue;

        swap(G[non_zero_row], G[row]);
        activates_in[col] = row;
        ++row;

        for (int i = row; i < k; ++i) {
            if (G[i][col]) {
                sub_from(G[i], G[non_zero_row], col, n);
            }
        }
    }
    // print_G(G);
    // print_line();
    while (--row >= 0) {
        int end_col = n - 1;
        while (!G[row][end_col]) {
            --end_col;
        }
        deactivates_in[end_col] = row;
        for (int i = 0; i < row; ++i) {
            if (G[i][end_col]) {
                sub_from(G[i], G[row], 0, end_col + 1);
            }
        }
    }
    // remove rows with single 1
    for (int i = 0; i < n; ++i) {
        if (activates_in[i] != -1 && activates_in[i] == deactivates_in[i]) {
            activates_in[i] = deactivates_in[i] = -1;
        }
    }
}

void fill_nodes_vector_by_mask_and_de_activated_row(
        vector<int>& nodes,
        uint32_t mask,
        int de_ad,
        vector<int> const& active)
{
    if (de_ad == -1) {
        nodes.resize(1);
        nodes[0] = mask;
    } else {
        nodes.resize(2);
        for (uint32_t i = 0; i < 2; ++i) {
            uint32_t const pos =
                lower_bound(active.begin(), active.end(), de_ad) - active.begin();
            nodes[i] = insert_bit(mask, pos, i);
        }
    }
}

void build_grid(vector<vector<int32_t>>& grid,
                int const n,
                int const k,
                vector<vector<bool>> G)
{
    vector<int> activates_in(n, -1);
    vector<int> deactivates_in(n, -1);
    to_MSF(G, n, k, activates_in, deactivates_in);

    // print_G(G);
    // for (int i = 0; i < n; ++i) {
    //     if (activates_in[i] != -1) {
    //         printf("activates_in[%d] = %d\n", i, activates_in[i]);
    //     }
    //     if (deactivates_in[i] != -1) {
    //         printf("deactivates_in[%d] = %d\n", i, deactivates_in[i]);
    //     }
    // }
     
    grid.resize(n + 1);
    grid[0].resize(2, NO_PREV);
    vector<int> active;
    active.reserve(k);
    int ad = -1; // activated row index
    int dead = -1; // deactivated row index
    vector<int> prev_nodes(2);
    vector<int> next_nodes(2);
    for (int i = 0; i < n; ++i) {
        ad = activates_in[i];
        dead = deactivates_in[i];
        if (dead != -1) {
            active.erase(find(active.begin(), active.end(), dead));
        }
        uint32_t mask_max = (1 << active.size());
        grid[i + 1].resize(
                // +1 for grid's layer structure
                (1 << (active.size() + (ad != -1) + 1)), NO_PREV);
        for (uint32_t mask = 0; mask < mask_max; ++mask) {
            fill_nodes_vector_by_mask_and_de_activated_row(
                    next_nodes, mask, ad, active);
            fill_nodes_vector_by_mask_and_de_activated_row(
                    prev_nodes, mask, dead, active);
            uint32_t prod_base = 0;
            for (size_t j = 0; j < active.size(); ++j) {
                prod_base ^= (((mask >> j) & 1) & (uint32_t) G[active[j]][i]);
            }
            // printf("ad = %d, dead = %d, mask = %d\n", ad + 1, dead + 1, (int) mask);
            // printf("active:");
            // for (auto const& it : active)
            //     printf(" %d", it);
            // printf("\n");
            // printf("prev_nodes:");
            // for (size_t j1 = 0; j1 < prev_nodes.size(); ++j1) {
            //     printf(" %d", prev_nodes[j1]);
            // }
            // printf("\n");
            // printf("next_nodes:");
            // for (size_t j2 = 0; j2 < next_nodes.size(); ++j2) {
            //     printf(" %d", next_nodes[j2]);
            // }
            // printf("\n");
            for (size_t j1 = 0; j1 < prev_nodes.size(); ++j1) {
                for (size_t j2 = 0; j2 < next_nodes.size(); ++j2) {
                    uint32_t w = prod_base;
                    if (dead != -1)
                        w ^= (j1 & (uint32_t) G[dead][i]);
                    if (ad != -1)
                        w ^= (j2 & (uint32_t) G[ad][i]);
                    // printf("i = %d, j1 = %d, j2 = %d\n",
                    //         i + 1, (int) j1, (int) j2);
                    grid[i + 1][2 * next_nodes[j2] + w] = prev_nodes[j1];
                }
            }
            // printf("\n");
        }
        if (ad != -1) {
            active.insert(lower_bound(active.begin(), active.end(), ad), ad);
        }
    }
}

char buffer[200];

void encode(vector<bool> const& src_seq,
            vector<bool>& dst_seq,
            vector<vector<bool>> const& G,
            int n,
            int k)
{
    for (int j = 0; j < n; ++j) {
        dst_seq[j] = false;
        for (int i = 0; i < k; ++i) {
            dst_seq[j] = xor_bool(dst_seq[j], src_seq[i] && G[i][j]);
        }
    }
}

void decode(vector<double> const& src_code,
            vector<bool>& decoded,
            vector<vector<int32_t>> const& grid,
            vector<vector<double>>& mind,
            vector<vector<int>>& parent)
{
    mind[0][0] = 0.0;
    for (size_t i = 0; i < src_code.size(); ++i) {
        for (size_t j = 0; j < mind[i + 1].size(); ++j) {
            mind[i + 1][j] = -DOUBLE_INF;
            if (grid[i + 1][2 * j] != NO_PREV) {
                mind[i + 1][j] = mind[i][grid[i + 1][2 * j]] + src_code[i];
                parent[i + 1][j] = 0;
            }
            if (grid[i + 1][2 * j + 1] != NO_PREV) {
                double newd = mind[i][grid[i + 1][2 * j + 1]] - src_code[i];
                if (newd > mind[i + 1][j]) {
                    mind[i + 1][j] = newd;
                    parent[i + 1][j] = 1;
                }
            }
        }
    }
    int node = 0;
    for (size_t i = 0; i < src_code.size(); ++i) {
        int p = parent[parent.size() - 1 - i][node];
        decoded[decoded.size() - 1 - i] = p;
        node = grid[grid.size() - 1 - i][2 * node + p];
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;
    int k;
    scanf("%d %d", &n, &k);
    vector<vector<bool>> G(k);
    read_G(G, n, k);
    vector<vector<int32_t>> grid;
    build_grid(grid, n, k, G);

    // for (int i = 1; i <= n; ++i) {
    //     printf("layer %d\n", i);
    //     for (size_t j = 0; j < grid[i].size(); ++j) {
    //         if (grid[i][j] != NO_PREV) {
    //             printf("%d -> %d, w = %d\n", grid[i][j], (int) j / 2, (int) (j & 1));
    //         }
    //     }
    // }

    for (int i = 0; i <= n; ++i) {
        printf("%d ", (int) grid[i].size() / 2);
    }
    printf("\n");

    vector<bool> src_seq(k);
    vector<bool> encoded(n);
    vector<double> src_code(n);
    vector<bool> decoded(n);
    vector<vector<double>> mind(n + 1);
    vector<vector<int>> parent(n + 1);
    for (int i = 0; i <= n; ++i) {
        mind[i].resize(grid[i].size() / 2);
        parent[i].resize(mind[i].size());
    }
    long seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine engine(seed);
    random_device rd;
    uniform_int_distribution random_bit(0, 1);

    while (scanf("%s", buffer) != -1) {
        if (buffer[0] == 'E') {
            int tmp_int;
            for (int i = 0; i < k; ++i) {
                scanf("%d", &tmp_int);
                src_seq[i] = tmp_int;
            }
            encode(src_seq, encoded, G, n, k);
            for (int i = 0; i < n; ++i) {
                printf("%d ", (int) encoded[i]);
            }
            printf("\n");
        } else if (buffer[0] == 'D') {
            for (int i = 0; i < n; ++i) {
                scanf("%lf", &src_code[i]);
            }
            decode(src_code, decoded, grid, mind, parent);
            for (int i = 0; i < n; ++i) {
                printf("%d ", (int) decoded[i]);
            }
            printf("\n");
        } else { // buffer[0] == 'S'
            double noise_level;
            int iterations_max;
            int errors_max;
            scanf("%lf %d %d", &noise_level, &iterations_max, &errors_max);
            double stddev = sqrt((0.5 * n / k) * pow(10, -noise_level / 10));
            normal_distribution normal(0.0, stddev);
            int iters = 0;
            int errs = 0;
            while (iters < iterations_max && errs < errors_max) {
                ++iters;
                generate(src_seq.begin(), src_seq.end(), [&](){ return (bool) random_bit(rd); });
                encode(src_seq, encoded, G, n, k);
                for (int i = 0; i < n; ++i) {
                    src_code[i] = (double) (1 - 2 * (int) encoded[i]) + normal(engine);
                }
                decode(src_code, decoded, grid, mind, parent);
                for (int i = 0; i < n; ++i) {
                    if (encoded[i] != decoded[i]) {
                        ++errs;
                        break;
                    }
                }
                // printf("src_seq:");
                // for (int i = 0; i < k; ++i) {
                //     printf(" %d", (int) src_seq[i]);
                // }
                // printf("\n");
                // printf("encoded:");
                // for (int i = 0; i < n; ++i) {
                //     printf(" %d", (int) encoded[i]);
                // }
                // printf("\n");
                // printf("src_code:");
                // for (int i = 0; i < n; ++i) {
                //     printf(" %lf", src_code[i]);
                // }
                // printf("\n");
                // printf("decoded:");
                // for (int i = 0; i < n; ++i) {
                //     printf(" %d", (int) decoded[i]);
                // }
                // printf("\n\n");
            }
            printf("%.42lf\n", (double) errs / (double) iters);
        }
    }

    fclose(stdout);
    fclose(stdin);

    return 0;
}
