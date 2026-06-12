
#include "../scae.h"

AlgoResult dfs_traversal(int* adjMatrix, int V, int src) {
    if (V <= 0 || src < 0 || src >= V) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    AlgoResult r = create_result(V);
    r.outputSize = 0;
    strcpy(r.big_o, "O(V+E)");

    int* stack = (int*)malloc(V * sizeof(int));
    int* visited = (int*)calloc(V, sizeof(int));
    int top = -1;

    clock_t start = clock();

    stack[++top] = src;

    while (top >= 0) {
        int u = stack[top--];
        r.steps++;

        if (!visited[u]) {
            visited[u] = 1;
            r.output[r.outputSize++] = u;

            for (int v = V - 1; v >= 0; v--) {
                if (adjMatrix[u * V + v] > 0 && !visited[v]) {
                    stack[++top] = v;
                }
            }
        }
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + 3 * V * sizeof(int);
    strcpy(r.status, "SUCCESS");

    free(stack);
    free(visited);
    return r;
}

static int dfs_cycle_helper(int* adjMatrix, int V, int u, int* visited, long* steps) {
    visited[u] = 1;
    (*steps)++;

    for (int v = 0; v < V; v++) {
        if (adjMatrix[u * V + v] > 0) {
            if (visited[v] == 1) {

                return 1;
            } else if (visited[v] == 0) {
                if (dfs_cycle_helper(adjMatrix, V, v, visited, steps)) {
                    return 1;
                }
            }
        }
    }
    visited[u] = 2;
    return 0;
}

AlgoResult has_cycle(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        strcpy(r.status, "NO_CYCLE");
        return r;
    }

    AlgoResult r = create_result(1);
    strcpy(r.big_o, "O(V+E)");

    int* visited = (int*)calloc(V, sizeof(int));
    long steps = 0;

    clock_t start = clock();

    int cycle_found = 0;
    for (int i = 0; i < V; i++) {
        if (visited[i] == 0) {
            if (dfs_cycle_helper(adjMatrix, V, i, visited, &steps)) {
                cycle_found = 1;
                break;
            }
        }
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.steps = steps;
    r.memory_used = sizeof(AlgoResult) + V * sizeof(int);

    if (cycle_found) {
        r.output[0] = 1;
        strcpy(r.status, "CYCLE_FOUND");
    } else {
        r.output[0] = 0;
        strcpy(r.status, "NO_CYCLE");
    }
    r.outputSize = 1;

    free(visited);
    return r;
}

static void dfs_component_helper(int* adjMatrix, int V, int u, int* visited, long* steps) {
    visited[u] = 1;
    (*steps)++;

    for (int v = 0; v < V; v++) {

        if (adjMatrix[u * V + v] > 0 && !visited[v]) {
            dfs_component_helper(adjMatrix, V, v, visited, steps);
        }
    }
}

AlgoResult count_components(int* adjMatrix, int V) {
    if (V <= 0) {
        AlgoResult r = create_result(1);
        r.output[0] = 0;
        r.outputSize = 1;
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(1);
    strcpy(r.big_o, "O(V+E)");

    int* visited = (int*)calloc(V, sizeof(int));
    long steps = 0;
    int components = 0;

    clock_t start = clock();

    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            components++;
            dfs_component_helper(adjMatrix, V, i, visited, &steps);
        }
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.steps = steps;
    r.memory_used = sizeof(AlgoResult) + V * sizeof(int);

    r.output[0] = components;
    r.outputSize = 1;
    strcpy(r.status, "SUCCESS");

    free(visited);
    return r;
}
