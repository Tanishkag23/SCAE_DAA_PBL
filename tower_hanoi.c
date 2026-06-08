// ==============================================================
//   tower_hanoi.c — SCAE Smart City Algorithm Engine
//   Tower of Hanoi: n disks ko peg A se peg C pe move karo
//   (peg B ko temporary helper ki tarah use karo)
//
//   SCAE Context:
//   Yeh algorithm ek crane system simulate karta hai jo city ke
//   3 construction sites (A, B, C) ke beech materials move karta hai.
//   Jitne zyada disks (materials), utni zyada crane trips — exponential!
//
//   Output format:
//   output[i*2]     = move i ka source peg  (0=A, 1=B, 2=C)
//   output[i*2 + 1] = move i ka dest peg    (0=A, 1=B, 2=C)
// ==============================================================

#include "../scae.h"


// ==============================================================
//   INTERNAL HELPER: hanoi_helper
//   Yeh recursive function asli kaam karta hai — wrapper usse
//   call karta hai aur result pack karta hai.
//
//   PEG ENCODING (0=A, 1=B, 2=C) — int use karne ka reason:
//   Hum char 'A','B','C' bhi use kar sakte the, lekin int isliye
//   choose kiya kyunki:
//     1. output array int* hai — directly store ho jata hai
//     2. ctypes se Python mein int directly readable hota hai
//     3. Switch case ya array indexing mein int zyada convenient
//   Bas ek mental mapping yaad rakho: 0=A, 1=B, 2=C. Simple!
//
//   Parameters:
//     n         — kitne disks move karne hain is subproblem mein
//     src       — source peg (yahan se disks uthenge)
//     dst       — destination peg (yahan pe disks rakhenge)
//     aux       — auxiliary peg (temporary helper peg)
//     output    — moves store karne ka array (caller ne allocate kiya)
//     move_idx  — pointer to current write position in output
//                 (pointer isliye ki recursive calls mein share ho)
//     steps     — pointer to step counter (same reason as move_idx)
// ==============================================================
static void hanoi_helper(int n, int src, int dst, int aux,
                          int* output, int* move_idx, long* steps) {
    // Base case: 0 disks = kuch karna nahi — return!
    // Yeh recursion ka anchor hai — iske bina infinite recursion hogi
    if (n <= 0) return;

    // STEP 1: Upar ke (n-1) disks ko src se aux pe move karo
    // dst ko temporary helper use karo
    // Yeh sub-problem khud apne aap solve hota hai — recursion ka mazaa!
    hanoi_helper(n - 1, src, aux, dst, output, move_idx, steps);

    // STEP 2: Sabse bada (bottom) disk seedha src se dst pe move karo
    // Yeh ACTUAL move hai — output mein record karo!
    //
    // move_idx POINTER ka kaam:
    // Agar simple int hota (not pointer), toh recursive calls mein
    // value copy hoti — next position track hi nahi hota!
    // Pointer se saari recursive calls ek hi counter share karte hain.
    // Jab ek call output[4] aur output[5] write karta hai, agla call
    // automatically output[6] se shuru karta hai. Seedha aur clean!
    output[*move_idx]     = src;  // Source peg ka int code
    output[*move_idx + 1] = dst;  // Destination peg ka int code
    *move_idx += 2;               // Next move ke liye 2 positions aage badho
    (*steps)++;                   // Ek disk move = ek step

    // STEP 3: Upar ke (n-1) disks ko aux se dst pe move karo
    // src ko ab helper use karo
    // Yeh Step 1 ka "mirror" hai — recursion symmetrical hai!
    hanoi_helper(n - 1, aux, dst, src, output, move_idx, steps);
}


// ==============================================================
//   FUNCTION: tower_hanoi
//   Public function — n disks ke liye Tower of Hanoi solve karo
//   aur saare moves AlgoResult mein pack karke return karo.
//
//   2^n - 1 MOVES KYU HOTE HAIN? (Recursive reasoning):
//   T(n) = moves for n disks
//   T(0) = 0  (no disks, no moves)
//   T(n) = T(n-1) + 1 + T(n-1)  (ek subproblem + 1 big move + doosra subproblem)
//        = 2 * T(n-1) + 1
//
//   Solve karo:
//   T(1) = 2*T(0) + 1 = 1
//   T(2) = 2*T(1) + 1 = 3
//   T(3) = 2*T(2) + 1 = 7
//   T(n) = 2^n - 1    ← mathematical closed form!
//
//   Yeh MINIMUM possible moves hain — koi shortcut nahi!
//   Mathematically prove ho chuka hai ki 2^n - 1 se kam mein
//   Tower of Hanoi solve nahi ho sakta.
//
//   N > 20 LIMIT KYU RAKHA?
//   n=20  → 2^20 - 1 = 1,048,575 moves → ~4 MB output (reasonable)
//   n=25  → 2^25 - 1 = 33,554,431 moves → ~128 MB (zyada hai!)
//   n=30  → 2^30 - 1 → 4 GB+ (system crash ho sakta hai!)
//   SCAE ek city management system hai, n=20 se zyada physically
//   meaningful bhi nahi hai — 1 million crane moves kaun karega? 😄
//
//   Parameters:
//     n — number of disks (0 to 20)
//
//   Returns: AlgoResult with all moves stored in output array
// ==============================================================
AlgoResult tower_hanoi(int n) {

    // Edge case 1: n <= 0 — koi disk nahi, koi move nahi
    // Empty result return karo — valid input hai, steps=0
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }

    // Edge case 2: n > 20 — bahut bada hai, memory blow ho jayegi
    // Status "TOO_LARGE" set karo aur empty result return karo
    // Caller ko pata chalega ki input valid range se bahar tha
    if (n > 20) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(2^n)");
        r.steps = 0;
        return r;
    }

    // Total moves calculate karo: 2^n - 1
    // (1 << n) = 2^n using bit shift — fast aur clean!
    // Bit shift isliye use kiya: pow(2, n) float return karta hai,
    // integer ke liye bit shift perfect hai
    int n_moves = (1 << n) - 1;

    // Output array size = n_moves * 2
    // Kyunki har move ke liye 2 ints chahiye: src aur dst peg
    AlgoResult r = create_result(n_moves * 2);
    r.outputSize = n_moves * 2;
    strcpy(r.big_o, "O(2^n)");

    // Time measurement — rekursion ke liye bhi clock() use karo
    clock_t start = clock();

    // move_idx: output array mein current write position
    // 0 se shuru, har move ke baad +2 hota hai
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
