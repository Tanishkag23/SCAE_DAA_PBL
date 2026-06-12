
#include "../scae.h"

static int g_bt_selected[25];
static int g_bt_count = 0;

static int subset_backtrack(int* arr, int n, int target,
                             int idx, int current, long* steps) {
    (*steps)++;

    if (current == target) {
        return 1;
    }
    if (current > target || idx >= n) {
        return 0;
    }


    int saved_count = g_bt_count;
    g_bt_selected[g_bt_count++] = idx;
    if (subset_backtrack(arr, n, target, idx + 1, current + arr[idx], steps)) {
        return 1;
    }
    g_bt_count = saved_count;


    if (subset_backtrack(arr, n, target, idx + 1, current, steps)) {
        return 1;
    }

    return 0;
}

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

        strcpy(big_o, "O(nW)");

        int M = target + 1;
        int* dp = (int*)calloc((long)(n + 1) * M, sizeof(int));
        if (!dp) {
            AlgoResult r = create_result(0);
            strcpy(r.status, "OOM");
            return r;
        }


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

            sel_indices = (int*)malloc(n * sizeof(int));
            int w = target;
            for (int i = n; i >= 1 && w > 0; i--) {

                if (!dp[(long)(i - 1) * M + w]) {
                    sel_indices[sel_count++] = i - 1;
                    w -= arr[i - 1];
                }
            }

            for (int i = 0; i < sel_count / 2; i++) {
                int tmp = sel_indices[i];
                sel_indices[i] = sel_indices[sel_count - 1 - i];
                sel_indices[sel_count - 1 - i] = tmp;
            }
        }
        free(dp);

    } else if (n <= 20) {

        strcpy(big_o, "O(2^n)");

        g_bt_count = 0;
        possible = subset_backtrack(arr, n, target, 0, 0, &steps);

        if (possible) {
            sel_count   = g_bt_count;
            sel_indices = (int*)malloc(sel_count * sizeof(int));
            for (int i = 0; i < sel_count; i++) sel_indices[i] = g_bt_selected[i];
        }

    } else {

        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(nW)");
        return r;
    }

    clock_t end_clk = clock();

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
