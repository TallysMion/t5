#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../KDTREE/kdtree.h"
#include "../HashTable/hashtable.h"

typedef void* GrafoD;   
typedef void* Aresta;   
typedef void* Vertice;  

typedef struct VerticeV{
    char *id;
    float x;
    float y;
    struct ArestaP *aresta;
}VerticeV;

typedef struct ArestaP{
    VerticeV *v2;
    int disable;
    char *nome; 
    char *cepR;
    char *cepL;
    float tam;
    float speed;
    struct ArestaP *next;
}ArestaP;

typedef struct Grafo{
    KDT vertices;
    HashTable left;
    HashTable right;
    HashTable ID;
}Grafo;

//-------------------------------------------------------------------------------------------------------------

//compara "xy" grafo
int compareGD(Vertice v1, Vertice v2, int dim){

    VerticeV *V1;
    VerticeV *V2;
    int cmp;

    V1 = (VerticeV *) v1;
    V2 = (VerticeV *) v2;
    cmp = dim % 2;

    if(dim==0){
       return V1->x > V2->x ? 1 : (V1->x < V2->x ? -1 : 0);
    }else{
        return V1->y > V2->y ? 1 : (V1->y < V2->y ? -1 : 0);
    }
}

//compara cep encontrado com cep procurado - 1 p/ true(igual) e 0 p/ false(diferente)
int compareH_CEP(HashTable itemA, Aresta itemB){

    ArestaP *found, *search;
    int i;

    found = (ArestaP *) itemA;
    search = (ArestaP *) itemB;

    i = strcmp(found->cepL,search->cepL);
    if(i==0){
        return 1;
    }
    i = strcmp(found->cepL,search->cepR);
    if(i==0){
        return 1;
    }
    i = strcmp(found->cepR,search->cepR);
    if(i==0){
        return 1;
    }
    i = strcmp(found->cepR,search->cepL);
    if(i==0){
        return 1;
    }

    return 0;
} 

//compara id encontrado com id procurado
int compareH_ID(HashTable itemA, Vertice itemB){

    VerticeV *found, *search;

    found = (VerticeV *) itemA;
    search = (VerticeV *) itemB;

    return strcmp(found->id,search->id);

} 

//hash encoding function
int hashFunctionArest(void *Item, int n){
    ArestaP * v;
    v = (ArestaP*)Item;
    char *string;
    int tam, hash;

    string = (char *) v->nome;

    tam = strlen(string);
    hash = 0;

    while(*string != 0){
        hash += tam*(*string);
        string++;
        tam--;
    }
    return n < 0 ? hash : hash%n;
}

//hash encoding function
int hashFunction(void *Item, int n){
    VerticeV * v;
    v = (VerticeV*)Item;
    char *string;
    int tam, hash;

    string = (char *) v->id;

    tam = strlen(string);
    hash = 0;

    while(*string != 0){
        hash += tam*(*string);
        string++;
        tam--;
    }
    return n < 0 ? hash : hash%n;
}

//cria estrutura grafo
void* GRAFO_CREATE(int n){
    Grafo* result;

    result = (Grafo*) calloc(1, sizeof(Grafo));
    result->vertices = KDT_create(compareGD,2);
    result->left = create_hashtable(n,compareH_CEP, hashFunctionArest);
    result->right = create_hashtable(n,compareH_CEP, hashFunctionArest);
    result->ID = create_hashtable(n,compareH_ID, hashFunction);
    return (void*) result;
}

//cria e insere vertice
void *grafoD_criar(GrafoD gd, char *id, float x, float y){

    Grafo *gr;
    gr = (Grafo*)gd;

    VerticeV *grafo = malloc(1*sizeof(VerticeV));
    grafo= malloc(1*sizeof(VerticeV));
    grafo->id = malloc(strlen(id)*sizeof(char));
    strcpy(grafo->id,id);

    KDT_insert(gr->vertices, grafo);
    gr = (Grafo *) gd;
    insert_hashtable(gr->ID, grafo);

    return (void *) gd;
}

//função insere uma aresta
void grafoD_insereAresta(GrafoD gd, char *vID1,  char *vID2, char *leftCEP, char *rightCEP, float size, float speed, char* nome){

    VerticeV *V1, *aux;
    Grafo *GD;

    GD = (Grafo *) gd;
    aux = calloc(1, sizeof(VerticeV));
    aux->id = vID1;
    V1 = (VerticeV *) get_hashtable(GD->ID, aux);
    V1->aresta = malloc(sizeof(ArestaP));
    aux->id = vID2;
    V1->aresta->v2 = (VerticeV *) get_hashtable(GD->ID, aux);

    V1->aresta->disable = 0;
    V1->aresta->nome = malloc(strlen(nome)*sizeof(char));
    strcpy(V1->aresta->nome, nome);
    V1->aresta->cepR = malloc(strlen(rightCEP)*sizeof(char));
    strcpy(V1->aresta->cepR, rightCEP);
    V1->aresta->cepL = malloc(strlen(leftCEP)*sizeof(char));
    strcpy(V1->aresta->cepL, leftCEP);
    V1->aresta->tam = size;
    V1->aresta->speed = speed;

    insert_hashtable(GD->left, V1->aresta);
    insert_hashtable(GD->right, V1->aresta);

}

//função retorna uma lista de adjacentes - destinos
Lista grafoD_listaAdjacente(Vertice v1){

    VerticeV *grafo = (VerticeV *) v1;

    return grafo->aresta;

}

//retorna true se a1 for adjacente a a2 e false se não for
int grafoD_Adjacente(Vertice a1, Vertice a2){

    VerticeV *v1 = (VerticeV *) a1;
    VerticeV *v2 = (VerticeV *) a2;

    while(v1->aresta->next!=NULL){
        if(strcmp(v1->aresta->v2->id,v1->id)==0){
            return 1;
        }
    }

    return 0;

}
