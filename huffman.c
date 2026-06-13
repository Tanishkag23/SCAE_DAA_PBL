// huffman: SCAE Smart City Algorithm Engine Unit 4: DP/Greedy — Huffman Encoding Sensor data compression — c...
#include "../scae.h"

// Huffman tree node — node pool mein store hote hain
typedef struct {
    int freq;       // frequency of this node (sum for internal)
    int char_idx;   // character index (-1 = internal node)
    int left;       // left child pool index (-1 = leaf)
    int right;      // right child pool index (-1 = leaf)
} HuffNode;
 // Min-heap helper: swap two heap positions
static void h_swap(int* heap, int a, int b) {
    int tmp = heap[a]; heap[a] = heap[b]; heap[b] = tmp;
}

// Sift down: heap[i] neeche le jao jab tak min-property satisfy ho
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

// Sift up: heap[i] upar le jao
static void h_sift_up(int* heap, int i, HuffNode* pool) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pool[heap[parent]].freq > pool[heap[i]].freq) {
            h_swap(heap, parent, i);
            i = parent;
        } else break;
    }
}

// Extract minimum frequency node
static int h_extract_min(int* heap, int* size, HuffNode* pool) {
    int min_node = heap[0];
    heap[0] = heap[--(*size)];
    h_sift_down(heap, *size, 0, pool);
    return min_node;
}

// Insert new node into heap
static void h_insert(int* heap, int* size, int node_idx, HuffNode* pool) {
    heap[(*size)] = node_idx;
    h_sift_up(heap, (*size), pool);
    (*size)++;
}

// DFS: to calculate code lengths (depth of each leaf) lengths[char_idx] = depth = code length for that c...
static void calc_depths(HuffNode* pool, int node, int depth, int* lengths) {
    if (pool[node].char_idx >= 0) {
        // Leaf node — yeh actual character hai, depth = code length
        lengths[pool[node].char_idx] = depth;
        return;
    }
    if (pool[node].left >= 0)  calc_depths(pool, pool[node].left,  depth + 1, lengths);
    if (pool[node].right >= 0) calc_depths(pool, pool[node].right, depth + 1, lengths);
}

// huffman_encode: Build Huffman tree, return code lengths for each character
AlgoResult huffman_encode(int* freq, int n) {
    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }

    // Special: case: single character Sirf ek character hai — uska code length = 1 (ek bit hi kaafi)
    if (n == 1) {
        AlgoResult r = create_result(2);
        r.output[0] = 1;          // code length for char 0
        r.output[1] = freq[0];    // total bits = freq * 1
        r.outputSize = 2;
        r.steps = 0;
        strcpy(r.big_o, "O(n log n)");
        strcpy(r.status, "SUCCESS");
        return r;
    }

    // Node pool: max 2*n-1 nodes (n leaves + n-1 internal)
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
 // Step: 1: Create leaf nodes for each character Min-heap mein saare characters daalo sorted by frequency
    for (int i = 0; i < n; i++) {
        pool[pool_size].freq     = freq[i];
        pool[pool_size].char_idx = i;
        pool[pool_size].left     = -1;
        pool[pool_size].right    = -1;
        h_insert(heap, &heap_size, pool_size, pool);
        pool_size++;
    }

    long steps = 0;
 // Step: 2: Build Huffman tree — greedily merge two smallest nodes Jab tak heap mein ek se zyada nodes hai...
    while (heap_size > 1) {
        int u = h_extract_min(heap, &heap_size, pool); steps++;
        int v = h_extract_min(heap, &heap_size, pool); steps++;
 // Create internal node: combined frequency, no character
        pool[pool_size].freq     = pool[u].freq + pool[v].freq;
        pool[pool_size].char_idx = -1; // internal node — koi character nahi
        pool[pool_size].left     = u;
        pool[pool_size].right    = v;
        h_insert(heap, &heap_size, pool_size, pool); steps++;
        pool_size++;
    }

    clock_t end_clk = clock();
 // Root is the only remaining element
    int root = heap[0];
 // Step: 3: DFS to get code lengths Root se depth 0, leaf tak depth = code length
    calc_depths(pool, root, 0, lengths);
 // Step: 4: Build output output[0
    AlgoResult r = create_result(n + 1);
    int total_bits = 0;
    for (int i = 0; i < n; i++) {
        r.output[i] = lengths[i];
        total_bits += freq[i] * lengths[i]; // sum(freq * length) = total bits needed
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
