// linear_search: SCAE Smart City Algorithm Engine Linear Search: array mein left se right scan karo Do versions im...
#include "../scae.h"


// linear_search: Classic left-to-right scan — simple, reliable, sorted ya unsorted dono mein kaam karta hai
AlgoResult linear_search(int* arr, int n, int key) {
    // Output array mein ek element — found index ya -1
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");  // Worst case: poora array scan karna padega
    r.outputSize = 1;
 // Time measurement — clock() CPU ticks deta hai
    clock_t start = clock();
 // Left: se right, ek ek element check karo Koi shortcut nahi — yahi toh linear search hai!
    for (int i = 0; i < n; i++) {
        // Har: element comparison = ek step Best case: pehla element hi key hai — 1 step Worst case: last elemen...
        r.steps++;

        if (arr[i] == key) {
            // Mil gaya! Index store karo
            r.output[0] = i;
            strcpy(r.status, "FOUND");
 // Clock rok lo — kaam ho gaya, baaki array scan karne ki zarurat nahi
            clock_t end = clock();
            r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
            r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
            return r;  // Early return — efficiency!
        }
    }

    // Loop khatam hua, mila nahi — NOT_FOUND
    r.output[0] = -1;
    strcpy(r.status, "NOT_FOUND");

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}


// linear_search_sentinel: Sentinel optimization — ek chota sa jugaad jo loop mein ek comparison per iteration bachata hai! ...
AlgoResult linear_search_sentinel(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");  // Sentinel se constant factor kam hota hai,
                                // lekin Big-O same rehta hai — asymptotically O(n)
    r.outputSize = 1;

    clock_t start = clock();
 // STEP: 1: arr[n-1] ki original value save karo Yeh BAHUT ZARURI hai — agar save nahi kiya toh: - Caller ...
    int last_val = arr[n - 1];
 // STEP: 2: Sentinel place karo — last position pe key daal do Ab loop guaranteed terminate karega bina bo...
    arr[n - 1] = key;
 // STEP: 3: Loop chalao — sirf value check, no bounds check! i < n check nahi karna kyunki sentinel guaran...
    int i = 0;
    while (arr[i] != key) {
        r.steps++;  // Yeh step tab count hota hai jab match NAHI hota
        i++;
    }
    // Ek aur step — last comparison (jahan match hua)
    r.steps++;
 // STEP: 4: arr[n-1] restore karo — caller ka data wapas do! Yeh line BHULO MAT — yahi is function ki zimm...
    arr[n - 1] = last_val;
 // STEP: 5: Ab decide karo — real find hua ya sirf sentinel hit? Teeno cases: Case A: i < n-1 Loop n-1 se ...
    if (i < n - 1 || last_val == key) {
        // Real: find! Case A ya Case B i ko correctly set karo: - Case A: i already sahi index pe hai - Case B: i...
        r.output[0] = i;
        strcpy(r.status, "FOUND");
    } else {
        // Sirf sentinel hit hua — Case C
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int) * 2); // *2 isliye ki last_val bhi ek extra int variable hai stack pe

    return r;
}
