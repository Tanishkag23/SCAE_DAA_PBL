/*
 * ============================================================
 * SCAE - Smart City Algorithm Engine
 * File: selection.c
 * Algorithm: Selection Sort
 * ============================================================
 * SCAE Context:
 *   In smart city infrastructure, some storage media — like
 *   EEPROM or flash memory on IoT sensors — are WRITE-EXPENSIVE.
 *   Writing data degrades the hardware over time. Selection sort
 *   is the hero here: it performs only O(n) SWAPS (writes),
 *   even though it does O(n^2) comparisons (reads).
 *
 *   For example, when logging sorted emergency priority data
 *   onto a sensor node with limited write cycles, selection
 *   sort minimizes hardware wear — each element moves to its
 *   final position in at most ONE write operation.
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult selection_sort(int* arr, int n) {

    // Edge case: khaali array ko sort karna? Koi kaam nahi!
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);
 // Caller ka array safe rakhna hai — apni copy pe kaam karo
    memcpy(r.output, arr, n * sizeof(int));

    int* a = r.output;
 // Selection: sort: unsorted part se minimum dhundho, sorted part ke end mein daalo Ek ek karke minimum element...

    clock_t start = clock();

    for (int i = 0; i < n - 1; i++) {

        // Maan lo pehla unsorted element hi minimum hai (shuruaat mein yahi assume karo)
        int min_idx = i;
 // Baaki sare unsorted elements se compare karo — kaun hai sabse chhota?
        for (int j = i + 1; j < n; j++) {
            r.steps++;  // Har comparison count karo — reads expensive nahi, writes expensive hain!

            if (a[j] < a[min_idx]) {
                min_idx = j;  // Naya minimum mila! Index update karo
            }
        }

        // Har pass mein exactly ek swap hota hai (ya zero agar already sahi jagah hai)
        if (min_idx != i) {
            // ADVANTAGE: minimum number of SWAPS — sirf O(n) swaps total!
            int temp    = a[i];
            a[i]        = a[min_idx];
            a[min_idx]  = temp;
        }

        // Flash: memory ya EEPROM jaise write-expensive storage pe selection sort better hai Kyunki writes zyada c...
    }

    clock_t end = clock();
 // ALWAYS: O(n^2): chahe already sorted ho ya reverse sorted — hamesha n*(n-1)/2 comparisons! Bubble sort ea...

    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.outputSize  = n;
    r.memory_used = (long)(sizeof(AlgoResult) + n * sizeof(int));

    strcpy(r.big_o,  "O(n^2)");
    strcpy(r.status, "SUCCESS");

    return r;
}
