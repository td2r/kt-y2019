#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <cstring>

void prefix_function(const char* s, unsigned* pi, const size_t n) {
    pi[0] = 0U;
    for (size_t i = 1; i < n; ++i) {
        unsigned j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        j += (s[i] == s[j]);
        pi[i] = j;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <search_string> <filename>\n";
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[2], "r");
    if (!f) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    const size_t MAXLEN = 8192;
    const char* success_ans = "true!\n";
    const char* failure_ans = "false =(\n";

    unsigned pi[MAXLEN];
    size_t m = strlen(argv[1]);
    prefix_function(argv[1], pi, m);

    unsigned p = 0;
    unsigned x = p;
    const char* answer = failure_ans;
    for (int c = fgetc(f);; c = fgetc(f)) {
        if (ferror(f)) {
            perror("fgetc failed");
            fclose(f);
            return EXIT_FAILURE;
        }
        if (feof(f)) {
            break;
        }
        x = p;
        while (x > 0 && c != argv[1][x]) {
            x = pi[x - 1];
        }
        x += (m != 0 && c == argv[1][x]);
        if (x == m) {
            answer = success_ans;
            break;
        }
        p = x;
    }

    size_t bytes_written = fwrite(answer, sizeof(char), strlen(answer), stdout);
    if (bytes_written == 0) {
        perror("fwrite failed");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);
    return EXIT_SUCCESS;
}
