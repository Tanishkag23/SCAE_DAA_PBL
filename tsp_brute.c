
#include "../scae.h"

static void tsp_swap(int* a, int* b) {
    int tmp = *a; *a = *b; *b = tmp;
}

static void check_tour(int* perm, int perm_len, int* adj, int V,
                       int* best_cost, int* best_path) {
    int cost = 0;

    if (adj[0 * V + perm[0]] == 0) return;
    cost += adj[0 * V + perm[0]];

    for (int i = 0; i < perm_len - 1; i++) {
        if (adj[perm[i] * V + perm[i + 1]] == 0) return;
        cost += adj[perm[i] * V + perm[i + 1]];
    }


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

AlgoResult tsp_brute(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    if (V > 12) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n!)");
        return r;
    }


    if (V <= 1) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        r.steps = 0;
        strcpy(r.big_o, "O(n!)");
        strcpy(r.status, "SUCCESS");
        return r;
    }

    int perm_len = V - 1;
    int* perm = (int*)malloc(perm_len * sizeof(int));
    int* c    = (int*)calloc(perm_len, sizeof(int));
    int* best_path = (int*)malloc((V + 1) * sizeof(int));

    for (int i = 0; i < perm_len; i++) perm[i] = i + 1;

    int best_cost = -1;
    long steps = 0;

    clock_t start_clk = clock();

    steps++;
    check_tour(perm, perm_len, adjMatrix, V, &best_cost, best_path);

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
    r.output[0] = best_cost;
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
