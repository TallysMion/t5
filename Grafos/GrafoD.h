#ifndef _GRAFOD_LIB_
#define _GRAFOD_LIB_

typedef void* GrafoD;   
typedef void* Aresta;   
typedef void* Vertice;  
typedef void* Lista;
typedef void* KDT;
typedef void* KTD_Value;
typedef void* HASH;

/*
TAD Grafos direcionados
*/

//cria estrutura grafo
void* GRAFO_CREATE(int modulo);

//cria e insere vertice
void *grafoD_criar(GrafoD gd, char *id, float x, float y);

//função insere uma aresta
void grafoD_insereAresta(GrafoD gd, char *vID1,  char *vID2, char *leftCEP, char *rightCEP, float size, float speed);

//função retorna uma lista de adjacentes - destinos
Lista grafoD_listaAdjacente(Vertice v1);

//retorna true se a1 for adjacente a a2 e false se não for
int grafoD_Adjacente(Vertice a1, Vertice a2);

//compara "xy" grafo
int compareGD(Vertice v1, Vertice v2, int dim);

//compara cep encontrado com cep procurado - 1 p/ true(igual) e 0 p/ false(diferente)
int compareH_CEP(HASH itemA, Aresta itemB);

//compara id encontrado com id procurado
int compareH_ID(HASH itemA, Vertice itemB);

//hash encoding function
int hashFunction(void *String, int n);

#endif
