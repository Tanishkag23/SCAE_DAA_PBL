

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult counting_sort(int* arr, int n) {


    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }



    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }



    if (max_val > 100000) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "TOO_LARGE");
        strcpy(r.big_o, "O(n+k)");
        return r;
    }

    AlgoResult r = create_result(n);
    strcpy(r.big_o, "O(n+k)");
    r.outputSize = n;


    int* count = (int*)calloc(max_val + 1, sizeof(int));
    if (!count) {

        strcpy(r.status, "ALLOC_FAILED");
        return r;
    }




    clock_t start = clock();


    for (int i = 0; i < n; i++) {
        count[arr[i]]++;
        r.steps++;
    }



    for (int i = 1; i <= max_val; i++) {
        count[i] += count[i - 1];
        r.steps++;
    }



    for (int i = n - 1; i >= 0; i--) {
        r.output[--count[arr[i]]] = arr[i];
        r.steps++;
    }

    clock_t end = clock();



    free(count);
    count = NULL;

    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.memory_used = (long)(sizeof(AlgoResult) + (long)(n * sizeof(int)) + (long)((max_val + 1) * sizeof(int)));

    strcpy(r.status, "SUCCESS");

    return r;
}
