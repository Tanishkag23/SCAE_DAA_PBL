// fibonacci: SCAE Smart City Algorithm Engine Fibonacci: sensor cascade prediction model Fib(n) = sensors that...
#include "../scae.h"


// fib_naive_helper: Pure recursive Fibonacci — koi caching nahi, koi shortcuts nahi
static long fib_naive_helper(int n, long* steps) {
    // Har: function call ek step hai — chahe base case ho ya recursive Yahi steps count naive ko itna slow d...
    (*steps)++;
 // Base cases: fib(0) = 0, fib(1) = 1 — math ki neenv
    if (n <= 0) return 0;
    if (n == 1) return 1;
 // Recursive: case — DO calls karo, dono subproblems solve karo Yahan hi saara drama hota hai — overlapping sub...
    return fib_naive_helper(n - 1, steps) + fib_naive_helper(n - 2, steps);
}


// fibonacci_naive: Pure recursive Fibonacci — slow, beautiful, brute force
AlgoResult fibonacci_naive(int n) {

    // Edge case: n > 40 — bahut slow hoga, abort!
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
 // Helper call — steps pointer pass karo taaki call tree count ho
    long result = fib_naive_helper(n, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
 // Fibonacci: numbers int range mein hote hain upto fib(46) ≈ 1
    r.output[0] = (int)result;
    strcpy(r.status, "SUCCESS");

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
    return r;
}


// fib_memo_helper: Memoized Fibonacci — "pehle dekho memo mein, mila toh wapas do, nahi mila toh compute karo aur me...
static long fib_memo_helper(int n, int* memo, long* steps) {
    // Base cases — directly return, memo nahi chahiye inhe
    if (n <= 0) return 0;
    if (n == 1) return 1;
 // CACHE: HIT: memo[n] already computed hai — seedha return karo! Koi step count nahi — yeh free hai! Cache...
    if (memo[n] != -1) {
        return (long)memo[n];
    }

    // CACHE: MISS: pehli baar is n ki value chahiye — compute karo Sirf yahan steps count karo — actual kaam y...
    (*steps)++;
 // Recursive calls — ye bhi memo use karenge agar already computed
    long result = fib_memo_helper(n - 1, memo, steps)
                + fib_memo_helper(n - 2, memo, steps);
 // Result save karo memo mein — agla call seedha yahan se lega NOTE: fib(80) > INT_MAX, isliye n <= 80 limit zaruri hai! Yahan cast safe hai kyunki limit ke andar hain hum
    memo[n] = (int)result;

    return result;
}


// fibonacci_memo: Memoized Fibonacci — fast, efficient, production-ready! Same answers as naive, but O(n) instead o...
AlgoResult fibonacci_memo(int n) {

    // Edge case: n > 80 — limit se bahar
    if (n > 80) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n)");
        return r;
    }

    AlgoResult r = create_result(1);
    strcpy(r.big_o, "O(n)");
    r.outputSize = 1;
 // n=0 aur n=1 ke liye memo array ki zarurat nahi — direct handle
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

    // Memo: array heap pe allocate karo — size n+1 (indices 0 to n) VLA nahi use kar rahe kyunki: 1
    int* memo = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (memo == NULL) {
        // Malloc fail — OOM situation
        strcpy(r.status, "NOT_FOUND");  // Error indicator
        return r;
    }

    // Sabko: 1 se initialize karo — -1 matlab "abhi compute nahi hua" Fibonacci values kabhi negative nahi hot...
    for (int i = 0; i <= n; i++) {
        memo[i] = -1;
    }

    clock_t start = clock();

    long result = fib_memo_helper(n, memo, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    r.output[0] = (int)result;  // Safe for n <= 46; n <= 80 pe overflow possible
    strcpy(r.status, "SUCCESS");

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int)          // output array
                         + (size_t)(n + 1) * sizeof(int));           // memo array
    // Note: memo free karo! Local function ke andar tha, ab kaam nahi
    free(memo);
    memo = NULL;

    return r;
}
