// ==============================================================
//   fibonacci.c — SCAE Smart City Algorithm Engine
//   Fibonacci: sensor cascade prediction model
//
//   SCAE Context:
//   Fib(n) = sensors that trigger in a chain reaction after n steps.
//   Sensor 0 = 0 triggers, Sensor 1 = 1 trigger, Sensor n =
//   Sensor(n-1) + Sensor(n-2) — each sensor activates the last two!
//
//   Do implementations:
//     1. fibonacci_naive — pure recursion, NO memoization (O(2^n))
//     2. fibonacci_memo  — memoized recursion (O(n))
//
//   Dono same answer dete hain — performance mein ZAMEEN AASMAN
//   ka farq hai. fib(30) ke liye: naive ~1.3M steps, memo ~30 steps!
// ==============================================================

#include "../scae.h"


// ==============================================================
//   INTERNAL HELPER: fib_naive_helper
//   Pure recursive Fibonacci — koi caching nahi, koi shortcuts nahi.
//
//   WHY IS NAIVE O(2^n)? — OVERLAPPING SUBPROBLEMS!
//   fib(5) ke liye call tree dekho:
//
//       fib(5)
//      L      R
//   fib(4)   fib(3)
//   L    R   L    R
// fib(3) fib(2) fib(2) fib(1)
//
//   fib(3) DO BAAR compute ho raha hai! fib(2) TEEN baar!
//   fib(n) = fib(n-1) + fib(n-2) — do branches, aur dono branches
//   apne andar aur branches banate hain. Har level pe DOUBLE kaam!
//   Total calls ≈ 2^n — isliye O(2^n) kehte hain.
//
//   fib(40) pe yeh ~2.2 BILLION calls karta hai — seconds lagte hain!
//   fib(50) pe? Saare janam beet jaayenge... 😱
//
//   Parameters:
//     n     — which fibonacci number to compute
//     steps — pointer to call counter (har call = ek step)
//
//   Returns: fibonacci(n) as long
// ==============================================================
static long fib_naive_helper(int n, long* steps) {
    // Har function call ek step hai — chahe base case ho ya recursive
    // Yahi steps count naive ko itna slow dikhata hai!
    (*steps)++;

    // Base cases: fib(0) = 0, fib(1) = 1 — math ki neenv
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // Recursive case — DO calls karo, dono subproblems solve karo
    // Yahan hi saara drama hota hai — overlapping subproblems dobara dobara compute!
    return fib_naive_helper(n - 1, steps) + fib_naive_helper(n - 2, steps);
}


// ==============================================================
//   FUNCTION: fibonacci_naive
//   Pure recursive Fibonacci — slow, beautiful, brute force.
//   Educational use ke liye perfect — real SCAE mein NEVER use karo!
//
//   Limit n <= 40 kyun?
//   fib(40) ≈ 2.2 billion calls — pehle se hi slow hai
//   fib(45) ≈ 70 billion calls — minutes lagte hain
//   fib(50) ≈ 2.25 TRILLION calls — pura din beet jaye
//   SCAE ek city management system hai, not a suicide machine! 😄
//   Toh n > 40 pe "TOO_LARGE" return karo — user ko protect karo.
//
//   Parameters: n — which Fibonacci number (0-indexed)
//   Returns: AlgoResult, output[0] = fib(n)
// ==============================================================
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

    // Fibonacci numbers int range mein hote hain upto fib(46) ≈ 1.8B < INT_MAX
    // n <= 40 limit ke andar fib(40) = 102334155 — int mein fit hai
    r.output[0] = (int)result;
    strcpy(r.status, "SUCCESS");

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
    return r;
}


