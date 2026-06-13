// hash_chain: SCAE Smart City Algorithm Engine Unit 5: Hashing — Chaining (Separate Chaining) Citizen lookup sy...
#include "../scae.h"

// Internal: helper: build chaining hash table as flat 2D array table[bucket * n + chain_pos] = key stored the...
static int* build_chain_table(int* keys, int n, int table_size, long* collisions_out) {
    long sz = (long)table_size * n;
    int* table = (int*)malloc(sz * sizeof(int)); // -1 matlab empty slot — memset -1 works because -1 = 0xFF...FF in two's complement
    memset(table, -1, sz * sizeof(int));

    long collisions = 0;
    for (int i = 0; i < n; i++) {
        // Hash function: k % table_size (handle negatives too)
        int bucket = ((keys[i] % table_size) + table_size) % table_size;
 // Find next free chain position in this bucket
        int chain_pos = 0;
        while (chain_pos < n && table[(long)bucket * n + chain_pos] != -1) {
            chain_pos++;
        }
        if (chain_pos > 0) collisions++; // Collision! bucket mein pehle se kuch hai

        if (chain_pos < n) {
            table[(long)bucket * n + chain_pos] = keys[i];
        }
    }

    *collisions_out = collisions;
    return table;
}

// hash_chain_build: Build hash table using chaining — return the flat table state
AlgoResult hash_chain_build(int* keys, int n, int table_size) {
    if (n <= 0 || table_size <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    if (table_size > 10000) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        return r;
    }

    clock_t start_clk = clock();
    long collisions = 0;
    int* table = build_chain_table(keys, n, table_size, &collisions);
    clock_t end_clk = clock();

    long sz = (long)table_size * n;
    AlgoResult r = create_result((int)sz);
    memcpy(r.output, table, sz * sizeof(int));
    r.outputSize = (int)sz;
    r.steps = collisions; // Collisions = steps = chaos indicator!
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + sz * sizeof(int);
    strcpy(r.big_o, "O(n)");
    strcpy(r.status, "SUCCESS");

    free(table);
    return r;
}

// hash_chain_search: Search key in chaining hash table — O(1) average, O(n) worst
AlgoResult hash_chain_search(int* keys, int n, int table_size, int search_key) {
    if (n <= 0 || table_size <= 0) {
        AlgoResult r = create_result(2);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    if (table_size > 10000) {
        AlgoResult r = create_result(2);
        strcpy(r.status, "TOO_LARGE");
        return r;
    }

    long dummy = 0;
    int* table = build_chain_table(keys, n, table_size, &dummy);
 // Compute bucket for search_key
    int bucket = ((search_key % table_size) + table_size) % table_size;

    clock_t start_clk = clock();
    long steps = 0;
    int found_pos = -1;

    for (int i = 0; i < n; i++) {
        steps++;
        if (table[(long)bucket * n + i] == -1) break; // End of chain — nahi mila
        if (table[(long)bucket * n + i] == search_key) {
            found_pos = i;
            break;
        }
    }

    clock_t end_clk = clock();

    AlgoResult r = create_result(2);
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + (long)table_size * n * sizeof(int);
    strcpy(r.big_o, "O(1)");
    r.outputSize = 2;

    if (found_pos >= 0) {
        r.output[0] = bucket;
        r.output[1] = found_pos;
        strcpy(r.status, "FOUND");
    } else {
        r.output[0] = -1;
        r.output[1] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    free(table);
    return r;
}
