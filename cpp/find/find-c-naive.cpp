#include <cstdlib>
#include <iostream>
#include <iterator>
#include <cstring>

bool contains(const char* s, const size_t m, const char* t, const size_t n) {
    for (size_t i = 0; i + m <= n; ++i) {
        if (strncmp(s, t + i, m) == 0) {
            return true;
        }
    }
    return false;
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

    char buffer[2 * MAXLEN];
    size_t n = fread(buffer, sizeof(char), MAXLEN, f) / sizeof(char);
    size_t m = strlen(argv[1]);

    const char* answer = failure_ans;
    for (;;) {
        size_t bytes_read = fread(buffer + MAXLEN, sizeof(char), MAXLEN, f);
        n += bytes_read / sizeof(char);
        if (contains(argv[1], m, buffer, n)) {
            answer = success_ans;
            break;
        }

        if (bytes_read == 0) {
            if (ferror(f)) {
                perror("fread failed");
                fclose(f);
                return EXIT_FAILURE;
            }
            break;
        }

        memcpy(buffer, buffer + MAXLEN, sizeof(char) * MAXLEN);
        n -= MAXLEN;
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
