

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../scae.h"

#define SWAP(a, b) do { int _t = (a); (a) = (b); (b) = _t; } while (0)

static int partition(int* a, int lo, int hi, long* steps)
{

    int pivot = a[hi];


    int i = lo - 1;


    for (int j = lo; j <= hi - 1; j++) {

        (*steps)++;

        if (a[j] <= pivot) {

            i++;
            SWAP(a[i], a[j]);
        }

    }


    SWAP(a[i + 1], a[hi]);


    return i + 1;
}

static void quick_helper(int* a, int lo, int hi, long* steps)
{

    if (lo >= hi) return;


    int pivot_idx = lo + rand() % (hi - lo + 1);


    SWAP(a[pivot_idx], a[hi]);


    int p = partition(a, lo, hi, steps);


    quick_helper(a, lo, p - 1, steps);


    quick_helper(a, p + 1, hi, steps);


}

AlgoResult quick_sort(int* arr, int n)
{

    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);


    memcpy(r.output, arr, n * sizeof(int));


    srand(42);


    clock_t start = clock();


    quick_helper(r.output, 0, n - 1, &r.steps);

    clock_t end = clock();


    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;


    r.memory_used = sizeof(AlgoResult) + n * sizeof(int);

    r.outputSize = n;


    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
