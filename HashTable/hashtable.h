#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "../Lista/lista.h"
typedef void* HashTable;

//Cria uma hashtable com modulo = n, usando a função de hash func e o comparador compare
HashTable create_hashtable(int modulo, int (*compare)(void*, void*), int hash(void*, int));

//Remove um item da hashtable
void remove_hashtable(HashTable hash, void* item);

//Insere um item na hashtable
void insert_hashtable(HashTable hash, void* item);

//retorna um item da hashtable
void* get_hashtable(HashTable hash, char* ident);

//retorna todos os itens da hashtable
Lista getAll_hashtable(HashTable hash);

//apaga a hashtable -> depende do free da lista -> causando leek de memoria
void free_hashtable(HashTable hash);

#endif