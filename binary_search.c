// binary_search: SCAE Smart City Algorithm Engine Binary Search: sorted array mein kisi element ko dhundho Yeh fil...
#include "../scae.h"


// binary_search_iterative: Sorted integer array mein key ko iterative binary search se dhundho
AlgoResult binary_search_iterative(int* arr, int n, int key) {
    // Output array mein sirf ek element chahiye — found index ya -1
    AlgoResult r = create_result(1);
 // Time: measurement shuru — clock() CPU ticks deta hai wall-clock nahi, CPU time — multithreaded systems ...
    clock_t start = clock();

    int lo = 0;
    int hi = n - 1;
 // Big-O: set karo — Binary Search ka theoretical complexity O(log n) isliye ki har step search space HALF ...
    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;

    while (lo <= hi) {
        // MID: CALCULATION — OVERFLOW WALI BAAT SUNO DHYAN SE! Galat tarika: mid = (lo + hi) / 2 Agar lo = 2,000...
        int mid = lo + (hi - lo) / 2;
 // Har: comparison ek step — iterative mein yeh loop body hai steps se hum theoretical aur practical comp...
        r.steps++;

        if (arr[mid] == key) {
            // Mil gaya! Index store karo aur status update karo
            r.output[0] = mid;
            strcpy(r.status, "FOUND");
            break;  // Kaam ho gaya, loop se bahar aao

        } else if (arr[mid] < key) {
            // Mid: se chhota hai key — left half eliminate karo Ab sirf right half mein dhundhna hai
            lo = mid + 1;

        } else {
            // Mid: se bada hai key — right half eliminate karo Ab sirf left half mein dhundhna hai
            hi = mid - 1;
        }
    }

    // Agar loop khatam hua bina break ke — key mili nahi!
    if (strcmp(r.status, "FOUND") != 0) {
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    // Time: measure karo — clock() end - start = CPU ticks lage Nanoseconds mein convert: ticks / CLOCKS_PER_...
    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
 // memory_used: ek AlgoResult struct + ek int output — simple!
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}


// bs_recursive_helper: Recursive binary search ka asli kaam yahan hota hai
static int bs_recursive_helper(int* arr, int lo, int hi, int key, long* steps) {
    // Base case: search window khatam — key mili nahi
    if (lo > hi) {
        return -1;
    }

    // Har: recursive call = ek comparison step Pointer se update karo taaki wrapper function mein dikhay
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


// binary_search_recursive: Public wrapper — helper ko call karo aur AlgoResult pack karo
AlgoResult binary_search_recursive(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;
 // Time: measurement — recursive version ke liye bhi same approach Note: recursive calls ki stack frames t...
    clock_t start = clock();
 // Helper: ko call karo — steps ka pointer pass karo taki har recursive call step count update kar sake
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

    // memory_used:: recursive ke liye stack frames bhi hote hain! O(log n) stack depth * approx frame size — rough es...
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}
