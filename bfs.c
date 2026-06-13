// bfs: SCAE Smart City Algorithm Engine Unit 3: Graph Algorithms (Breadth First Search) BFS level-by-lev...
#include "../scae.h"

// FUNCTION: 1: bfs_traversal Standard BFS using a simple array-based queue
AlgoResult bfs_traversal(int* adjMatrix, int V, int src) {
    if (V <= 0 || src < 0 || src >= V) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    AlgoResult r = create_result(V); // max possible size
    r.outputSize = 0;
    strcpy(r.big_o, "O(V+E)");

    int* queue = (int*)malloc(V * sizeof(int));
    int* visited = (int*)calloc(V, sizeof(int));
    
    if (!queue || !visited) {
        if(queue) free(queue);
        if(visited) free(visited);
        strcpy(r.status, "OOM");
        return r;
    }

    int head = 0, tail = 0;

    clock_t start = clock();
 // Start BFS
    visited[src] = 1;
    queue[tail++] = src; // Enqueue

    while (head < tail) {
        int u = queue[head++]; // Dequeue
        r.steps++; // Count dequeue operation as one step
        r.output[r.outputSize++] = u;
 // Explore neighbors
        for (int v = 0; v < V; v++) {
            if (adjMatrix[u * V + v] > 0 && !visited[v]) {
                visited[v] = 1;
                queue[tail++] = v; // Enqueue neighbor
            }
        }
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + V * sizeof(int) + 2 * V * sizeof(int); // output + queue + visited
    strcpy(r.status, "SUCCESS");

    free(queue);
    free(visited);
    return r;
}

// FUNCTION: 2: bfs_shortest_path_hops Shortest path using BFS (in terms of hops/edges)
AlgoResult bfs_shortest_path_hops(int* adjMatrix, int V, int src, int dst) {
    if (V <= 0 || src < 0 || src >= V || dst < 0 || dst >= V) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    if (src == dst) {
        AlgoResult r = create_result(1);
        r.output[0] = src;
        r.outputSize = 1;
        r.steps = 0;
        strcpy(r.status, "SUCCESS");
        strcpy(r.big_o, "O(V+E)");
        return r;
    }

    int* queue = (int*)malloc(V * sizeof(int));
    int* dist = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        dist[i] = -1; // Unvisited / Not reachable
        parent[i] = -1;
    }

    int head = 0, tail = 0;
    long steps = 0;

    clock_t start = clock();

    dist[src] = 0;
    queue[tail++] = src;

    while (head < tail) {
        int u = queue[head++];

        for (int v = 0; v < V; v++) {
            if (adjMatrix[u * V + v] > 0) {
                steps++; // Edge relaxation step
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    queue[tail++] = v;
                    if (v == dst) {
                        // Shortest: path mill gaya! Hum BFS rok sakte hain par puri loop chalne di
                    }
                }
            }
        }
    }

    clock_t end = clock();
 // Reconstruct path
    AlgoResult r = create_result(V);
    r.steps = steps;
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    strcpy(r.big_o, "O(V+E)");
    r.memory_used = sizeof(AlgoResult) + 4 * V * sizeof(int);

    if (dist[dst] == -1) {
        strcpy(r.status, "NOT_REACHABLE");
        r.output[0] = -1;
        r.outputSize = 1;
    } else {
        strcpy(r.status, "SUCCESS"); // Path trace-back (reverse order)
        int curr = dst;
        int path_len = 0;
        int* temp_path = (int*)malloc(V * sizeof(int));
        while (curr != -1) {
            temp_path[path_len++] = curr;
            curr = parent[curr];
        }
        // Reverse it into output
        for (int i = 0; i < path_len; i++) {
            r.output[i] = temp_path[path_len - 1 - i];
        }
        r.outputSize = path_len;
        free(temp_path);
    }

    free(queue);
    free(dist);
    free(parent);

    return r;
}
