// ==============================================================
//   linear_search.c — SCAE Smart City Algorithm Engine
//   Linear Search: array mein left se right scan karo
//
//   Do versions implement kiye hain:
//     1. linear_search          — classic, seedha scan
//     2. linear_search_sentinel — ek smart optimization
//                                 jo ek bounds check per
//                                 iteration bachata hai
//
//   Binary search se slower hai (O(n) vs O(log n)) — lekin
//   unsorted arrays mein YAHI kaam aata hai! City complaints
//   list sorted nahi hoti, toh linear search zaruri hai.
// ==============================================================

#include "../scae.h"


// ==============================================================
//   FUNCTION: linear_search
//   Classic left-to-right scan — simple, reliable, sorted ya
//   unsorted dono mein kaam karta hai.
//
//   Parameters:
//     arr  — integer array (sorted hona zaruri nahi!)
//     n    — array ki length
//     key  — dhundhna kya hai
//
//   Returns: AlgoResult (heap-allocated output, size=1)
// ==============================================================
AlgoResult linear_search(int* arr, int n, int key) {
    // Output array mein ek element — found index ya -1
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");  // Worst case: poora array scan karna padega
    r.outputSize = 1;

    // Time measurement — clock() CPU ticks deta hai
    clock_t start = clock();

    // Left se right, ek ek element check karo
    // Koi shortcut nahi — yahi toh linear search hai!
    for (int i = 0; i < n; i++) {
        // Har element comparison = ek step
        // Best case: pehla element hi key hai — 1 step
        // Worst case: last element ya not found — n steps
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


// ==============================================================
//   FUNCTION: linear_search_sentinel
//   Sentinel optimization — ek chota sa jugaad jo loop mein
//   ek comparison per iteration bachata hai!
//
//   SENTINEL TRICK SAMJHO:
//   Normal linear search mein loop condition hai:
//     while (i < n && arr[i] != key)   ← DO comparisons har step!
//       i < n       → bounds check (array se bahar mat jao)
//       arr[i]!=key → actual search comparison
//
//   Sentinel trick:
//   Array ke LAST POSITION pe temporarily key daal do.
//   Ab loop ko bounds check ki zarurat nahi! Loop sirf
//   arr[i]==key check karta hai — aur yeh KABHI bhi true
//   ho jayega (worst case: sentinel pe hi sahi).
//     while (arr[i] != key)   ← SIRF EK comparison!
//       i++
//
//   Million elements pe yeh 50% comparisons bachata hai!
//   Micro-optimization hai, lekin embedded systems mein matter karta hai.
//
//   IMPORTANT: arr[n-1] ko temporarily overwrite karte hain —
//   original value ZARURI save karni hai, warna caller ka data
//   corrupt ho jayega! Function khatam hote hi restore karo.
//
//   Parameters:
//     arr  — integer array (direct modify karte hain temporarily!)
//     n    — array ki length (n >= 1 hona chahiye)
//     key  — dhundhna kya hai
//
//   Returns: AlgoResult (heap-allocated output, size=1)
// ==============================================================
AlgoResult linear_search_sentinel(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");  // Sentinel se constant factor kam hota hai,
                                // lekin Big-O same rehta hai — asymptotically O(n)
    r.outputSize = 1;

    clock_t start = clock();

    // STEP 1: arr[n-1] ki original value save karo
    // Yeh BAHUT ZARURI hai — agar save nahi kiya toh:
    //   - Caller ka array permanently corrupt ho jayega
    //   - Dobara us array pe koi bhi operation galat results dega
    //   - Debug karna nightmare hoga! ("Mera data kahan gaya??")
    int last_val = arr[n - 1];

    // STEP 2: Sentinel place karo — last position pe key daal do
    // Ab loop guaranteed terminate karega bina bounds check ke
    arr[n - 1] = key;

    // STEP 3: Loop chalao — sirf value check, no bounds check!
    // i < n check nahi karna kyunki sentinel guarantee karta hai
    // ki loop n-1 se pehle ya n-1 pe zaroor rukegi
    int i = 0;
    while (arr[i] != key) {
        r.steps++;  // Yeh step tab count hota hai jab match NAHI hota
        i++;
    }
    // Ek aur step — last comparison (jahan match hua)
    r.steps++;

    // STEP 4: arr[n-1] restore karo — caller ka data wapas do!
    // Yeh line BHULO MAT — yahi is function ki zimmedari hai
    // "Tumne array uthaya, kaam kiya, wapas rakh do jaise tha"
    arr[n - 1] = last_val;

    // STEP 5: Ab decide karo — real find hua ya sirf sentinel hit?
    //
    // Teeno cases:
    //   Case A: i < n-1
    //     Loop n-1 se pehle ruka — matlab key genuinely wahan thi
    //     Yeh real find hai — FOUND!
    //
    //   Case B: i == n-1 AND last_val == key
    //     Loop last position pe ruka — lekin last_val bhi key thi!
    //     Matlab original arr[n-1] hi key tha — real find, FOUND!
    //
    //   Case C: i == n-1 AND last_val != key
    //     Loop last position pe ruka — sirf sentinel ki wajah se
    //     Original arr[n-1] key nahi tha — yeh fake hit hai
    //     Key genuinely nahi mili — NOT_FOUND!
    if (i < n - 1 || last_val == key) {
        // Real find! Case A ya Case B
        // i ko correctly set karo:
        //   - Case A: i already sahi index pe hai
        //   - Case B: i == n-1, jo sahi index bhi hai
        r.output[0] = i;
        strcpy(r.status, "FOUND");
    } else {
        // Sirf sentinel hit hua — Case C
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int) * 2);
    // *2 isliye ki last_val bhi ek extra int variable hai stack pe

    return r;
}
