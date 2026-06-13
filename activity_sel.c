// activity_sel: SCAE Smart City Algorithm Engine Unit 4: Greedy Algorithms — Activity Selection City maintenance ...
#include "../scae.h"

// File-level: static pointer for qsort comparator qsort ko finish array ka access chahiye — static global se de...
static int* g_finish_arr;

static int cmp_by_finish(const void* a, const void* b) {
    int ia = *(int*)a;
    int ib = *(int*)b; // Ascending order by finish time — sabse pehle khatam wala aage
    return g_finish_arr[ia] - g_finish_arr[ib];
}

// activity_selection: Greedy: sort by finish time, pick non-overlapping jobs
AlgoResult activity_selection(int* start, int* finish, int n) {
    // Invalid input check
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    AlgoResult r = create_result(n);
    r.outputSize = 0;
    r.steps = 0;
    strcpy(r.big_o, "O(n log n)");
 // Index: array banao — original arrays chhedo mat! Sorting indices karte hain, actual start/finish arrays ...
    int* idx = (int*)malloc(n * sizeof(int));
    if (!idx) {
        strcpy(r.status, "OOM");
        return r;
    }
    for (int i = 0; i < n; i++) idx[i] = i;
 // qsort: ke liye global pointer set karo Exchange argument: agar earliest-finish job select na karo, toh h...
    g_finish_arr = finish;
    qsort(idx, n, sizeof(int), cmp_by_finish);

    clock_t start_clk = clock();
 // Greedy selection: last_finish = last selected job ki finish time
    int last_finish = -1;
    for (int i = 0; i < n; i++) {
        r.steps++; // Har job consider karna ek step hai
        int job = idx[i];
 // Non-overlapping: condition: current job start >= last selected job finish Agar start time last_finish se pehle hai...
        if (start[job] >= last_finish) {
            r.output[r.outputSize++] = job;
            last_finish = finish[job]; // Ab yeh job ki finish time track karo
        }
    }

    clock_t end_clk = clock();
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + n * sizeof(int) * 2;
    strcpy(r.status, "SUCCESS");

    free(idx);
    return r;
}
