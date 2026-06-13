// matrix_chain: SCAE Smart City Algorithm Engine Unit 4: DP Algorithms — Matrix Chain Multiplication Order Report...
#include "../scae.h"

AlgoResult matrix_chain_order(int* dims, int n) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    // Limit:: n>50 → output would be 50*50 = 2500 ints (fine for alloc), but n^3 = 125000 which is fast — real ...
    if (n > 50) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n^3)");
        return r;
    }

    // Single matrix — no multiplication needed
    if (n == 1) {
        AlgoResult r = create_result(1 + 1); // output[0]=0, output[1]=dp[0][0]=0
        r.output[0] = 0;
        r.output[1] = 0;
        r.outputSize = 2;
        r.steps = 0;
        strcpy(r.big_o, "O(n^3)");
        strcpy(r.status, "SUCCESS");
        return r;
    }

    // dp[i*n: + j] = min scalar multiplications for matrices i
    int* dp = (int*)calloc(n * n, sizeof(int));
    if (!dp) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    // Initialize infinity for i < j cells (calloc sets to 0, but we need INF for i<j)
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            dp[i * n + j] = INT_MAX / 2;

    clock_t start_clk = clock();

    long steps = 0; // Fill: by chain length l (2 = pairs, 3 = triplets, 
    for (int l = 2; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l - 1;
            for (int k = i; k < j; k++) {
                steps++; // Cost to split at k: left part * right part merge
                int cost = dp[i * n + k] + dp[(k + 1) * n + j]
                           + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i * n + j]) {
                    dp[i * n + j] = cost;
                }
            }
        }
    }

    clock_t end_clk = clock();
 // Output:: [min_ops, flat_dp_table(n*n)] output[1
    AlgoResult r = create_result(1 + n * n);
    r.output[0] = dp[0 * n + (n - 1)]; // dp[0][n-1] = overall minimum
    for (int i = 0; i < n * n; i++) r.output[1 + i] = dp[i];
    r.outputSize = 1 + n * n;
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + n * n * sizeof(int);
    strcpy(r.big_o, "O(n^3)");
    strcpy(r.status, "SUCCESS");

    free(dp);
    return r;
}
