// ==============================================================
//   binary_search.c — SCAE Smart City Algorithm Engine
//   Binary Search: sorted array mein kisi element ko dhundho
//   Yeh file do flavors provide karta hai:
//     1. Iterative  — loop wala, stack safe, production ready
//     2. Recursive  — elegant, lekin deep arrays pe stack overflow
//                     ho sakta hai (SCAE mein N chhota hai, safe hai)
// ==============================================================

#include "../scae.h"


// ==============================================================
//   FUNCTION: binary_search_iterative
//   Sorted integer array mein key ko iterative binary search se
//   dhundho. Har comparison ek step count hoti hai.
//
//   Parameters:
//     arr  — sorted integer array (caller ki zimmedari sort karna)
//     n    — array ki length
//     key  — dhundhna kya hai
//
//   Returns: AlgoResult (heap-allocated output, size=1)
// ==============================================================
AlgoResult binary_search_iterative(int* arr, int n, int key) {
    // Output array mein sirf ek element chahiye — found index ya -1
    AlgoResult r = create_result(1);

    // Time measurement shuru — clock() CPU ticks deta hai
    // wall-clock nahi, CPU time — multithreaded systems pe zyada accurate
    clock_t start = clock();

    int lo = 0;
    int hi = n - 1;

    // Big-O set karo — Binary Search ka theoretical complexity
    // O(log n) isliye ki har step search space HALF ho jata hai
    // 1 billion elements? Sirf 30 comparisons! Mast hai na?
    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;

    while (lo <= hi) {
        // MID CALCULATION — OVERFLOW WALI BAAT SUNO DHYAN SE!
        // Galat tarika: mid = (lo + hi) / 2
        //   Agar lo = 2,000,000,000 aur hi = 2,000,000,001 toh
        //   lo + hi = 4,000,000,001 → int overflow → UNDEFINED BEHAVIOUR!
        //   C mein signed overflow = UB = naak pe ghoonsa!
        //
        // Sahi tarika: mid = lo + (hi - lo) / 2
        //   hi - lo kabhi bhi INT_MAX se zyada nahi ho sakta (array size bounded)
        //   phir lo add karo — safe, clean, professional!
        int mid = lo + (hi - lo) / 2;

        // Har comparison ek step — iterative mein yeh loop body hai
        // steps se hum theoretical aur practical comparison karate hain
        r.steps++;

        if (arr[mid] == key) {
            // Mil gaya! Index store karo aur status update karo
            r.output[0] = mid;
            strcpy(r.status, "FOUND");
            break;  // Kaam ho gaya, loop se bahar aao

        } else if (arr[mid] < key) {
            // Mid se chhota hai key — left half eliminate karo
            // Ab sirf right half mein dhundhna hai
            lo = mid + 1;

        } else {
            // Mid se bada hai key — right half eliminate karo
            // Ab sirf left half mein dhundhna hai
            hi = mid - 1;
        }
    }

    // Agar loop khatam hua bina break ke — key mili nahi!
    if (strcmp(r.status, "FOUND") != 0) {
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    // Time measure karo — clock() end - start = CPU ticks lage
    // Nanoseconds mein convert: ticks / CLOCKS_PER_SEC * 1e9
    // 1e9 isliye ki nanoseconds = seconds * 10^9
    // Binary search itna fast hai ki microseconds mein bhi accurate nahi hota!
    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    // memory_used: ek AlgoResult struct + ek int output — simple!
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}


// ==============================================================
//   INTERNAL HELPER: bs_recursive_helper
//   Recursive binary search ka asli kaam yahan hota hai.
//
//   Yeh function "static" kyun hai?
//     static = file-scope only — bahar se koi seedha call nahi
//     kar sakta. Encapsulation! Internal implementation detail
//     hide karo — public API sirf binary_search_recursive hai.
//     Naam conflict bhi avoid hota hai agar kisi aur file mein
//     bhi helper function ho.
//
//   Parameters:
//     arr   — sorted array
//     lo    — current search window ka left bound
//     hi    — current search window ka right bound
//     key   — dhundhna kya hai
//     steps — pointer to step counter (r.steps update karte rehte hain)
//
//   Returns: found index, ya -1 if not found
// ==============================================================
static int bs_recursive_helper(int* arr, int lo, int hi, int key, long* steps) {
    // Base case: search window khatam — key mili nahi
    if (lo > hi) {
        return -1;
    }

    // Har recursive call = ek comparison step
    // Pointer se update karo taaki wrapper function mein dikhay
    (*steps)++;

    // Same overflow-safe mid calculation — recursive mein bhi same rule!
    int mid = lo + (hi - lo) / 2;

    if (arr[mid] == key) {
        return mid;  // Mil gayi! Index return karo — recursion unwind hogi

    } else if (arr[mid] < key) {
        // Right half mein dhundho — left half eliminate
        return bs_recursive_helper(arr, mid + 1, hi, key, steps);

    } else {
        // Left half mein dhundho — right half eliminate
        return bs_recursive_helper(arr, lo, mid - 1, key, steps);
    }
}


// ==============================================================
//   FUNCTION: binary_search_recursive
//   Public wrapper — helper ko call karo aur AlgoResult pack karo.
//   Same interface as iterative — caller ko andar ki baat nahi
//   pata chalegi ki iterative hai ya recursive!
//
//   Parameters:
//     arr  — sorted integer array
//     n    — array ki length
//     key  — dhundhna kya hai
//
//   Returns: AlgoResult (heap-allocated output, size=1)
// ==============================================================
AlgoResult binary_search_recursive(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;

    // Time measurement — recursive version ke liye bhi same approach
    // Note: recursive calls ki stack frames time mein add hoti hain
    // isliye recursive slightly slower dikhega for same input
    clock_t start = clock();

    // Helper ko call karo — steps ka pointer pass karo taki
    // har recursive call step count update kar sake
    int idx = bs_recursive_helper(arr, 0, n - 1, key, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    // Result set karo based on helper ka return value
    if (idx >= 0) {
        r.output[0] = idx;
        strcpy(r.status, "FOUND");
    } else {
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    // memory_used: recursive ke liye stack frames bhi hote hain!
    // O(log n) stack depth * approx frame size — rough estimate
    // Yahan sirf struct + output count karte hain (stack measure karna mushkil hai)
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}
