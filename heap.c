

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../scae.h"

#define SWAP(a, b) do { int _t = (a); (a) = (b); (b) = _t; } while (0)

static void heapify(int* a, int sz, int i, long* steps)
{

    int largest = i;


    int left  = 2 * i + 1;
    int right = 2 * i + 2;


    (*steps)++;
    if (left < sz && a[left] > a[largest]) {
        largest = left;
    }


    (*steps)++;
    if (right < sz && a[right] > a[largest]) {
        largest = right;
    }


    if (largest != i) {
        SWAP(a[i], a[largest]);


        heapify(a, sz, largest, steps);
    }

}

AlgoResult heap_sort(int* arr, int n)
{

    if (n <= 0) {
        AlgoResult r = create_result(0);
        strcpy(r.status, "SUCCESS");
        return r;
    }

    AlgoResult r = create_result(n);


    memcpy(r.output, arr, n * sizeof(int));


    clock_t start = clock();


    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(r.output, n, i, &r.steps);
    }


    for (int i = n - 1; i > 0; i--) {

        SWAP(r.output[0], r.output[i]);



        heapify(r.output, i, 0, &r.steps);
    }

    clock_t end = clock();


    r.time_ns = (double)(end - start) * 1e9 / (double)CLOCKS_PER_SEC;


    r.memory_used = sizeof(AlgoResult) + n * sizeof(int);

    r.outputSize = n;


    strcpy(r.big_o, "O(n log n)");

    strcpy(r.status, "SUCCESS");

    return r;
}
