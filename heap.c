/*
 * ============================================================
 *  SCAE — Smart City Algorithm Engine
 *  File   : heap.c
 *  Author : Tanishka (DAA Project)
 * ------------------------------------------------------------
 *  SCAE Context:
 *  Smart city operations mein kabhi kabhi sabhi pending
 *  complaints ya sensor alerts ek saath process karni padti
 *  hain — for example, end-of-day batch processing ya
 *  emergency re-prioritization.
 *
 *  Heap Sort yahan kyun?
 *   - GUARANTEED O(n log n): quicksort ka worst case O(n^2)
 *     ho sakta hai — heap sort mein yeh impossible hai.
 *   - IN-PLACE: merge sort ki tarah extra O(n) temp array
 *     nahi chahiye — sirf O(log n) stack.
 *   - PRIORITY QUEUE se better: agar sabhi complaints ek
 *     saath process karni hain, heap sort efficient hai.
 *   - Predictable: safety-critical city systems mein
 *     guaranteed timing bahut important hai.
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
 *  heapify — max-heap property maintain karo index i pe
 *
 *  Heap sort: max-heap use karta hai — ek aisa tree jahan
 *  parent hamesha bacha se bada hota hai.
 *  MAX-HEAP PROPERTY: a[parent] >= a[children] — poore
 *  tree mein yeh rule follow hota hai.
 *
 *  a    : array (heap as array representation)
 *  sz   : current heap size (shrinks during extraction)
 *  i    : index jahan heapify karna hai (root of subtree)
 *  steps: comparison counter (shared pointer)
 * ---------------------------------------------------------- */
static void heapify(int* a, int sz, int i, long* steps)
{
    /* Assume karo ki current node largest hai */
    int largest = i;

    /* Array-as-binary-tree: left child = 2*i+1, right = 2*i+2
     * Yeh zero-indexed array mein standard formula hai */
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    /* Left child exist karta hai aur parent se bada hai? */
    (*steps)++;
    if (left < sz && a[left] > a[largest]) {
        largest = left;
    }

    /* Right child exist karta hai aur abhi tak ke largest se bada hai? */
    (*steps)++;
    if (right < sz && a[right] > a[largest]) {
        largest = right;
    }

    /* Agar parent largest nahi hai — swap karo aur recursively
     * neeche heapify karo. Max-heap property restore ho jayegi. */
    if (largest != i) {
        SWAP(a[i], a[largest]);

        /* Recursive call: swap ke baad subtree fix karo */
        heapify(a, sz, largest, steps);
    }
    /* Agar i hi largest tha — kuch nahi karna, heap valid hai! */
}

/* ----------------------------------------------------------
 *  heap_sort — public entry point
 *
 *  arr : input array (caller ka — TOUCH NAHI KARNA!)
 *  n   : number of elements
 *
 *  Returns: AlgoResult with sorted output, step count,
 *           timing, memory usage, and big-O notation.
 *
 *  Algorithm overview (do phases):
 *   Phase 1 — BUILD HEAP:
 *     Bottom-up max-heap banao — O(n) time.
 *   Phase 2 — EXTRACTION:
 *     Max element (root) nikalo, end pe rakh do,
 *     heap shrink karo, heapify karo — n-1 baar.
 *     Total: O(n log n).
 * ---------------------------------------------------------- */
AlgoResult heap_sort(int* arr, int n)
{
    /* Edge case: khaali ya invalid array — gracefully handle karo */
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);

    /* Caller ka arr copy karo output mein — original ko mat chhuo!
     * In-place sort OUTPUT array pe hoga. */
    memcpy(r.output, arr, n * sizeof(int));

    /* Timing shuru karo — dono phases cover honge */
    clock_t start = clock();

    /* --------------------------------------------------------
     *  Phase 1: BUILD MAX-HEAP
     *
     *  BUILD HEAP PHASE: O(n) time mein max-heap banao —
     *  sirf n/2 se 0 tak heapify karo.
     *  Kyu n/2? Kyunki leaf nodes ko heapify karne ki zarurat
     *  nahi — already valid heap hain! (No children to compare)
     *
     *  Bottom-up approach: sabse neeche wale internal nodes se
     *  shuru karo, upar aao — correct order hai yeh!
     * -------------------------------------------------------- */
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(r.output, n, i, &r.steps);
    }

    /* --------------------------------------------------------
     *  Phase 2: EXTRACTION — sorted array banao
     *
     *  EXTRACTION PHASE: root (max element) ko end mein swap
     *  karo, heap size ghao, heapify karo.
     *  Baar baar yeh karo — sorted array ban jata hai!
     *  Elegant hai na?
     *
     *  Har iteration mein:
     *   - r.output[0] = current max (heap root)
     *   - Isko r.output[i] pe rakh do (sorted portion start)
     *   - Heap size i tak shrink ho gaya
     *   - Root pe heapify — next max upar aa jayega
     * -------------------------------------------------------- */
    for (int i = n - 1; i > 0; i--) {
        /* Max element (root) ko sorted portion ke end pe rakh do */
        SWAP(r.output[0], r.output[i]);

        /* Heap size ek kam hua — r.output[i] ab sorted hai */
        /* Naye root pe heapify — max-heap property restore karo */
        heapify(r.output, i, 0, &r.steps);
    }

    clock_t end = clock();

    /* Time nanoseconds mein convert karo */
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    /* IN-PLACE: merge sort ki tarah extra O(n) temp array
     * nahi chahiye — sirf O(log n) stack (recursive heapify).
     * Memory = AlgoResult struct + output array copy only. */
    r.memory_used = sizeof(AlgoResult) + n * sizeof(int);

    r.outputSize = n;

    /* GUARANTEED O(n log n): worst case bhi —
     * quicksort se zyada predictable!
     * SCAE mein: priority queue se better — agar sabhi
     * complaints ek saath process karni hain. */
    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
