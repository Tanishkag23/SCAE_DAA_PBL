
#include "../scae.h"

AlgoResult naive_string_match(int* text, int text_len,
                               int* pattern, int pat_len) {
    if (text_len <= 0 || pat_len <= 0 || pat_len > text_len) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }


    AlgoResult r = create_result(text_len - pat_len + 1);
    r.outputSize = 0;
    r.steps = 0;
    strcpy(r.big_o, "O(nm)");

    clock_t start_clk = clock();

    for (int i = 0; i <= text_len - pat_len; i++) {
        int match = 1;
        for (int j = 0; j < pat_len; j++) {
            r.steps++;
            if (text[i + j] != pattern[j]) {
                match = 0;
                break;
            }
        }
        if (match) {
            r.output[r.outputSize++] = i;
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (text_len - pat_len + 1) * sizeof(int);

    if (r.outputSize > 0) {
        strcpy(r.status, "FOUND");
    } else {
        strcpy(r.status, "NOT_FOUND");
    }

    return r;
}
