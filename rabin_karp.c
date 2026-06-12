
#include "../scae.h"

#define RK_BASE 31LL
#define RK_MOD  1000000007LL

AlgoResult rabin_karp_search(int* text, int text_len,
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

    long long h_pow = 1;
    for (int i = 0; i < pat_len - 1; i++) {
        h_pow = (h_pow * RK_BASE) % RK_MOD;
    }


    long long pat_hash = 0, win_hash = 0;
    for (int i = 0; i < pat_len; i++) {
        pat_hash = (pat_hash * RK_BASE + (long long)pattern[i]) % RK_MOD;
        win_hash = (win_hash * RK_BASE + (long long)text[i])    % RK_MOD;
    }

    clock_t start_clk = clock();

    for (int i = 0; i <= text_len - pat_len; i++) {
        if (win_hash == pat_hash) {

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


        if (i < text_len - pat_len) {
            win_hash = (win_hash - (long long)text[i] * h_pow % RK_MOD + RK_MOD) % RK_MOD;
            win_hash = (win_hash * RK_BASE + (long long)text[i + pat_len]) % RK_MOD;
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
