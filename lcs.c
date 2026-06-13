// lcs: SCAE Smart City Algorithm Engine Unit 4: DP Algorithms — Longest Common Subsequence Complaint pat...
#include "../scae.h"

// lcs: Standard DP — O(m*n) time, O(m*n) space
AlgoResult lcs(int* seq1, int len1, int* seq2, int len2) {
    if (len1 <= 0 || len2 <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    // Limit: too large would OOM
    if ((long long)len1 * len2 > 5000000L) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(mn)");
        return r;
    }

    // DP: table: dp[i*(len2+1) + j] = LCS length of seq1[0
    int* dp = (int*)calloc((long)(len1 + 1) * (len2 + 1), sizeof(int));
    if (!dp) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    clock_t start_clk = clock();

    long steps = 0;
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            steps++;
            if (seq1[i - 1] == seq2[j - 1]) {
                // Match! Diagonal se aao — dono sequences mein same element
                dp[i * (len2 + 1) + j] = dp[(i - 1) * (len2 + 1) + (j - 1)] + 1;
            } else {
                // No match — upar ya left se best lo
                int from_top  = dp[(i - 1) * (len2 + 1) + j];
                int from_left = dp[i * (len2 + 1) + (j - 1)];
                dp[i * (len2 + 1) + j] = from_top > from_left ? from_top : from_left;
            }
        }
    }

    clock_t end_clk = clock();
    int lcs_len = dp[len1 * (len2 + 1) + len2];
 // Backtrack:: LCS ko reconstruct karo Diagonal = match → include in LCS Up/Left = skip → whichever direction ha...
    int* lcs_seq = (int*)malloc((lcs_len + 1) * sizeof(int));
    int bi = len1, bj = len2, bk = lcs_len - 1;
    while (bi > 0 && bj > 0) {
        if (seq1[bi - 1] == seq2[bj - 1]) {
            lcs_seq[bk--] = seq1[bi - 1];
            bi--; bj--;
        } else if (dp[(bi - 1) * (len2 + 1) + bj] > dp[bi * (len2 + 1) + (bj - 1)]) {
            bi--;
        } else {
            bj--;
        }
    }

    // Build output: output[0] = lcs_len, output[1..] = LCS elements
    AlgoResult r = create_result(lcs_len + 1);
    r.output[0] = lcs_len;
    for (int i = 0; i < lcs_len; i++) r.output[i + 1] = lcs_seq[i];
    r.outputSize = lcs_len + 1;
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (long)(len1 + 1) * (len2 + 1) * sizeof(int);
    strcpy(r.big_o, "O(mn)");
    strcpy(r.status, "SUCCESS");

    free(dp);
    free(lcs_seq);
    return r;
}
