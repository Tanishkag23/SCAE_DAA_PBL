#include "../scae.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct Edge {
    int u, v, w;
};

// Comparison function for qsort
static int compare_edges(const void* a, const void* b) {
    return ((struct Edge*)a)->w - ((struct Edge*)b)->w;
}

// DSU path compression
// Cycle detection relies on this
static int dsu_find(int* parent, int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = dsu_find(parent, parent[i]);
}

// DSU union by rank
static void dsu_union(int* parent, int* rank, int x, int y) {
    int rootX = dsu_find(parent, x);
    int rootY = dsu_find(parent, y);
    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

AlgoResult kruskal_mst(int* adjMatrix, int V) {
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

    clock_t start_time = clock();
    
    // Extract edges from upper triangle (i<j)
    int maxEdges = V * (V - 1) / 2;
    struct Edge* edges = (struct Edge*)malloc(maxEdges * sizeof(struct Edge));
    int edgeCount = 0;
    
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            int weight = adjMatrix[i * V + j];
            if (weight != 0) {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                edges[edgeCount].w = weight;
                edgeCount++;
            }
        }
    }

    // Ye greedy sorting MST ka foundation hai! Sabse sasta rasta pehle check karenge.
    qsort(edges, edgeCount, sizeof(struct Edge), compare_edges);

    // DSU logic setup
    int* parent = (int*)malloc(V * sizeof(int));
    int* rank = (int*)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    AlgoResult r = create_result((V - 1) * 2);
    int edges_added = 0;
    int total_weight = 0;
    int out_index = 0;
    r.steps = 0;

    for (int i = 0; i < edgeCount; i++) {
        r.steps++; // Count step for every edge considered in loop
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].w;

        // DSU path compression for cycle detection
        int rootU = dsu_find(parent, u);
        int rootV = dsu_find(parent, v);

        if (rootU != rootV) {
            r.output[out_index++] = u;
            r.output[out_index++] = v;
            total_weight += w;
            // Union by rank lagakar graph merge karte hain bina height badhaye.
            dsu_union(parent, rank, rootU, rootV);
            edges_added++;
            if (edges_added == V - 1) {
                break;
            }
        }
    }

    clock_t end_time = clock();
    r.time_ns = (double)(end_time - start_time) * 1e9 / (double)CLOCKS_PER_SEC;
    strncpy(r.big_o, "O(E log E)", sizeof(r.big_o) - 1);
    
    if (edges_added < V - 1) {
        strncpy(r.status, "DISCONNECTED", sizeof(r.status) - 1);
        r.outputSize = edges_added * 2;
    } else {
        strncpy(r.status, "SUCCESS", sizeof(r.status) - 1);
        r.outputSize = (V - 1) * 2;
    }
    
    // Total MST weight is: total_weight
    
    free(edges);
    free(parent);
    free(rank);
    
    return r;
}
