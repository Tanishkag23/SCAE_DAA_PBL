// subset_sum: SCAE Smart City Algorithm Engine Unit 5 NP: Subset Sum — NP-Complete Demo Budget feasibility chec...
#include "../scae.h"

// Static: globals for backtracking path — thread-safe nahi but SCAE single-threaded hai toh theek hai
static int g_bt_selected[25]; // max n=20 + buffer
static int g_bt_count = 0;
 // subset_backtrack: recursive include/exclude Returns 1 if subset found, 0 otherwise g_bt_selected[] stores indices o...
static int subset_backtrack(int* arr, int n, int target,
                             int idx, int current, long* steps) {
    (*steps)++;

    if (current == target) {
        return 1; // Subset mila!
    }
    if (current > target || idx >= n) {
        return 0; // Overshoot ya elements khatam
    }

    // INCLUDE arr[idx]: current sum mein add karo
    int saved_count = g_bt_count;
    g_bt_selected[g_bt_count++] = idx;
    if (subset_backtrack(arr, n, target, idx + 1, current + arr[idx], steps)) {
        return 1;
    }
    g_bt_count = saved_count; // Backtrack — undo include

    // EXCLUDE arr[idx]: bina liye agle element pe jao
    if (subset_backtrack(arr, n, target, idx + 1, current, steps)) {
        return 1;
    }

    return 0;
}

//   subset_sum — smart dispatch: DP or backtracking
AlgoResult subset_sum(int* arr, int n, int target) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    if (target < 0) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        strcpy(r.status, "NO_SUBSET");
        return r;
    }
    // target=0 is trivially possible with empty subset
    if (target == 0) {
        AlgoResult r = create_result(1);
        r.output[0] = 1;
        r.outputSize = 1;
        strcpy(r.big_o, "O(nW)");
        strcpy(r.status, "SUBSET_FOUND");
        return r;
    }

    clock_t start_clk = clock();
    long steps = 0;
    int* sel_indices = NULL;
    int  sel_count   = 0;
    int  possible    = 0;
    char big_o[20];

    long dp_size = (long)n * target;

    if (dp_size <= 1000000L) {
        // =====: DP APPROACH: O(n * target) ===== Pseudo-polynomial — depends on target VALUE not just n
        strcpy(big_o, "O(nW)");

        int M = target + 1;
        int* dp = (int*)calloc((long)(n + 1) * M, sizeof(int));
        if (!dp) {
            AlgoResult r = create_result(0);
            strcpy(r.status, "OOM");
            return r;
        }

        // dp[0][0] = 1: empty subset se sum 0 banaya ja sakta hai
        dp[0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int w = target; w >= 0; w--) {
                steps++;
                dp[(long)i * M + w] = dp[(long)(i - 1) * M + w];
                if (w >= arr[i - 1] && dp[(long)(i - 1) * M + w - arr[i - 1]]) {
                    dp[(long)i * M + w] = 1;
                }
            }
        }

        possible = dp[(long)n * M + target];

        if (possible) {
            // Backtrack to find WHICH elements were selected
            sel_indices = (int*)malloc(n * sizeof(int));
            int w = target;
            for (int i = n; i >= 1 && w > 0; i--) {
                // Agar dp[i-1][w] = 0 but dp[i][w] = 1: arr[i-1] zaroor include hua
                if (!dp[(long)(i - 1) * M + w]) {
                    sel_indices[sel_count++] = i - 1; // 0-based index
                    w -= arr[i - 1];
                }
            }
            // Reverse for ascending order
            for (int i = 0; i < sel_count / 2; i++) {
                int tmp = sel_indices[i];
                sel_indices[i] = sel_indices[sel_count - 1 - i];
                sel_indices[sel_count - 1 - i] = tmp;
            }
        }
        free(dp);

    } else if (n <= 20) {
        // =====: BACKTRACKING APPROACH: O(2^n) ===== Exponential but only when DP infeasible (large target, small n)
        strcpy(big_o, "O(2^n)");

        g_bt_count = 0; // Reset globals
        possible = subset_backtrack(arr, n, target, 0, 0, &steps);

        if (possible) {
            sel_count   = g_bt_count;
            sel_indices = (int*)malloc(sel_count * sizeof(int));
            for (int i = 0; i < sel_count; i++) sel_indices[i] = g_bt_selected[i];
        }

    } else {
        // ===== TOO_LARGE: neither approach feasible =====
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(nW)");
        return r;
    }

    clock_t end_clk = clock();
 // Build output: [possible_flag, selected_idx_0, selected_idx_1, ...]
    AlgoResult r = create_result(1 + (possible ? sel_count : 0));
    r.output[0] = possible ? 1 : 0;
    for (int i = 0; i < sel_count; i++) r.output[1 + i] = sel_indices[i];
    r.outputSize = 1 + (possible ? sel_count : 0);
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (long)n * target * sizeof(int);
    strcpy(r.big_o, big_o);
    strcpy(r.status, possible ? "SUBSET_FOUND" : "NO_SUBSET");

    if (sel_indices) free(sel_indices);
    return r;
}
