
#include "../scae.h"

typedef struct {
    int freq;
    int char_idx;
    int left;
    int right;
} HuffNode;

static void h_swap(int* heap, int a, int b) {
    int tmp = heap[a]; heap[a] = heap[b]; heap[b] = tmp;
}

static void h_sift_down(int* heap, int size, int i, HuffNode* pool) {
    while (1) {
        int smallest = i;
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < size && pool[heap[l]].freq < pool[heap[smallest]].freq) smallest = l;
        if (r < size && pool[heap[r]].freq < pool[heap[smallest]].freq) smallest = r;
        if (smallest == i) break;
        h_swap(heap, i, smallest);
        i = smallest;
    }
}

static void h_sift_up(int* heap, int i, HuffNode* pool) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pool[heap[parent]].freq > pool[heap[i]].freq) {
            h_swap(heap, parent, i);
            i = parent;
        } else break;
    }
}

static int h_extract_min(int* heap, int* size, HuffNode* pool) {
    int min_node = heap[0];
    heap[0] = heap[--(*size)];
    h_sift_down(heap, *size, 0, pool);
    return min_node;
}

static void h_insert(int* heap, int* size, int node_idx, HuffNode* pool) {
    heap[(*size)] = node_idx;
    h_sift_up(heap, (*size), pool);
    (*size)++;
}

static void calc_depths(HuffNode* pool, int node, int depth, int* lengths) {
    if (pool[node].char_idx >= 0) {

        lengths[pool[node].char_idx] = depth;
        return;
    }
    if (pool[node].left >= 0)  calc_depths(pool, pool[node].left,  depth + 1, lengths);
    if (pool[node].right >= 0) calc_depths(pool, pool[node].right, depth + 1, lengths);
}

AlgoResult huffman_encode(int* freq, int n) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }


    if (n == 1) {
        AlgoResult r = create_result(2);
        r.output[0] = 1;
        r.output[1] = freq[0];
        r.outputSize = 2;
        r.steps = 0;
        strcpy(r.big_o, "O(n log n)");
        strcpy(r.status, "SUCCESS");
        return r;
    }


    HuffNode* pool = (HuffNode*)malloc(2 * n * sizeof(HuffNode));
    int* heap      = (int*)malloc(2 * n * sizeof(int));
    int* lengths   = (int*)calloc(n, sizeof(int));

    if (!pool || !heap || !lengths) {
        if (pool) free(pool);
        if (heap) free(heap);
        if (lengths) free(lengths);
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    int pool_size = 0, heap_size = 0;

    clock_t start_clk = clock();

    for (int i = 0; i < n; i++) {
        pool[pool_size].freq     = freq[i];
        pool[pool_size].char_idx = i;
        pool[pool_size].left     = -1;
        pool[pool_size].right    = -1;
        h_insert(heap, &heap_size, pool_size, pool);
        pool_size++;
    }

    long steps = 0;

    while (heap_size > 1) {
        int u = h_extract_min(heap, &heap_size, pool); steps++;
        int v = h_extract_min(heap, &heap_size, pool); steps++;

        pool[pool_size].freq     = pool[u].freq + pool[v].freq;
        pool[pool_size].char_idx = -1;
        pool[pool_size].left     = u;
        pool[pool_size].right    = v;
        h_insert(heap, &heap_size, pool_size, pool); steps++;
        pool_size++;
    }

    clock_t end_clk = clock();

    int root = heap[0];

    calc_depths(pool, root, 0, lengths);

    AlgoResult r = create_result(n + 1);
    int total_bits = 0;
    for (int i = 0; i < n; i++) {
        r.output[i] = lengths[i];
        total_bits += freq[i] * lengths[i];
    }
    r.output[n] = total_bits;
    r.outputSize = n + 1;
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + 2 * n * sizeof(HuffNode) + 2 * n * sizeof(int);
    strcpy(r.big_o, "O(n log n)");
    strcpy(r.status, "SUCCESS");

    free(pool);
    free(heap);
    free(lengths);
    return r;
}
