#ifndef SORT_H
#define SORT_H
#include "../Utils/Point.h"

void heapSort(Point** vet, int bgn, int end, int (*compare)(Point*, Point*));

void mergeSort(Point** vet, int bgn, int end, int (*compare)(Point*, Point*));

#endif