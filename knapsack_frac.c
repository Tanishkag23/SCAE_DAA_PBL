
#include "../scae.h"

static double* g_ratio_arr;

static int cmp_by_ratio_desc(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    if (g_ratio_arr[ib] > g_ratio_arr[ia]) return 1;
    if (g_ratio_arr[ib] < g_ratio_arr[ia]) return -1;
    return 0;
}

AlgoResult knapsack_fractional(int* weight, int* value, int n, int capacity) {
    if (n <= 0 || capacity <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }


    double* ratio = (double*)malloc(n * sizeof(double));
    int* idx = (int*)malloc(n * sizeof(int));
    if (!ratio || !idx) {
        if (ratio) free(ratio);
        if (idx) free(idx);
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    for (int i = 0; i < n; i++) {
        ratio[i] = (double)value[i] / (double)weight[i];
        idx[i] = i;
    }


    g_ratio_arr = ratio;
    qsort(idx, n, sizeof(int), cmp_by_ratio_desc);

    AlgoResult r = create_result(2);
    r.steps = 0;
    strcpy(r.big_o, "O(n log n)");

    clock_t start_clk = clock();

    double total_value = 0.0;
    int remaining_cap = capacity;
    int full_items = 0;

    for (int i = 0; i < n; i++) {
        r.steps++;
        if (remaining_cap <= 0) break;

        int item = idx[i];

        if (weight[item] <= remaining_cap) {

            total_value += (double)value[item];
            remaining_cap -= weight[item];
            full_items++;
        } else {

            double fraction = (double)remaining_cap / (double)weight[item];
            total_value += (double)value[item] * fraction;
            remaining_cap = 0;
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;

    r.output[0] = (int)(total_value * 100.0 + 0.5);
    r.output[1] = full_items;
    r.outputSize = 2;
    r.memory_used = sizeof(AlgoResult) + n * (sizeof(double) + sizeof(int)) + 2 * sizeof(int);
    strcpy(r.status, "SUCCESS");

    free(ratio);
    free(idx);
    return r;
}
