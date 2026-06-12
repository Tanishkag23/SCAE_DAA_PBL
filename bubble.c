

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../scae.h"

AlgoResult bubble_sort(int* arr, int n) {


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


        int swapped = 0;

        for (int j = 0; j < n - i - 2 + 1; j++) {


            r.steps++;

            if (a[j] > a[j + 1]) {

                int temp = a[j];
                a[j]     = a[j + 1];
                a[j + 1] = temp;
                swapped  = 1;
            }
        }


        if (!swapped) break;
    }

    clock_t end = clock();


    r.time_ns     = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;
    r.outputSize  = n;
    r.memory_used = (long)(sizeof(AlgoResult) + n * sizeof(int));

    strcpy(r.big_o,  "O(n^2)");
    strcpy(r.status, "SUCCESS");

    return r;
}
