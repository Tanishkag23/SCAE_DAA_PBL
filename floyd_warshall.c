#include "../scae.h"
#include <limits.h>
#include <time.h>
#include <string.h>

AlgoResult floyd_warshall(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    if (V > 100) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        return r;
    }

    AlgoResult r = create_result(V * V);
    r.outputSize = V * V;
    r.memory_used = sizeof(AlgoResult) + V * V * sizeof(int);
    strcpy(r.big_o, "O(V^3)");

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                r.output[i * V + i] = 0;
            } else if (adjMatrix[i * V + j] != 0) {
                r.output[i * V + j] = adjMatrix[i * V + j];
            } else {
                r.output[i * V + j] = INT_MAX / 2;
            }
        }
    }

    clock_t start = clock();
     // DP: recurrence dist[i][j] = min(direct, via k) O(V^3) triple loop
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                r.steps++;
                if (r.output[i * V + k] + r.output[k * V + j] < r.output[i * V + j]) {
                    r.output[i * V + j] = r.output[i * V + k] + r.output[k * V + j];
                }
            }
        }
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    int negative_cycle = 0; // negative cycle detection via diagonal
    for (int i = 0; i < V; i++) {
        if (r.output[i * V + i] < 0) {
            negative_cycle = 1;
            break;
        }
    }

    // output matrix for admin all-pairs
    if (negative_cycle) {
        strcpy(r.status, "NEGATIVE_CYCLE");
    } else {
        strcpy(r.status, "SUCCESS");
    }

    return r;
}
