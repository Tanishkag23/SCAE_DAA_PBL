
#include "../scae.h"

static long fib_naive_helper(int n, long* steps) {

    (*steps)++;

    if (n <= 0) return 0;
    if (n == 1) return 1;

    return fib_naive_helper(n - 1, steps) + fib_naive_helper(n - 2, steps);
}

AlgoResult fibonacci_naive(int n) {


    if (n > 40) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(2^n)");
        return r;
    }

    AlgoResult r = create_result(1);
    strcpy(r.big_o, "O(2^n)");
    r.outputSize = 1;

    clock_t start = clock();

    long result = fib_naive_helper(n, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    r.output[0] = (int)result;
    strcpy(r.status, "SUCCESS");

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
    return r;
}

static long fib_memo_helper(int n, int* memo, long* steps) {

    if (n <= 0) return 0;
    if (n == 1) return 1;

    if (memo[n] != -1) {
        return (long)memo[n];
    }


    (*steps)++;

    long result = fib_memo_helper(n - 1, memo, steps)
                + fib_memo_helper(n - 2, memo, steps);

    memo[n] = (int)result;

    return result;
}

AlgoResult fibonacci_memo(int n) {


    if (n > 80) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n)");
        return r;
    }

    AlgoResult r = create_result(1);
    strcpy(r.big_o, "O(n)");
    r.outputSize = 1;

    if (n <= 0) {
        r.output[0] = 0;
        strcpy(r.status, "SUCCESS");
        r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
        return r;
    }
    if (n == 1) {
        r.output[0] = 1;
        strcpy(r.status, "SUCCESS");
        r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
        return r;
    }


    int* memo = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (memo == NULL) {

        strcpy(r.status, "NOT_FOUND");
        return r;
    }


    for (int i = 0; i <= n; i++) {
        memo[i] = -1;
    }

    clock_t start = clock();

    long result = fib_memo_helper(n, memo, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    r.output[0] = (int)result;
    strcpy(r.status, "SUCCESS");

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int)
                         + (size_t)(n + 1) * sizeof(int));

    free(memo);
    memo = NULL;

    return r;
}
