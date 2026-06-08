
#ifndef SCAE_H
#define SCAE_H

// ============================================================
//   SCAE — Smart City Algorithm Engine
//   Yeh header file poore project ka dil hai!
//   Sabhi structs, helpers, aur definitions yahan milenge.
// ============================================================

#include <stdio.h>    // printf, scanf wagera ke liye
#include <stdlib.h>   // malloc, free — memory ke raja
#include <string.h>   // strcpy, memset — strings ko sambhalo
#include <time.h>     // clock() se time measure karo
#include <limits.h>   // INT_MAX wagera — infinity ka desi jugaad
#include <math.h>     // sqrt, log — mathematical magic ke liye

// ============================================================
//   STRUCT: Edge
//   Yeh ek edge hai city graph mein — do locations ke beech
//   ek road. Weight = distance ya cost, active = road khuli hai
//   ya band (accident, maintenance, etc.)
// ============================================================
typedef struct {
    int src;     // Source node — yahan se road shuru hoti hai
    int dst;     // Destination node — yahan pe road khatam hoti hai
    int weight;  // Road ki cost ya distance (traffic bhi factor ho sakta hai!)
    int active;  // 1 = road khuli hai, 0 = road band hai (pothole? VIP convoy? 😅)
} Edge;

// ============================================================
//   STRUCT: Graph
//   Poora city ka map! Vertices = locations (signals, hubs),
//   Edges = roads. adjMatrix se O(1) mein check karo ki do
//   nodes connected hain ya nahi — Bellman-Ford, Dijkstra
//   dono ko yahi chahiye.
// ============================================================
typedef struct {
    int    V;          // Number of vertices — kitne locations hain city mein
    int    E;          // Number of edges — kitni roads hain total
    Edge*  edges;      // Flat array of all edges — saari roads ek jagah
    int*   adjMatrix;  // 1D adjacency matrix, size V*V, row-major order
                       // adjMatrix[i*V + j] = weight of edge i→j (0 if no edge)
                       // 2D array ka desi jugaad — cache-friendly bhi hai!
} Graph;

// ============================================================
//   STRUCT: Complaint
//   Citizen ne complaint dali — road kharab hai, light nahi
//   jal rahi, etc. Urgency 1-10 scale pe hai: 10 = EMERGENCY!
//   timestamp se hum dekhte hain kitni purani complaint hai.
// ============================================================
typedef struct {
    int  id;             // Unique complaint ID — har complaint ka alag number
    int  urgency;        // 1 (chai ki baat) se 10 (CITY ON FIRE!) tak
    int  timestamp;      // Unix timestamp — kab aayi complaint (seconds since epoch)
    char category[50];   // Complaint type: "Road", "Water", "Electricity", etc.
                         // 50 chars enough hai — "Bhaiya bijli nahi hai" fit ho jata hai 😄
} Complaint;

// ============================================================
//   STRUCT: AlgoResult
//   AlgoResult mein saare algorithms ka output aata hai —
//   ctypes se yahi struct Python ko milega! Har algorithm
//   (Dijkstra, Bellman-Ford, BFS, Sorting...) yahi return karta
//   hai taaki frontend ko ek consistent format mile.
// ============================================================
typedef struct {
    int*   output;       // Heap-allocated result array — path, sorted order, distances...
                         // Algorithm ke hisaab se meaning badlega, par array toh yahi hai
    int    outputSize;   // output array mein kitne elements hain — bounds check ke liye ZARURI
    long   steps;        // Actual operation count — kitne steps lage algorithm ko?
                         // Yeh theoretical nahi, actual count hai — performance proof!
    double time_ns;      // Execution time in nanoseconds — Python dashboard pe dikhega
                         // nanoseconds isliye ki fast algorithms milliseconds mein bhi nahi dikhte
    char   big_o[20];    // Big-O notation string — e.g. "O(log n)", "O(V+E)", "O(n^2)"
                         // 20 chars mein "O(V * E)" aaram se fit ho jata hai
    long   memory_used;  // Bytes used by this algorithm — memory footprint track karo
                         // Embedded systems ke liye yeh bahut important hai!
    char   status[50];   // Result status — "SUCCESS", "NOT_FOUND", "NEGATIVE_CYCLE", etc.
                         // Python side pe is string ko check karke UI update hoga
} AlgoResult;

// ============================================================
//   HELPER: create_result
//   Output array banao given size ka, baaki saare fields zero
//   karo, aur status "SUCCESS" set karo — fresh start!
//   Har algorithm shuru mein yahi call karega.
//
//   NOTE: Agar malloc fail ho gaya toh output NULL rahega —
//   caller ko check karna chahiye! (prod mein toh zarur karo)
// ============================================================
static inline AlgoResult create_result(int size) {
    AlgoResult r;

    // Pehle poora struct zero karo — garbage values se bachao!
    memset(&r, 0, sizeof(AlgoResult));

    // Output array ke liye memory maango — yeh heap pe banega
    if (size > 0) {
        r.output = (int*)malloc(size * sizeof(int));
        if (r.output != NULL) {
            // Array ko zero se initialize karo — undefined behaviour se daro mat
            memset(r.output, 0, size * sizeof(int));
        }
        // Agar malloc fail hua toh r.output = NULL — caller handle karega
    } else {
        r.output = NULL;  // Size 0 hai? Array ki zarurat nahi — NULL rakho
    }

    r.outputSize = size;  // Kitne elements ke liye memory li — yaad rakho!

    // Default status: "SUCCESS" — algorithm ke baad zarurat ho toh badal lena
    strcpy(r.status, "SUCCESS");

    // steps, time_ns, memory_used, big_o — sab zero hain memset se
    // Algorithm khud in fields ko fill karega execution ke baad

    return r;  // Stack pe return — struct copy hoti hai (small enough hai)
}

