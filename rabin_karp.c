// rabin_karp: SCAE Smart City Algorithm Engine Unit 5: String Matching — Rabin-Karp Rolling Hash Pattern detect...
#include "../scae.h"

// Rolling: hash parameters BASE = 31 (prime, commonly used for lowercase chars/small ints) MOD  = 1000000007...
#define RK_BASE 31LL
#define RK_MOD  1000000007LL

// rabin_karp_search: Sliding window polynomial rolling hash — O(n+m) average
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
 // Precompute: BASE^(pat_len-1) mod MOD — for removing leftmost character h_pow = BASE^(m-1): leftmost character...
    long long h_pow = 1;
    for (int i = 0; i < pat_len - 1; i++) {
        h_pow = (h_pow * RK_BASE) % RK_MOD;
    }

    // Compute: pattern hash and first window hash hash = text[0]*BASE^(m-1) + text[1]*BASE^(m-2) + 
    long long pat_hash = 0, win_hash = 0;
    for (int i = 0; i < pat_len; i++) {
        pat_hash = (pat_hash * RK_BASE + (long long)pattern[i]) % RK_MOD;
        win_hash = (win_hash * RK_BASE + (long long)text[i])    % RK_MOD;
    }

    clock_t start_clk = clock();

    for (int i = 0; i <= text_len - pat_len; i++) {
        if (win_hash == pat_hash) {
            // Hash: match — but spurious hit ho sakta hai! Character-by-character verify karo (O(m) worst case per wi...
            int match = 1;
            for (int j = 0; j < pat_len; j++) {
                r.steps++; // Character comparison = step
                if (text[i + j] != pattern[j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                r.output[r.outputSize++] = i;
            }
        }

        // Slide: window to next position — yahi rolling hash ka magic hai! new_hash = (old_hash - leftmost*h_pow) ...
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
