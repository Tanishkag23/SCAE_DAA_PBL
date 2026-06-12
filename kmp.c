
#include "../scae.h"

static void build_failure(int* pattern, int m, int* fail) {
    fail[0] = 0;
    int k = 0;

    for (int i = 1; i < m; i++) {

        while (k > 0 && pattern[k] != pattern[i]) {
            k = fail[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            k++;
        }
        fail[i] = k;
    }
}

AlgoResult kmp_search(int* text, int text_len,
                      int* pattern, int pat_len) {
    if (text_len <= 0 || pat_len <= 0 || pat_len > text_len) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    AlgoResult r = create_result(text_len - pat_len + 1);
    r.outputSize = 0;
    r.steps = 0;
    strcpy(r.big_o, "O(n+m)");

    int* fail = (int*)calloc(pat_len, sizeof(int));
    if (!fail) {
        strcpy(r.status, "OOM");
        return r;
    }
    build_failure(pattern, pat_len, fail);

    clock_t start_clk = clock();

    int q = 0;
    for (int i = 0; i < text_len; i++) {
        r.steps++;


        while (q > 0 && pattern[q] != text[i]) {
            q = fail[q - 1];
        }

        if (pattern[q] == text[i]) {
            q++;
        }

        if (q == pat_len) {

            r.output[r.outputSize++] = i - pat_len + 1;
            q = fail[q - 1];
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (text_len - pat_len + 1 + pat_len) * sizeof(int);

    if (r.outputSize > 0) {
        strcpy(r.status, "FOUND");
    } else {
        strcpy(r.status, "NOT_FOUND");
    }

    free(fail);
    return r;
}
