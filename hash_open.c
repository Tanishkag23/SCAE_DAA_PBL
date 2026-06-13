// hash_open: SCAE Smart City Algorithm Engine Unit 5: Hashing — Open Addressing (Linear Probing) Zone mapping ...
#include "../scae.h"

// Internal: helper: build open-addressed table with linear probing Returns allocated array of size table_size...
static int* build_open_table(int* keys, int n, int table_size, long* probes_out, int* full_out) {
    int* table = (int*)malloc(table_size * sizeof(int));
    memset(table, -1, table_size * sizeof(int)); // -1 = empty slot

    long total_probes = 0;
    *full_out = 0;

    for (int i = 0; i < n; i++) {
        int start = ((keys[i] % table_size) + table_size) % table_size;
        int slot = start;
        int placed = 0;
 // Linear probe: slot, slot+1, slot+2, ... (circular)
        for (int p = 0; p < table_size; p++) {
            total_probes++;
            int try_slot = (slot + p) % table_size;
            if (table[try_slot] == -1) {
                // Free slot mila! — yahan rakh do
                table[try_slot] = keys[i];
                placed = 1;
                break;
            }
        }

        if (!placed) {
            *full_out = 1; // Table full ho gaya!
            break;
        }
    }

    *probes_out = total_probes;
    return table;
}

// hash_open_build: Linear probing hash table — return slot contents
AlgoResult hash_open_build(int* keys, int n, int table_size) {
    if (n <= 0 || table_size <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    // n: > table_size: mathematically impossible to fit all keys! Open addressing mein table_size >= n hon...
    if (n > table_size) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TABLE_OVERFLOW");
        strcpy(r.big_o, "O(n)");
        return r;
    }

    clock_t start_clk = clock();
    long probes = 0;
    int full = 0;
    int* table = build_open_table(keys, n, table_size, &probes, &full);
    clock_t end_clk = clock();

    if (!table) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "OOM");
        return r;
    }

    AlgoResult r = create_result(table_size);
    memcpy(r.output, table, table_size * sizeof(int));
    r.outputSize = table_size;
    r.steps = probes;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + table_size * sizeof(int);
    strcpy(r.big_o, "O(n)");

    if (full) {
        strcpy(r.status, "TABLE_FULL");
    } else {
        strcpy(r.status, "SUCCESS");
    }

    free(table);
    return r;
}

// hash_open_search: Search in linear-probing table — O(1) average
AlgoResult hash_open_search(int* keys, int n, int table_size, int search_key) {
    if (n <= 0 || table_size <= 0) {
        AlgoResult r = create_result(1);
        strcpy(r.status, "INVALID_INPUT");
        return r;
    }
    if (n > table_size) {
        AlgoResult r = create_result(1);
        strcpy(r.status, "TABLE_OVERFLOW");
        return r;
    }

    long dummy_probes = 0;
    int dummy_full = 0;
    int* table = build_open_table(keys, n, table_size, &dummy_probes, &dummy_full);

    if (!table) {
        AlgoResult r = create_result(1);
        strcpy(r.status, "OOM");
        return r;
    }

    int start = ((search_key % table_size) + table_size) % table_size;
    clock_t start_clk = clock();
    long steps = 0;
    int found_slot = -1;
 // Search with linear probing — stop at empty slot (key can't be beyond empty)
    for (int p = 0; p < table_size; p++) {
        steps++;
        int slot = (start + p) % table_size;
        if (table[slot] == -1) break;    // Empty slot — key nahi hai
        if (table[slot] == search_key) { // Found!
            found_slot = slot;
            break;
        }
    }

    clock_t end_clk = clock();
    AlgoResult r = create_result(1);
    r.steps = steps;
    r.time_ns = (double)(end_clk - start_clk) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = sizeof(AlgoResult) + table_size * sizeof(int);
    strcpy(r.big_o, "O(1)");
    r.outputSize = 1;
    r.output[0] = found_slot;

    if (found_slot >= 0) {
        strcpy(r.status, "FOUND");
    } else {
        strcpy(r.status, "NOT_FOUND");
    }

    free(table);
    return r;
}
