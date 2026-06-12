
#include "../scae.h"

static int* g_finish_arr;

static int cmp_by_finish(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b;
    return g_finish_arr[ia] - g_finish_arr[ib];
}

AlgoResult activity_selection(int* start, int* finish, int n) {

    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    AlgoResult r = create_result(n);
    r.outputSize = 0;
    r.steps = 0;
    strcpy(r.big_o, "O(n log n)");

    int* idx = (int*)malloc(n * sizeof(int));
    if (!idx) {
        strcpy(r.status, "OOM");
        return r;
    }
    for (int i = 0; i < n; i++) idx[i] = i;

    g_finish_arr = finish;
    qsort(idx, n, sizeof(int), cmp_by_finish);

    clock_t start_clk = clock();

    int last_finish = -1;
    for (int i = 0; i < n; i++) {
        r.steps++;
        int job = idx[i];

        if (start[job] >= last_finish) {
            r.output[r.outputSize++] = job;
            last_finish = finish[job];
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + n * sizeof(int) * 2;
    strcpy(r.status, "SUCCESS");

    free(idx);
    return r;
}
