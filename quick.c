/*
 * ============================================================
 *  SCAE — Smart City Algorithm Engine
 *  File   : quick.c
 *  Author : Tanishka (DAA Project)
 * ------------------------------------------------------------
 *  SCAE Context:
 *  Smart city mein real-time data processing ke liye speed
 *  sabse important hai. Quicksort average case mein fastest
 *  in-place sorting algorithm hai.
 *
 *  Quick Sort yahan kyun?
 *   - IN-PLACE: extra O(n) memory nahi chahiye — embedded
 *     systems aur memory-constrained sensors ke liye perfect.
 *   - CACHE FRIENDLY: sequential memory access patterns —
 *     modern CPUs pe badi speed advantage milti hai.
 *   - RANDOMIZED PIVOT: sorted/nearly-sorted city data pe
 *     bhi O(n^2) worst case avoid hota hai.
 *   - Average O(n log n) — real-world city workloads ke liye
 *     practically sabse fast hai.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../scae.h"

/* ----------------------------------------------------------
 *  Swap macro — temp variable use karo, XOR swap se dur raho!
 *  XOR swap same-address aliasing pe fail ho sakta hai.
 * ---------------------------------------------------------- */
#define SWAP(a, b) do { int _t = (a); (a) = (b); (b) = _t; } while (0)

/* ----------------------------------------------------------
 *  partition — Lomuto partition scheme
 *
 *  Pivot pehle se a[hi] pe set hai (quick_helper ne swap kiya).
 *  Sab elements jo pivot se <= hain, unhe left side le jao;
 *  baaki right side.
 *
 *  a    : array
 *  lo   : subarray start (inclusive)
 *  hi   : subarray end (inclusive) — pivot yahan hai
 *  steps: comparison counter
 *
 *  Returns: pivot ka final index (correctly placed position)
 * ---------------------------------------------------------- */
static int partition(int* a, int lo, int hi, long* steps)
{
    /* Pivot already a[hi] pe hai — Lomuto convention */
    int pivot = a[hi];

    /* i: last index jahan tak "small elements" zone hai */
    int i = lo - 1;

    /* Quicksort: city ka traffic officer — pivot pe khada ho,
     * chhote left bhejo, bade right! */
    for (int j = lo; j <= hi - 1; j++) {
        /* Har element ko pivot se compare karo — step count karo */
        (*steps)++;

        if (a[j] <= pivot) {
            /* Yeh element pivot se chota ya barabar hai —
             * small zone mein leke jao */
            i++;
            SWAP(a[i], a[j]);
        }
        /* Bada element? Kuch mat karo — right side pe rehne do */
    }

    /* Pivot ko uski sahi jagah pe rakh do —
     * ab a[i+1] left se bada aur right se chota hai */
    SWAP(a[i + 1], a[hi]);

    /* Pivot ka final correct index return karo */
    return i + 1;
}

/* ----------------------------------------------------------
 *  quick_helper — recursive quicksort
 *
 *  a    : array to sort in-place
 *  lo   : subarray start (inclusive)
 *  hi   : subarray end (inclusive)
 *  steps: comparison counter (shared pointer)
 * ---------------------------------------------------------- */
static void quick_helper(int* a, int lo, int hi, long* steps)
{
    /* Base case: ek ya zero elements — already sorted! */
    if (lo >= hi) return;

    /* RANDOMIZED PIVOT: worst case se bachao —
     * sorted array pe naive quicksort O(n^2) ho jata!
     * agar hamesha pehla/aakhri element pivot ho aur array
     * already sorted ho — disaster! */
    int pivot_idx = lo + rand() % (hi - lo + 1);

    /* Pivot ko end pe move karo — Lomuto style */
    SWAP(a[pivot_idx], a[hi]);

    /* Partition: pivot ko uski correct position pe rakh do */
    int p = partition(a, lo, hi, steps);

    /* Left half: elements jo pivot se chote hain */
    quick_helper(a, lo, p - 1, steps);

    /* Right half: elements jo pivot se bade hain */
    quick_helper(a, p + 1, hi, steps);

    /* Random pivot se expected O(n log n) guarantee hoti hai —
     * probability se! Koi bhi particular permutation worst case
     * nahi ban sakti jab pivot random ho. */
}

/* ----------------------------------------------------------
 *  quick_sort — public entry point
 *
 *  arr : input array (caller ka — TOUCH NAHI KARNA!)
 *  n   : number of elements
 *
 *  Returns: AlgoResult with sorted output, step count,
 *           timing, memory usage, and big-O notation.
 * ---------------------------------------------------------- */
AlgoResult quick_sort(int* arr, int n)
{
    /* Edge case: khaali ya invalid array — gracefully handle karo */
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);

    /* Caller ka arr copy karo output mein — original ko mat chhuo!
     * In-place sort OUTPUT array pe hoga, not on caller's arr. */
    memcpy(r.output, arr, n * sizeof(int));

    /* srand(42) — seed fix kiya hai reproducibility ke liye —
     * tests deterministic rahein. Same input = same pivot choices
     * = same comparison count — testing ke liye zaroori! */
    srand(42);

    /* Timing shuru karo */
    clock_t start = clock();

    /* Recursive quicksort: 0 to n-1 */
    quick_helper(r.output, 0, n - 1, &r.steps);

    clock_t end = clock();

    /* Time nanoseconds mein convert karo */
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    /* In-place hai yeh! Merge sort ki tarah extra O(n) memory
     * nahi chahiye — sirf AlgoResult struct + output array copy.
     * Stack space O(log n) average (recursive calls) —
     * tail recursion optimize ho sakti hai. */
    r.memory_used = sizeof(AlgoResult) + n * sizeof(int);

    r.outputSize = n;

    /* Lomuto partition: ek pointer se — simple implementation,
     * thoda more swaps Hoare se, but readable aur correct! */
    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
