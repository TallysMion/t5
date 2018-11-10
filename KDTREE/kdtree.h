#ifndef KDTREE_H
#define KDTREE_H

typedef void* KDT;
typedef void* KTD_Value;


/* Cria uma KD-Tree*/
void* KDT_create(int (*compare)(void*, void*, int), int dimension, void (*freeFunc)(void*));

/* Insere um Valor na KD-Tree*/
void   KDT_insert(KDT tree, KTD_Value value);

/*Retorna o tamanho da tree*/
int KDT_Size(void* tree);

/* Limpa a memoria da KD-Tree*/
void   freeKDTree(KDT tree);

/* Recupera uma lista com todos os valores da KD-Tree*/
void*  KDT_getAll(KDT tree);

/*Recupera um valor da arvore, com uma comparação iqual a referencia*/
void*  KDT_getKTD_Value(KDT tree, KTD_Value reference);

/*Remove um valor da arvore, com uma comparação iqual a referencia*/
void   KDT_remove(KDT tree, KTD_Value reference);

/*Retorna o item mais proximo da referencia*/
void* closestNeibord(void* tree, void* reference);

#endif