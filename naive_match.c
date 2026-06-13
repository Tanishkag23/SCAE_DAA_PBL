// naive_match: SCAE Smart City Algorithm Engine Unit 5: String Matching — Naive Algorithm (Baseline) Complaint t...
#include "../scae.h"

// naive_string_match: Try every position — compare pattern at each location
AlgoResult naive_string_match(int* text, int text_len,
                               int* pattern, int pat_len) {
    if (text_len <= 0 || pat_len <= 0 || pat_len > text_len) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    // Maximum matches possible = text_len - pat_len + 1
    AlgoResult r = create_result(text_len - pat_len + 1);
    r.outputSize = 0;
    r.steps = 0;
    strcpy(r.big_o, "O(nm)");

    clock_t start_clk = clock();

    for (int i = 0; i <= text_len - pat_len; i++) {
        int match = 1;
        for (int j = 0; j < pat_len; j++) {
            r.steps++; // Har character comparison ek step hai
            if (text[i + j] != pattern[j]) {
                match = 0;
                break; // Mismatch! Agle position pe jao
            }
        }
        if (match) {
            r.output[r.outputSize++] = i; // Match mila at position i
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
