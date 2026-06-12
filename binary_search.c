
#include "../scae.h"

AlgoResult binary_search_iterative(int* arr, int n, int key) {

    AlgoResult r = create_result(1);

    clock_t start = clock();

    int lo = 0;
    int hi = n - 1;

    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;

    while (lo <= hi) {

        int mid = lo + (hi - lo) / 2;

        r.steps++;

        if (arr[mid] == key) {

            r.output[0] = mid;
            strcpy(r.status, "FOUND");
            break;

        } else if (arr[mid] < key) {

            lo = mid + 1;

        } else {

            hi = mid - 1;
        }
    }


    if (strcmp(r.status, "FOUND") != 0) {
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }


    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}

static int bs_recursive_helper(int* arr, int lo, int hi, int key, long* steps) {

    if (lo > hi) {
        return -1;
    }


    (*steps)++;

    int mid = lo + (hi - lo) / 2;

    if (arr[mid] == key) {
        return mid;

    } else if (arr[mid] < key) {

        return bs_recursive_helper(arr, mid + 1, hi, key, steps);

    } else {

        return bs_recursive_helper(arr, lo, mid - 1, key, steps);
    }
}

AlgoResult binary_search_recursive(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(log n)");
    r.outputSize = 1;

    clock_t start = clock();

    int idx = bs_recursive_helper(arr, 0, n - 1, key, &r.steps);

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;

    if (idx >= 0) {
        r.output[0] = idx;
        strcpy(r.status, "FOUND");
    } else {
        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }


    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}
