#include "../scae.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

AlgoResult prim_mst(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(0);
        strncpy(r.status, "INVALID_INPUT", sizeof(r.status) - 1);
        return r;
    }
    if (V == 1) {
        AlgoResult r = create_result(0);
        strncpy(r.status, "SUCCESS", sizeof(r.status) - 1);
        r.outputSize = 0;
        return r;
    }

    AlgoResult r = create_result((V - 1) * 2);

    int* key = (int*)malloc(V * sizeof(int));
    int* inMST = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX / 2;
        inMST[i] = 0;
        parent[i] = -1;
    }

    key[0] = 0;
    r.steps = 0;
    int edges_added = 0;

    clock_t start_time = clock();

    for (int count = 0; count < V; count++) {
        int min = INT_MAX / 2, u = -1;

        for (int v = 0; v < V; v++) {
            if (inMST[v] == 0 && key[v] < min) {
                min = key[v];
                u = v;
            }
        }

        if (min == INT_MAX / 2) {

            break;
        }

        inMST[u] = 1;

        for (int v = 0; v < V; v++) {
            int weight = adjMatrix[u * V + v];
            if (weight > 0 && inMST[v] == 0 && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                r.steps++;
            }
        }
    }

    clock_t end_time = clock();
    r.time_ns = (double)(end_time - start_time) * 1e9 / (double)CLOCKS_PER_SEC;
    strncpy(r.big_o, "O(V^2)", sizeof(r.big_o) - 1);

    int out_index = 0;
    int is_disconnected = 0;
    for (int i = 0; i < V; i++) {
        if (inMST[i] == 0) {
            is_disconnected = 1;
        }
    }

    for (int v = 1; v < V; v++) {
        if (parent[v] != -1) {
            r.output[out_index++] = parent[v];
            r.output[out_index++] = v;
            edges_added++;
        }
    }

    if (is_disconnected) {
        strncpy(r.status, "DISCONNECTED", sizeof(r.status) - 1);
        r.outputSize = edges_added * 2;
    } else {
        strncpy(r.status, "SUCCESS", sizeof(r.status) - 1);
        r.outputSize = (V - 1) * 2;
    }

    free(key);
    free(inMST);
    free(parent);

    return r;
}
