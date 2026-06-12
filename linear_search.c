
#include "../scae.h"

AlgoResult linear_search(int* arr, int n, int key) {

    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");
    r.outputSize = 1;

    clock_t start = clock();

    for (int i = 0; i < n; i++) {

        r.steps++;

        if (arr[i] == key) {

            r.output[0] = i;
            strcpy(r.status, "FOUND");

            clock_t end = clock();
            r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
            r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));
            return r;
        }
    }


    r.output[0] = -1;
    strcpy(r.status, "NOT_FOUND");

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int));

    return r;
}

AlgoResult linear_search_sentinel(int* arr, int n, int key) {
    AlgoResult r = create_result(1);

    strcpy(r.big_o, "O(n)");

    r.outputSize = 1;

    clock_t start = clock();

    int last_val = arr[n - 1];

    arr[n - 1] = key;

    int i = 0;
    while (arr[i] != key) {
        r.steps++;
        i++;
    }

    r.steps++;

    arr[n - 1] = last_val;

    if (i < n - 1 || last_val == key) {

        r.output[0] = i;
        strcpy(r.status, "FOUND");
    } else {

        r.output[0] = -1;
        strcpy(r.status, "NOT_FOUND");
    }

    clock_t end = clock();
    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + sizeof(int) * 2);

    return r;
}
