

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../scae.h"

#define SWAP(a, b) do { int _t = (a); (a) = (b); (b) = _t; } while (0)

static void merge_helper(int* a, int* temp, int lo, int hi, long* steps)
{

    if (lo >= hi) return;


    int mid = lo + (hi - lo) / 2;


    merge_helper(a, temp, lo, mid, steps);


    merge_helper(a, temp, mid + 1, hi, steps);




    for (int k = lo; k <= hi; k++) {
        temp[k] = a[k];
    }


    int i = lo;
    int j = mid + 1;
    int k = lo;

    while (i <= mid && j <= hi) {

        (*steps)++;


        if (temp[i] <= temp[j]) {
            a[k++] = temp[i++];
        } else {
            a[k++] = temp[j++];
        }
    }


    while (i <= mid) {
        a[k++] = temp[i++];
    }


    while (j <= hi) {
        a[k++] = temp[j++];
    }
}

AlgoResult merge_sort(int* arr, int n)
{

    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }


    AlgoResult r = create_result(n);


    memcpy(r.output, arr, n * sizeof(int));


    int* temp = (int*)malloc(n * sizeof(int));
    if (!temp) {

        strcpy(r.status, "ERROR: malloc failed for temp buffer");
        return r;
    }


    clock_t start = clock();


    merge_helper(r.output, temp, 0, n - 1, &r.steps);

    clock_t end = clock();


    free(temp);
    temp = NULL;


    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;


    r.memory_used = sizeof(AlgoResult) + 2 * n * sizeof(int);

    r.outputSize = n;


    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
