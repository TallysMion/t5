#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lista/lista.h"

typedef struct{
    int modulo;
    int (*compare)(void*, char*);
    int (*hash)(void*, int);
    void **hashtable;
} HashTable;

//Cria uma hashtable com modulo = n, usando a função de hash func e o comparador compare
void* create_hashtable(int modulo, int (*compare)(void*, char*), int (*hash)(void*, int)){
    HashTable *table;
    table = (HashTable*) calloc(1, sizeof(HashTable));
    table->modulo = modulo;
    table->compare = compare;
    table->hash = hash;
    table->hashtable = (Lista*) calloc(modulo, sizeof(Lista));
    int i;
    for(i=0; i < modulo; i++){
        *(table->hashtable + i) = Lista_createLista();
    }
    return (void*) table;
}

//Insere um item na hashtable
void insert_hashtable(void* hash, void* item){
    HashTable* table;
    table = (HashTable*) hash;
    int hashcode = table->hash(item, table->modulo);
    Lista_insert(*(table->hashtable + hashcode), item);
}

//retorna um item da hashtable
void* get_hashtable(void* hash, char* ident){
    HashTable* table;
    table = (HashTable*) hash;
    int hashcode = table->hash(ident, table->modulo);    
    Lista list = *(table->hashtable + hashcode);
    Posic t;
    t=Lista_getFirst(list);
    while(t != NULL){
        void* aux;
        aux = Lista_get(list,t);
        if(table->compare(aux, ident) == 0){
            return aux;
        }        
        t = Lista_getNext(list, t);
    }
    return NULL;

}

//retorna todos os itens da hashtable
Lista getAll_hashtable(void* hash){
    HashTable* table;
    table = (HashTable*) hash;
    Lista list = Lista_createLista();
    int i;

    for(i = 0; i < table->modulo; i++){
        Lista_insertAll(list, *(table->hashtable + i));
    }

    return list;

}

//apaga a hashtable
void free_hashtable(void* hash){
        HashTable* table;
    table = (HashTable*) hash;
    Lista list = Lista_createLista();
    int i;

    for(i = 0; i < table->modulo; i++){
        //Lista_free(*(table->hashtable + i));
        free(*(table->hashtable + i));
    }
    free(table->hashtable);
    free(table);
}
