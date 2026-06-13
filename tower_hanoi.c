// tower_hanoi: SCAE Smart City Algorithm Engine Tower of Hanoi: n disks ko peg A se peg C pe move karo (peg B ko...
#include "../scae.h"


// hanoi_helper: Yeh recursive function asli kaam karta hai — wrapper usse call karta hai aur result pack karta hai
static void hanoi_helper(int n, int src, int dst, int aux,
                          int* output, int* move_idx, long* steps) {
    // Base: case: 0 disks = kuch karna nahi — return! Yeh recursion ka anchor hai — iske bina infinite recurs...
    if (n <= 0) return;
 // STEP: 1: Upar ke (n-1) disks ko src se aux pe move karo dst ko temporary helper use karo Yeh sub-proble...
    hanoi_helper(n - 1, src, aux, dst, output, move_idx, steps);
 // STEP: 2: Sabse bada (bottom) disk seedha src se dst pe move karo Yeh ACTUAL move hai — output mein reco...
    output[*move_idx]     = src;  // Source peg ka int code
    output[*move_idx + 1] = dst;  // Destination peg ka int code
    *move_idx += 2;               // Next move ke liye 2 positions aage badho
    (*steps)++;                   // Ek disk move = ek step

    // STEP: 3: Upar ke (n-1) disks ko aux se dst pe move karo src ko ab helper use karo Yeh Step 1 ka "mirror...
    hanoi_helper(n - 1, aux, dst, src, output, move_idx, steps);
}


// tower_hanoi: Public function — n disks ke liye Tower of Hanoi solve karo aur saare moves AlgoResult mein pack ...
AlgoResult tower_hanoi(int n) {

    // Edge: case 1: n <= 0 — koi disk nahi, koi move nahi Empty result return karo — valid input hai, steps=0
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }

    // Edge: case 2: n > 20 — bahut bada hai, memory blow ho jayegi Status "TOO_LARGE" set karo aur empty resu...
    if (n > 20) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }

    // Total: moves calculate karo: 2^n - 1 (1 << n) = 2^n using bit shift — fast aur clean! Bit shift isliye u...
    int n_moves = (1 << n) - 1;
 // Output: array size = n_moves * 2 Kyunki har move ke liye 2 ints chahiye: src aur dst peg
    AlgoResult r = create_result(n_moves * 2);
    r.outputSize = n_moves * 2;
    strcpy(r.big_o, "O(2^n)");
 // Time measurement — rekursion ke liye bhi clock() use karo
    clock_t start = clock();
 // move_idx:: output array mein current write position 0 se shuru, har move ke baad +2 hota hai
    int move_idx = 0;
 // Solve karo! Peg A(0) se Peg C(2) pe, Peg B(1) helper
    hanoi_helper(n, 0, 2, 1, r.output, &move_idx, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
 // Memory: output array + struct itself
    r.memory_used = (long)(sizeof(AlgoResult) + (size_t)n_moves * 2 * sizeof(int));

    strcpy(r.status, "SUCCESS");
    return r;
}
