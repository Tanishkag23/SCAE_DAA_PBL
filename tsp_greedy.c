
#include "../scae.h"

AlgoResult tsp_greedy(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }


    if (V <= 1) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        r.steps = 0;
        strcpy(r.big_o, "O(n^2)");
        strcpy(r.status, "SUCCESS");
        return r;
    }

    int* visited = (int*)calloc(V, sizeof(int));
    int* path    = (int*)malloc((V + 1) * sizeof(int));

    if (!visited || !path) {
        if (visited) free(visited);
        if (path) free(path);
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    path[0] = 0;
    visited[0] = 1;
    int current = 0;
    int total_cost = 0;
    int valid = 1;
    long steps = 0;

    clock_t start_clk = clock();

    for (int step = 1; step < V; step++) {
        int nearest = -1;
        int min_dist = INT_MAX;

        for (int j = 0; j < V; j++) {
            steps++;
            if (!visited[j] && adjMatrix[current * V + j] > 0) {
                if (adjMatrix[current * V + j] < min_dist) {
                    min_dist = adjMatrix[current * V + j];
                    nearest = j;
                }
            }
        }

        if (nearest == -1) {

            valid = 0;
            break;
        }

        path[step] = nearest;
        visited[nearest] = 1;
        total_cost += min_dist;
        current = nearest;
    }


    if (valid) {
        steps++;
        if (adjMatrix[current * V + 0] == 0) {
            valid = 0;
        } else {
            total_cost += adjMatrix[current * V + 0];
            path[V] = 0;
        }
    }

    clock_t end_clk = clock();

    AlgoResult r = create_result(V + 1);
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (V + 1 + V) * sizeof(int);
    strcpy(r.big_o, "O(n^2)");
    r.outputSize = V + 1;

    if (!valid) {
        r.output[0] = -1;
        strcpy(r.status, "NO_HAMILTONIAN_PATH");
    } else {
        r.output[0] = total_cost;
        for (int i = 1; i <= V; i++) r.output[i] = path[i];
        strcpy(r.status, "SUCCESS");
    }

    free(visited);
    free(path);
    return r;
}
