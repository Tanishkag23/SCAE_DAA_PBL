/*
 * ============================================================
 * SCAE - Smart City Algorithm Engine
 * File: insertion.c
 * Algorithm: Insertion Sort
 * ============================================================
 * SCAE Context:
 *   Insertion sort shines as an ONLINE ALGORITHM — it can sort
 *   data as it arrives, without needing the full dataset upfront.
 *   In a smart city, complaints, sensor readings, and emergency
 *   tickets arrive continuously. Insertion sort can integrate
 *   each new entry into an already-sorted list efficiently.
 *
 *   For nearly-sorted data (e.g., daily temperature readings
 *   that fluctuate only slightly), insertion sort approaches
 *   O(n) — making it incredibly fast in practice.
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult insertion_sort(int* arr, int n) {

    // Edge case: agar array hi nahi hai toh sort kya karein?
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);

    // Caller ka array safe rakhna hai — apni copy pe kaam karo
    memcpy(r.output, arr, n * sizeof(int));

    int* a = r.output;

    // Insertion sort: taash ke paaton ki tarah sort karo — ek ek card uthao aur sahi jagah daalo
    // Pehla element already "sorted" hai (akela hai!), toh i=1 se shuru karo

    clock_t start = clock();

    for (int i = 1; i < n; i++) {

        // Key = woh card jo haath mein uthaya, ab sahi jagah dhoondho
        int key = a[i];
        int j   = i - 1;

        // City mein nai complaints aati hain continuously — insertion sort unhe realtime sort kar sakta hai
        // Nearly-sorted data pe SUPERFAST hai — almost O(n) — kyunki while loop jaldi exit karta hai

        // Comparison count: while loop ki condition bhi step hai — fail condition bhi count karo!
        while (j >= 0 && a[j] > key) {
            r.steps++;          // Yeh comparison hua — count karo
            a[j + 1] = a[j];   // Bade element ko ek jagah daayein shift karo
            j--;
        }

        // While loop ek baar aur check kiya aur fail hua — woh comparison bhi count hoti hai!
        // (ya toh j < 0 hua, ya a[j] <= key mila — dono cases mein ek comparison hua)
        r.steps++;

        // Sahi jagah mil gayi — key ko yahan daalo
        a[j + 1] = key;

        // Worst case: reverse sorted — har element poori left side se guzar ke jata hai — O(n^2)
    }

    clock_t end = clock();

    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.outputSize  = n;
    r.memory_used = (long)(sizeof(AlgoResult) + n * sizeof(int));

    // ONLINE ALGORITHM: naya data aata rahe, sort karte raho — poora array pehle nahi chahiye!
    strcpy(r.big_o,  "O(n^2)");
    strcpy(r.status, "SUCCESS");

    return r;
}
