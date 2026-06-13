// kmp: SCAE Smart City Algorithm Engine Unit 5: String Matching — Knuth-Morris-Pratt Algorithm Fast comp...
#include "../scae.h"

// build_failure: Partial Match Table (KMP failure function) fail[i] = length of longest proper prefix of pattern[0
static void build_failure(int* pattern, int m, int* fail) {
    fail[0] = 0; // First character ka prefix/suffix = 0 (always)
    int k = 0;   // Length of previous longest prefix suffix

    for (int i = 1; i < m; i++) {
        // Jab tak mismatch aur k>0, peeche jao failure chain mein
        while (k > 0 && pattern[k] != pattern[i]) {
            k = fail[k - 1]; // Fall back!
        }
        if (pattern[k] == pattern[i]) {
            k++; // Match! prefix length badhao
        }
        fail[i] = k;
    }
}

// kmp_search: KMP string search — O(n+m) guaranteed
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
 // Build failure function — O(m) phase
    int* fail = (int*)calloc(pat_len, sizeof(int));
    if (!fail) {
        strcpy(r.status, "OOM");
        return r;
    }
    build_failure(pattern, pat_len, fail);

    clock_t start_clk = clock();

    int q = 0; // Number of characters matched so far in pattern
    for (int i = 0; i < text_len; i++) {
        r.steps++; // Text[i] compare kar rahe hain

        // Mismatch!: Failure function use karo — text[i] wapas nahi dekhna Yahi KMP ka magic hai: text pointer kabhi p...
        while (q > 0 && pattern[q] != text[i]) {
            q = fail[q - 1];
        }

        if (pattern[q] == text[i]) {
            q++; // Ek aur character match hua
        }

        if (q == pat_len) {
            // Full match! Position = i - pat_len + 1
            r.output[r.outputSize++] = i - pat_len + 1;
            q = fail[q - 1]; // Next potential match ke liye reset
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
