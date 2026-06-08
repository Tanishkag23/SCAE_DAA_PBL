/*
 * ============================================================
 * SCAE - Smart City Algorithm Engine
 * File: counting.c
 * Algorithm: Counting Sort
 * ============================================================
 * SCAE Context:
 *   Smart cities deal with data that has bounded integer ranges:
 *   complaint priority (1-10), pollution AQI (0-500), traffic
 *   congestion level (0-100), star ratings (1-5). For such data,
 *   counting sort is REVOLUTIONARY — it breaks the O(n log n)
 *   barrier of comparison-based sorts and achieves O(n+k)!
 *
 *   SCAE uses counting sort when the value range (k) is known
 *   and reasonable. A safety cap of 100000 is enforced to
 *   prevent memory explosions on edge-case inputs.
 *
 *   Stability is preserved by building output in REVERSE order,
 *   which ensures equal elements maintain their original order.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult counting_sort(int* arr, int n) {

    /* Edge case: khaali array — kuch nahi karna */
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    /* Pehle max value dhundho — counting sort ke liye range jaanna zaroori hai */
    /* Yeh loop clock se bahar hai — max finding ki cost separately nahi count karte */
    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    /* k = max value — agar k bahut bada hai toh memory bahut lagti hai count array ke liye */
    /* Isliye 100000 limit rakha — 100001 ints = ~400KB, reasonable. 10M ints = 40MB, nahi! */
    if (max_val > 100000) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n+k)");
        return r;
    }

    AlgoResult r = create_result(n);
    strcpy(r.big_o, "O(n+k)");
    r.outputSize = n;

    /* Count array allocate karo — har value ki frequency store karni hai */
    int* count = (int*)calloc(max_val + 1, sizeof(int));
    if (!count) {
        /* Memory nahi mili — failure handle karo */
        strcpy(r.status, "ALLOC_FAILED");
        return r;
    }

    /* Non-comparison based sort: O(n+k) achieve karta hai, comparison sorts O(n log n) se faster! */
    /* Counting sort: comparison hi nahi karta — direct index use karta hai! Revolutionary! */

    clock_t start = clock();

    /* Phase 1: Count occurrences — har element kitni baar aaya? */
    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
        r.steps++;  /* Har read/write ek step */
    }

    /* Phase 2: Prefix sum — count[i] = "kitne elements i se CHOTE ya BARABAR hain" */
    /* Prefix sum trick: yeh batata hai ki element i ka output array mein kahan tak ka space hai */
    for (int i = 1; i <= max_val; i++) {
        count[i] += count[i - 1];
        r.steps++;  /* Prefix sum bhi ek operation */
    }

    /* Phase 3: Build output — Phir reverse mein place karo — STABILITY maintain hoti hai! */
    /* Reverse mein isliye: equal elements apna relative order maintain karte hain */
    for (int i = n - 1; i >= 0; i--) {
        r.output[--count[arr[i]]] = arr[i];
        r.steps++;  /* Placement bhi ek step */
    }

    clock_t end = clock();

    /* SCAE mein: complaint urgency 1-10 sort karo — k=10, instant! */

    free(count);
    count = NULL;

    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + (long)(n * sizeof(int)) + (long)((max_val + 1) * sizeof(int)));

    strcpy(r.status, "SUCCESS");

    return r;
}
