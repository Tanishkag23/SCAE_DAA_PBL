
#include "../scae.h"

static int* g_profit_arr;

static int cmp_by_profit_desc(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    return g_profit_arr[ib] - g_profit_arr[ia];
}

AlgoResult job_sequencing(int* profit, int* deadline, int n) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }


    int max_d = 0;
    for (int i = 0; i < n; i++) {
        if (deadline[i] > max_d) max_d = deadline[i];
    }


    int* slots = (int*)malloc(max_d * sizeof(int));
    if (!slots) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }
    for (int i = 0; i < max_d; i++) slots[i] = -1;

    int* idx = (int*)malloc(n * sizeof(int));
    if (!idx) {
        free(slots);
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }
    for (int i = 0; i < n; i++) idx[i] = i;

    g_profit_arr = profit;
    qsort(idx, n, sizeof(int), cmp_by_profit_desc);

    AlgoResult r = create_result(n);
    r.steps = 0;
    strcpy(r.big_o, "O(n^2)");

    clock_t start_clk = clock();

    for (int i = 0; i < n; i++) {
        r.steps++;
        int job = idx[i];
        int d = deadline[job];

        int slot_limit = (d <= max_d ? d : max_d) - 1;
        for (int s = slot_limit; s >= 0; s--) {
            if (slots[s] == -1) {
                slots[s] = job;
                break;
            }
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;

    int out = 0;
    for (int s = 0; s < max_d; s++) {
        if (slots[s] != -1) {
            r.output[out++] = slots[s];
        }
    }
    r.outputSize = out;
    r.memory_used = sizeof(AlgoResult) + (n + max_d) * sizeof(int);
    strcpy(r.status, "SUCCESS");

    free(slots);
    free(idx);
    return r;
}
