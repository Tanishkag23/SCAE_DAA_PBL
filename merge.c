/*
 * ============================================================
 *  SCAE — Smart City Algorithm Engine
 *  File   : merge.c
 *  Author : Tanishka (DAA Project)
 * ------------------------------------------------------------
 *  SCAE Context:
 *  Ek smart city mein hazaron complaints, sensor readings,
 *  aur priority tickets aate hain. Unhe efficiently sort
 *  karna padta hai taaki sabse urgent kaam pehle ho.
 *
 *  Merge Sort yahan isliye perfect hai:
 *   - STABLE hai — same-priority complaints timestamp order
 *     mein rahti hain (FIFO within same priority).
 *   - GUARANTEED O(n log n) — koi bhi input worst case
 *     trigger nahi kar sakta.
 *   - Large datasets ke liye predictable performance deta hai.
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
 *  merge_helper — recursive divide-and-conquer
 *
 *  a    : array jo sort karni hai (in-place)
 *  temp : pre-allocated scratch buffer (merge ke liye)
 *  lo   : current subarray ka start index (inclusive)
 *  hi   : current subarray ka end index (inclusive)
 *  steps: comparison counter (pointer — shared across calls)
 * ---------------------------------------------------------- */
static void merge_helper(int* a, int* temp, int lo, int hi, long* steps)
{
    /* Base case: ek element already sorted hai — wapas jao! */
    if (lo >= hi) return;

    /* Divide: beech ka point nikalo — overflow-safe formula */
    int mid = lo + (hi - lo) / 2;

    /* Conquer: pehle left half sort karo ... */
    merge_helper(a, temp, lo, mid, steps);

    /* ... phir right half sort karo */
    merge_helper(a, temp, mid + 1, hi, steps);

    /* --------------------------------------------------
     *  Merge step: do sorted halves ko ek sorted array
     *  mein combine karo.
     *
     *  Strategy:
     *   1. a[lo..hi] ko temp[lo..hi] mein copy karo.
     *   2. temp se merge karke wapas a mein likho.
     *
     *  Temp array ek baar merge_sort mein allocate hua hai —
     *  yahan malloc karna galat hoga (expensive + slow)!
     * -------------------------------------------------- */

    /* Step 1: copy current segment to temp */
    for (int k = lo; k <= hi; k++) {
        temp[k] = a[k];
    }

    /* Step 2: merge temp[lo..mid] and temp[mid+1..hi] -> a[lo..hi] */
    int i = lo;        /* left half ka pointer  */
    int j = mid + 1;  /* right half ka pointer */
    int k = lo;        /* output ka pointer     */

    while (i <= mid && j <= hi) {
        /* Merge sort mein comparisons count karo —
         * divide step free hai (sirf index arithmetic) */
        (*steps)++;

        /* Stable: equal elements mein left ko pehle rakho —
         * City complaint system mein stability matter karta hai!
         * Same urgency complaints unhi order mein rahni chahiye
         * jis order mein aayi thi (timestamp order). */
        if (temp[i] <= temp[j]) {
            a[k++] = temp[i++];
        } else {
            a[k++] = temp[j++];
        }
    }

    /* Bacha hua left half copy karo (agar kuch bacha ho) */
    while (i <= mid) {
        a[k++] = temp[i++];
    }

    /* Bacha hua right half copy karo (agar kuch bacha ho) */
    while (j <= hi) {
        a[k++] = temp[j++];
    }
}

/* ----------------------------------------------------------
 *  merge_sort — public entry point
 *
 *  arr : input array (caller ka — TOUCH NAHI KARNA!)
 *  n   : number of elements
 *
 *  Returns: AlgoResult with sorted output, step count,
 *           timing, memory usage, and big-O notation.
 * ---------------------------------------------------------- */
AlgoResult merge_sort(int* arr, int n)
{
    /* Edge case: khaali ya invalid array — gracefully handle karo */
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    /* Merge sort: divide and conquer — problem ko chota karo,
     * solve karo, combine karo! */
    AlgoResult r = create_result(n);

    /* Caller ka arr copy karo output mein — original ko mat chhuo! */
    memcpy(r.output, arr, n * sizeof(int));

    /* Temp array ek baar allocate karo merge_sort mein —
     * har recursive call mein malloc MAT karo!
     * Har malloc expensive hai — ek baar karo, sab recursive
     * calls share karenge. */
    int* temp = (int*)malloc(n * sizeof(int));
    if (!temp) {
        /* malloc fail ho gaya — error status set karo */
        strcpy(r.status, "ERROR: malloc failed for temp buffer");
        return r;
    }

    /* Timing shuru karo — clock_t accurate hai short sorts ke liye */
    clock_t start = clock();

    /* Recursive merge sort: 0 to n-1 */
    merge_helper(r.output, temp, 0, n - 1, &r.steps);

    clock_t end = clock();

    /* Temp buffer free karo — memory leak mat karo! */
    free(temp);
    temp = NULL;

    /* Time nanoseconds mein convert karo */
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    /* Memory: AlgoResult struct + output array + temp array
     * STABLE SORT hai yeh — equal elements ki relative order
     * maintain hoti hai. Extra O(n) memory ki yeh cost hai. */
    r.memory_used = sizeof(AlgoResult) + 2 * n * sizeof(int);

    r.outputSize = n;

    /* O(n log n) guaranteed — koi worst case nahi!
     * Quicksort se zyada predictable — SCAE ke liye reliable! */
    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