// ============================================================
//   HELPER: free_result
//   Memory free karo — leak mat karo! Yeh function call karna
//   BHOOL MAT, warna RAM bhart jayega aur city ka dashboard
//   crash ho jayega. Double-free se bachne ke liye output ko
//   NULL set kar dete hain free ke baad — safe hai!
// ============================================================
static inline void free_result(AlgoResult* r) {
    if (r == NULL) {
        return;  // NULL pointer pe kaam mat karo — segfault aayega!
    }

    if (r->output != NULL) {
        free(r->output);  // Heap memory wapas do OS ko — city ko RAM chahiye!
        r->output = NULL; // NULL karo taaki double-free na ho — ek baar aur free kiya toh UB!
    }

    // outputSize bhi zero karo — stale size se galat loop na chale
    r->outputSize = 0;
}
// ==============================================================
//   FUNCTION DECLARATIONS — Unit 1: Searching
//   Yeh prototypes hain — test files aur bridge.py ko pata
//   chalega ki functions exist karte hain bina .c file include
//   kiye. Har naye .c file ke functions yahan declare honge!
// ==============================================================

// Binary Search — do flavors: iterative (safe) aur recursive (elegant)
AlgoResult binary_search_iterative(int* arr, int n, int key);
AlgoResult binary_search_recursive(int* arr, int n, int key);

// Unit 1 — Linear Search
// Classic scan aur sentinel optimization — unsorted arrays ke liye
// Sentinel trick: last element temporarily key se replace karo,
// bounds check bachao, phir original restore karo. Jugaad hai, kaam karta hai!
AlgoResult linear_search(int* arr, int n, int key);
AlgoResult linear_search_sentinel(int* arr, int n, int key);

// Unit 1 — Tower of Hanoi
// City crane simulation: n disks ko peg A(0) se peg C(2) pe move karo
// Output: flat array of (src, dst) pairs — 2^n - 1 moves total
// n > 20 pe "TOO_LARGE" status aayega — memory protect karne ke liye
AlgoResult tower_hanoi(int n);

// Unit 1 — Fibonacci
// Sensor cascade prediction — fib(n) sensors trigger after n steps
// Naive: O(2^n) — pure recursion, overlapping subproblems bar bar compute
// Memo:  O(n)   — top-down DP, cache miss pe hi compute, hit pe free!
// n > 40 naive ke liye TOO_LARGE, n > 80 memo ke liye TOO_LARGE
AlgoResult fibonacci_naive(int n);
AlgoResult fibonacci_memo(int n);

// ==============================================================
//   Unit 2 — Sorting Algorithms
//   Complaint prioritization, traffic management, resource allocation.
//   Sabhi sorts ek COPY pe kaam karte hain — original array safe rehta hai!
//   Output: r.output = sorted copy, r.outputSize = n
// ==============================================================

// O(n^2) sorts — simple, in-place (copy pe), educational
AlgoResult bubble_sort(int* arr, int n);     // Early-exit optimized
AlgoResult insertion_sort(int* arr, int n);  // Online algorithm, great for nearly-sorted
AlgoResult selection_sort(int* arr, int n);  // Minimum swaps, always O(n^2)
AlgoResult counting_sort(int* arr, int n);   // O(n+k), non-comparison, k<=100000

// O(n log n) sorts — production-grade sorting
AlgoResult merge_sort(int* arr, int n);      // Stable, divide & conquer, extra O(n) space
AlgoResult quick_sort(int* arr, int n);      // Randomized pivot, in-place, avg O(n log n)
AlgoResult heap_sort(int* arr, int n);       // In-place, max-heap, guaranteed O(n log n)

// Unit 3 — Graph Algorithms
// ADJACENCY MATRIX representation for simplicity and ctypes compatibility
AlgoResult bfs_traversal(int* adjMatrix, int V, int src);
AlgoResult bfs_shortest_path_hops(int* adjMatrix, int V, int src, int dst);
AlgoResult dfs_traversal(int* adjMatrix, int V, int src);
AlgoResult has_cycle(int* adjMatrix, int V);
AlgoResult count_components(int* adjMatrix, int V);

AlgoResult dijkstra(int* adjMatrix, int V, int src, int dst);
AlgoResult bellman_ford(int* adjMatrix, int V, int src, int dst);
AlgoResult floyd_warshall(int* adjMatrix, int V);
AlgoResult kruskal_mst(int* adjMatrix, int V);
AlgoResult prim_mst(int* adjMatrix, int V);

#endif /* SCAE_H */