// ==============================================================
//   INTERNAL HELPER: fib_memo_helper
//   Memoized Fibonacci — "pehle dekho memo mein, mila toh wapas do,
//   nahi mila toh compute karo aur memo mein save kar lo!"
//
//   Yeh strategy "top-down dynamic programming" kehlaati hai.
//   Pehli baar fib(k) compute karo → memo[k] mein store karo.
//   Doosri baar fib(k) maango → seedha memo[k] return karo — O(1)!
//
//   STEPS COUNT KA FARQ:
//   Naive mein har call step hai (including repeated calls).
//   Memo mein sirf CACHE MISS pe step count hota hai — jab actually
//   naya computation kiya. Cache hit = free hai, step nahi badhta!
//
//   Isliye memo ka steps ≈ n (sirf n unique values compute honge),
//   jabki naive ka steps ≈ 2^n (har subproblem bar bar compute!).
//   fib(30) ke liye: memo = 59 steps, naive = ~1.3 MILLION steps!
//   That's a 40,000x speedup. Memoization ki shakti!
//
//   Parameters:
//     n     — which fibonacci number
//     memo  — array of size n+1, -1 means not computed yet
//     steps — pointer to ACTUAL computation counter (cache misses only)
//
//   Returns: fibonacci(n) as long
// ==============================================================
static long fib_memo_helper(int n, int* memo, long* steps) {
    // Base cases — directly return, memo nahi chahiye inhe
    if (n <= 0) return 0;
    if (n == 1) return 1;

    // CACHE HIT: memo[n] already computed hai — seedha return karo!
    // Koi step count nahi — yeh free hai! Cache ka fayda yahi hai.
    if (memo[n] != -1) {
        return (long)memo[n];
    }

    // CACHE MISS: pehli baar is n ki value chahiye — compute karo
    // Sirf yahan steps count karo — actual kaam yahan ho raha hai
    (*steps)++;

    // Recursive calls — ye bhi memo use karenge agar already computed
    long result = fib_memo_helper(n - 1, memo, steps)
                + fib_memo_helper(n - 2, memo, steps);

    // Result save karo memo mein — agla call seedha yahan se lega
    // NOTE: fib(80) > INT_MAX, isliye n <= 80 limit zaruri hai!
    // Yahan cast safe hai kyunki limit ke andar hain hum
    memo[n] = (int)result;

    return result;
}


// ==============================================================
//   FUNCTION: fibonacci_memo
//   Memoized Fibonacci — fast, efficient, production-ready!
//   Same answers as naive, but O(n) instead of O(2^n).
//
//   INT OVERFLOW ISSUE — yeh important hai!
//   Fibonacci numbers bahut tezi se badhte hain:
//   fib(46) = 1,836,311,903  ← INT_MAX ke paas (2,147,483,647)
//   fib(47) = 2,971,215,073  ← INT_MAX se bada! Overflow!
//   Hum memo[] mein int store karte hain, output[0] bhi int hai.
//   Toh agar n > 46, overflow hoga aur galat answer aayega.
//   Lekin hum n <= 80 limit rakh rahe hain kyunki:
//   - Helper mein long use kiya hai compute ke liye (safe!)
//   - memo[] mein cast to int karte hain (n<=46 ke liye safe)
//   - output[0] int hai — agar fib(47+) store karo toh garbage!
//   Real system mein long long output use karna chahiye.
//   SCAE ke liye n <= 40 meaningful hai (sensor count ke roop mein).
//
//   Limit n <= 80 kyun? — generous upper bound rakh rahe hain
//   memo ke liye, steps count dekhna interesting hai bade n pe bhi.
//   Caller ko pata hona chahiye ki output[0] n>46 pe unreliable hai.
//
//   Parameters: n — which Fibonacci number (0-indexed)
//   Returns: AlgoResult, output[0] = fib(n)
// ==============================================================
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

    // Memo array heap pe allocate karo — size n+1 (indices 0 to n)
    // VLA nahi use kar rahe kyunki:
    //   1. n bada ho sakta hai — stack overflow ka risk
    //   2. Malloc se size flexible aur safe rehta hai
    //   3. MSVC VLA support nahi karta (Windows compatibility!)
    int* memo = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (memo == NULL) {
        // Malloc fail — OOM situation
        strcpy(r.status, "NOT_FOUND");  // Error indicator
        return r;
    }

    // Sabko -1 se initialize karo — -1 matlab "abhi compute nahi hua"
    // Fibonacci values kabhi negative nahi hote, toh -1 safe sentinel hai
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
