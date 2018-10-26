#include <stdio.h>
#include <stdlib.h>
#include "../Utils/Point.h"


//compare - retorna 1 se argA < argB

void bruteForceSort(Point **vet, int bgn, int end, int (compare)(Point*, Point*)){
    int i, j;
    Point *aux;

    for(i = bgn; i < end; i++){
        for(j = i+1; j < end; j++){
            if(compare(vet[i], vet[j]) >= 0){
                aux = vet[j];
                vet[j] = vet[i];
                vet[i] = aux;
            }
        }
    }


}

void heap(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*));
void heapFirst(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*));
void heapSecond(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*));

void heap(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*)){

    heapFirst(raiz, vet, bgn, end, compare);
    heapSecond(raiz, vet, bgn, end, compare);
}

void heapFirst(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*)){
    void* aux;
    if(2*raiz < end-bgn && compare(vet[raiz],vet[2*raiz])){
            aux = vet[raiz];
            vet[raiz] = vet[2*raiz];
            vet[2*raiz] = aux;
            heap(2*raiz, vet, bgn, end, compare);
    }
}

void heapSecond(int raiz, void** vet, int bgn, int end, int (*compare)(void*, void*)){
    void* aux;
    if(2*raiz < end-bgn && compare(vet[raiz],vet[2*raiz+1])){
            aux = vet[raiz];
            vet[raiz] = vet[2*raiz+1];
            vet[2*raiz+1] = aux;
            heap(2*raiz+1, vet, bgn, end, compare);
    }
}



void heapSort (void** vet, int bgn, int end, int (*compare)(void*, void*)){
    int i;
    void* aux;

    //tranformar em heap
    for(i = (end-bgn)/2; i >= 0; i--){
        heap(i, vet, bgn, end, compare);
    }

    while(end-bgn > 0){
        aux = vet[0];
        vet[0] = vet[end-bgn];
        vet[end-bgn] = aux;
        bgn --;
        heap(bgn, vet, bgn, end, compare);
    }

}

void mergeSort(Point **vet, int bgn, int end, int (*compare)(Point*, Point*)){
    if(end - bgn <= 3){
        bruteForceSort(vet, bgn, end, compare);
        return;
    }
    int mid = (end-bgn)/2 + bgn;
    mergeSort(vet, bgn, mid, compare);
    mergeSort(vet, mid+1, end, compare);
    int i, j, k;
    i = bgn;
    j = mid+1;
    k = 0;
    Point *out[end-bgn];
    for(k=0; k<end-bgn; k++){
        if(j == end || (i < mid && compare(vet[j], vet[i]) >= 0)){
            out[k] = (Point*)calloc(1, sizeof(Point));
            *out[k] = *vet[i];
            i++;
            continue;
        }
        if(i == mid || (j < end && compare(vet[i], vet[j]) >= 0)){
            out[k] = (Point*)calloc(1, sizeof(Point));
            *out[k] = *vet[j];
            j++;
            continue;
        }
    }
    for(k = 0; k< end-bgn; k++){
        vet[bgn+k] = out[k];
    }
}
