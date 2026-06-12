#include "../scae.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

typedef struct {
    int u;
    int v;
    int w;
} BFEdge;

AlgoResult bellman_ford(int* adjMatrix, int V, int src, int dst) {
    if (V <= 0 || src < 0 || src >= V || dst < 0 || dst >= V) {
        AlgoResult r = create_result(0);
        strncpy(r.status, "INVALID_INPUT", sizeof(r.status) - 1);
        r.status[sizeof(r.status) - 1] = '\0';
        return r;
    }

    AlgoResult r = create_result(V);
    strncpy(r.status, "SUCCESS", sizeof(r.status) - 1);
    r.status[sizeof(r.status) - 1] = '\0';
    r.steps = 0;
    strncpy(r.big_o, "O(VE)", sizeof(r.big_o) - 1);
    r.big_o[sizeof(r.big_o) - 1] = '\0';

    int maxEdges = V * V;
    BFEdge* edges = (BFEdge*)malloc(maxEdges * sizeof(BFEdge));
    int E = 0;

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            int weight = adjMatrix[i * V + j];
            if (weight != 0) {
                edges[E].u = i;
                edges[E].v = j;
                edges[E].w = weight;
                E++;
            }
        }
    }

    int* dist = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));

    r.memory_used = sizeof(AlgoResult) + maxEdges * sizeof(BFEdge) + V * sizeof(int) * 2;

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX / 2;
        parent[i] = -1;
    }
    dist[src] = 0;

    clock_t start_time = clock();

    for (int i = 0; i < V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int weight = edges[j].w;

            if (dist[u] != INT_MAX / 2) {
                r.steps++;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }


    int negativeCycle = 0;
    for (int j = 0; j < E; j++) {
        int u = edges[j].u;
        int v = edges[j].v;
        int weight = edges[j].w;
        if (dist[u] != INT_MAX / 2 && dist[u] + weight < dist[v]) {
            negativeCycle = 1;
            break;
        }
    }

    clock_t end_time = clock();
    r.time_ns = (double)(end_time - start_time) * 1e9 / (double)CLOCKS_PER_SEC;

    if (negativeCycle) {
        strncpy(r.status, "NEGATIVE_CYCLE", sizeof(r.status) - 1);
        r.status[sizeof(r.status) - 1] = '\0';
        r.output[0] = -1;
        r.outputSize = 1;
    } else if (dist[dst] == INT_MAX / 2) {
        strncpy(r.status, "NOT_REACHABLE", sizeof(r.status) - 1);
        r.status[sizeof(r.status) - 1] = '\0';
        r.output[0] = -1;
        r.outputSize = 1;
    } else {
        int curr = dst;
        int* tempPath = (int*)malloc(V * sizeof(int));
        int pathLen = 0;
        while (curr != -1) {
            tempPath[pathLen++] = curr;
            curr = parent[curr];
        }
        r.outputSize = pathLen;
        for (int i = 0; i < pathLen; i++) {
            r.output[i] = tempPath[pathLen - 1 - i];
        }
        free(tempPath);
    }

    free(edges);
    free(dist);
    free(parent);

    return r;
}
