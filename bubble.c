/*
 * ============================================================
 * SCAE - Smart City Algorithm Engine
 * File: bubble.c
 * Algorithm: Bubble Sort (Optimized with Early Exit)
 * ============================================================
 * SCAE Context:
 *   Smart cities generate continuous streams of data — sensor
 *   readings, traffic densities, complaint priorities, pollution
 *   levels. Bubble sort, though not the fastest for large data,
 *   is a great teaching tool and performs surprisingly well on
 *   NEARLY SORTED data (like real-time sensor streams where
 *   values change slightly each tick).
 *
 *   With early-exit optimization, if the city data is already
 *   in order, bubble sort detects it in ONE pass — O(n)!
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult bubble_sort(int* arr, int n) {

    // Edge case: khaali ya invalid array — kuch karne ki zaroorat nahi
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);

    // Caller ka array safe rakhna hai — apni copy pe kaam karo
    memcpy(r.output, arr, n * sizeof(int));

    int* a = r.output;

    // Bubble sort: bade elements 'bubble up' karte hain — jaise paani mein hawa ke bulbule!
    // Har pass mein sabse bada unsorted element apni sahi jagah pe pahunch jata hai

    clock_t start = clock();

    for (int i = 0; i < n - 1; i++) {

        // Early-exit optimization: agar ek bhi swap nahi hua, matlab array already sorted hai!
        int swapped = 0;

        for (int j = 0; j < n - i - 2 + 1; j++) {

            // Har comparison ek step — swaps ko count nahi karte kyunki comparison main operation hai
            r.steps++;

            if (a[j] > a[j + 1]) {
                // Swap karo — chhota element aage aao, bada element peeche jao!
                int temp = a[j];
                a[j]     = a[j + 1];
                a[j + 1] = temp;
                swapped  = 1;
            }
        }

        // Best case O(n): already sorted array mein sirf ek pass chahiye — genius hai!
        if (!swapped) break;  // Koi swap nahi hua? Array sorted hai, band karo!
    }

    clock_t end = clock();

    // Worst case O(n^2): reverse sorted array mein har element apni jagah tak bubble karta hai

    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.outputSize  = n;
    r.memory_used = (long)(sizeof(AlgoResult) + n * sizeof(int));

    strcpy(r.big_o,  "O(n^2)");
    strcpy(r.status, "SUCCESS");

    return r;
}
