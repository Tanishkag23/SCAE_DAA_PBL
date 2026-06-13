// knapsack_frac: SCAE Smart City Algorithm Engine Unit 4: Greedy Algorithms — Fractional Knapsack Emergency truck ...
#include "../scae.h"

// File-level: statics for ratio-based qsort comparator value/weight ratio ka comparison — sabse zyada ratio wal...
static double* g_ratio_arr;

static int cmp_by_ratio_desc(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b; // Higher ratio = better value per kg = pehle lo!
    if (g_ratio_arr[ib] > g_ratio_arr[ia]) return 1;
    if (g_ratio_arr[ib] < g_ratio_arr[ia]) return -1;
    return 0;
}

// knapsack_fractional: Greedy: sort by value/weight ratio, take greedily
AlgoResult knapsack_fractional(int* weight, int* value, int n, int capacity) {
    if (n <= 0 || capacity <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    // Ratio: array calculate karo: value per unit weight "Rupees per kg" — jis cheez ki value per kg sabse zya...
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

    // Sort indices by ratio descending
    g_ratio_arr = ratio;
    qsort(idx, n, sizeof(int), cmp_by_ratio_desc);

    AlgoResult r = create_result(2); // output[0]=total_value*100, output[1]=full_items
    r.steps = 0;
    strcpy(r.big_o, "O(n log n)");

    clock_t start_clk = clock();

    double total_value = 0.0;
    int remaining_cap = capacity;
    int full_items = 0;

    for (int i = 0; i < n; i++) {
        r.steps++; // Har item consider karna ek step
        if (remaining_cap <= 0) break;

        int item = idx[i];

        if (weight[item] <= remaining_cap) {
            // Pura item le lo — weight fits!
            total_value += (double)value[item];
            remaining_cap -= weight[item];
            full_items++;
        } else {
            // Fractional: item lo — sirf kitna capacity hai utna! Yahan tukda bhi le sakte hain — 0/1 knapsack mein yeh nah...
            double fraction = (double)remaining_cap / (double)weight[item];
            total_value += (double)value[item] * fraction;
            remaining_cap = 0; // Don't increment full_items — ye poora nahi tha
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
 // *100: trick: double ko int mein convert — Python divide by 100 karega e
    r.output[0] = (int)(total_value * 100.0 + 0.5); // +0.5 for rounding
    r.output[1] = full_items;
    r.outputSize = 2;
    r.memory_used = sizeof(AlgoResult) + n * (sizeof(double) + sizeof(int)) + 2 * sizeof(int);
    strcpy(r.status, "SUCCESS");

    free(ratio);
    free(idx);
    return r;
}
