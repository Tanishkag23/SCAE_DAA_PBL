// knapsack_01: SCAE Smart City Algorithm Engine Unit 4: DP Algorithms — 0/1 Knapsack Budget allocation — limited...
#include "../scae.h"

AlgoResult knapsack_01(int* weight, int* value, int n, int capacity) {
    if (n <= 0 || capacity <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    // Limit check: n*capacity > 10M = too much memory / time
    if ((long long)n * capacity > 10000000L) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(nW)");
        return r;
    }

    // 2D: DP table: dp[i][w] = max value using first i items, capacity w Flat 1D: dp[i*(capacity+1) + w] W ...
    int* dp = (int*)calloc((long)(n + 1) * (capacity + 1), sizeof(int));
    if (!dp) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    clock_t start_clk = clock();

    long steps = 0;
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            steps++; // Default: don't take item i
            dp[i * (capacity + 1) + w] = dp[(i - 1) * (capacity + 1) + w]; // Agar item fit hota hai, check karo lena better hai ya nahi
            if (weight[i - 1] <= w) {
                int take_val = dp[(i - 1) * (capacity + 1) + (w - weight[i - 1])] + value[i - 1];
                if (take_val > dp[i * (capacity + 1) + w]) {
                    dp[i * (capacity + 1) + w] = take_val;
                }
            }
        }
    }

    clock_t end_clk = clock();
    int max_val = dp[n * (capacity + 1) + capacity];
 // Backtracking:: DP table se trace karo kaunse items select hue Agar dp[i][w] != dp[i-1][w] toh item i select hua tha
    int* selected = (int*)malloc(n * sizeof(int));
    int k = 0;
    int w = capacity;
    for (int i = n; i >= 1; i--) {
        if (dp[i * (capacity + 1) + w] != dp[(i - 1) * (capacity + 1) + w]) {
            selected[k++] = i - 1; // 0-based index
            w -= weight[i - 1];
        }
    }
    // selected array is in reverse order — reverse karo
    for (int i = 0; i < k / 2; i++) {
        int tmp = selected[i]; selected[i] = selected[k - 1 - i]; selected[k - 1 - i] = tmp;
    }

    // Build output: [max_val, count, item0, item1, ...]
    AlgoResult r = create_result(2 + k);
    r.output[0] = max_val;
    r.output[1] = k;
    for (int i = 0; i < k; i++) r.output[2 + i] = selected[i];
    r.outputSize = 2 + k;
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (long)(n + 1) * (capacity + 1) * sizeof(int);
    strcpy(r.big_o, "O(nW)");
    strcpy(r.status, "SUCCESS");

    free(dp);
    free(selected);
    return r;
}
