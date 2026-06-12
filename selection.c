

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult selection_sort(int* arr, int n) {


    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);

    memcpy(r.output, arr, n * sizeof(int));

    int* a = r.output;


    clock_t start = clock();

    for (int i = 0; i < n - 1; i++) {


        int min_idx = i;

        for (int j = i + 1; j < n; j++) {
            r.steps++;

            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }


        if (min_idx != i) {

            int temp    = a[i];
            a[i]        = a[min_idx];
            a[min_idx]  = temp;
        }


    }

    clock_t end = clock();


    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.outputSize  = n;
    r.memory_used = (long)(sizeof(AlgoResult) + n * sizeof(int));

    strcpy(r.big_o,  "O(n^2)");
    strcpy(r.status, "SUCCESS");

    return r;
}
