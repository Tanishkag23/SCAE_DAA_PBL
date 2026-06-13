// tsp_brute: SCAE Smart City Algorithm Engine Unit 5 NP: Travelling Salesman Problem — Brute Force Patrol rout...
#include "../scae.h"

// Swap two ints — Heap's algorithm mein use hoga
static void tsp_swap(int* a, int* b) {
    int tmp = *a; *a = *b; *b = tmp;
}

// Check: a single tour and update best if cheaper perm = [city1, city2, 
static void check_tour(int* perm, int perm_len, int* adj, int V,
                       int* best_cost, int* best_path) {
    int cost = 0;
     // Edge 0 → perm[0]
    if (adj[0 * V + perm[0]] == 0) return; // No road!
    cost += adj[0 * V + perm[0]];
     // Middle edges
    for (int i = 0; i < perm_len - 1; i++) {
        if (adj[perm[i] * V + perm[i + 1]] == 0) return;
        cost += adj[perm[i] * V + perm[i + 1]];
    }
    
    // Edge perm[last] → 0 (return to base)
    if (adj[perm[perm_len - 1] * V + 0] == 0) return;
    cost += adj[perm[perm_len - 1] * V + 0];
    
    if (*best_cost == -1 || cost < *best_cost) {
        *best_cost = cost;
        if (best_path) {
            best_path[0] = 0;
            for (int i = 0; i < perm_len; i++) best_path[i + 1] = perm[i];
            best_path[perm_len + 1] = 0;
        }
    }
}

// tsp_brute: Heap's algorithm to generate all (V-1)! permutations
AlgoResult tsp_brute(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    // V! grows CATASTROPHICALLY — V>12 pe laptop bhi ruk jayega!
    if (V > 12) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n!)");
        return r;
    }

    // Edge case: single city, no travel needed
    if (V <= 1) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        r.steps = 0;
        strcpy(r.big_o, "O(n!)");
        strcpy(r.status, "SUCCESS");
        return r;
    }

    int perm_len = V - 1; // Cities to permute: 1..V-1 (0 is fixed start)
    int* perm = (int*)malloc(perm_len * sizeof(int));
    int* c    = (int*)calloc(perm_len, sizeof(int)); // Heap's state array
    int* best_path = (int*)malloc((V + 1) * sizeof(int));

    for (int i = 0; i < perm_len; i++) perm[i] = i + 1; // [1, 2, ..., V-1]

    int best_cost = -1;
    long steps = 0;

    clock_t start_clk = clock();
 // Check initial permutation
    steps++;
    check_tour(perm, perm_len, adjMatrix, V, &best_cost, best_path);
 // Heap's: iterative algorithm: generates (n-1)! - 1 more permutations Har swap ke baad ek nayi permutation ...
    int i = 0;
    while (i < perm_len) {
        if (c[i] < i) {
            if (i % 2 == 0) tsp_swap(&perm[0], &perm[i]);
            else             tsp_swap(&perm[c[i]], &perm[i]);

            steps++;
            check_tour(perm, perm_len, adjMatrix, V, &best_cost, best_path);

            c[i]++;
            i = 0;
        } else {
            c[i] = 0;
            i++;
        }
    }

    clock_t end_clk = clock();

    AlgoResult r = create_result(V + 1);
    r.output[0] = best_cost; // -1 if no valid tour found
    if (best_cost >= 0) {
        for (int j = 1; j <= V; j++) r.output[j] = best_path[j];
        strcpy(r.status, "SUCCESS");
    } else {
        r.output[0] = -1;
        strcpy(r.status, "NO_VALID_TOUR");
    }
    r.outputSize = V + 1;
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (V + 1 + perm_len * 2) * sizeof(int);
    strcpy(r.big_o, "O(n!)");

    free(perm);
    free(c);
    free(best_path);
    return r;
}
