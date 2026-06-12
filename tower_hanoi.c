
#include "../scae.h"

static void hanoi_helper(int n, int src, int dst, int aux,
                          int* output, int* move_idx, long* steps) {

    if (n <= 0) return;

    hanoi_helper(n - 1, src, aux, dst, output, move_idx, steps);

    output[*move_idx]     = src;
    output[*move_idx + 1] = dst;
    *move_idx += 2;
    (*steps)++;


    hanoi_helper(n - 1, aux, dst, src, output, move_idx, steps);
}

AlgoResult tower_hanoi(int n) {


    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }


    if (n > 20) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }


    int n_moves = (1 << n) - 1;

    AlgoResult r = create_result(n_moves * 2);
    r.outputSize = n_moves * 2;
    strcpy(r.big_o, "O(2^n)");

    clock_t start = clock();

    int move_idx = 0;

    hanoi_helper(n, 0, 2, 1, r.output, &move_idx, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    r.memory_used = (long)(sizeof(AlgoResult) + (size_t)n_moves * 2 * sizeof(int));

    strcpy(r.status, "SUCCESS");
    return r;
}
