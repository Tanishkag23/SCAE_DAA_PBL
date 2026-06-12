#include "../scae.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

AlgoResult dijkstra(int* adjMatrix, int V, int src, int dst) {
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
    strncpy(r.big_o, "O(V^2)", sizeof(r.big_o) - 1);
    r.big_o[sizeof(r.big_o) - 1] = '\0';

    int* dist = (int*)malloc(V * sizeof(int));
    int* visited = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));

    r.memory_used = sizeof(AlgoResult) + V * sizeof(int) * 3;

    for (int i = 0; i < V; i++) {

        dist[i] = INT_MAX / 2;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    clock_t start_time = clock();

    for (int count = 0; count < V; count++) {
        int min = INT_MAX / 2;
        int u = -1;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1 || dist[u] == INT_MAX / 2) break;

        visited[u] = 1;

        for (int v = 0; v < V; v++) {
            int weight = adjMatrix[u * V + v];
            if (weight > 0 && !visited[v]) {
                r.steps++;
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    clock_t end_time = clock();
    r.time_ns = (double)(end_time - start_time) * 1e9 / (double)CLOCKS_PER_SEC;

    if (dist[dst] == INT_MAX / 2) {
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

    free(dist);
    free(visited);
    free(parent);

    return r;
}
