
#include "../scae.h"

AlgoResult matrix_chain_order(int* dims, int n) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    if (n > 50) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n^3)");
        return r;
    }


    if (n == 1) {
        AlgoResult r = create_result(1 + 1);
        r.output[0] = 0;
        r.output[1] = 0;
        r.outputSize = 2;
        r.steps = 0;
        strcpy(r.big_o, "O(n^3)");
        strcpy(r.status, "SUCCESS");
        return r;
    }


    int* dp = (int*)calloc(n * n, sizeof(int));
    if (!dp) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }


    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            dp[i * n + j] = INT_MAX / 2;

    clock_t start_clk = clock();

    long steps = 0;
    for (int l = 2; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l - 1;
            for (int k = i; k < j; k++) {
                steps++;
                int cost = dp[i * n + k] + dp[(k + 1) * n + j]
                           + dims[i] * dims[k + 1] * dims[j + 1];
                if (cost < dp[i * n + j]) {
                    dp[i * n + j] = cost;
                }
            }
        }
    }

    clock_t end_clk = clock();

    AlgoResult r = create_result(1 + n * n);
    r.output[0] = dp[0 * n + (n - 1)];
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
