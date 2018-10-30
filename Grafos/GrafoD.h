#ifndef _GRAFOD_LIB_
#define _GRAFOD_LIB_

typedef void* GrafoD;   
typedef void* Aresta;   
typedef void* Vertice;  
typedef void* Lista;

/*
TAD Grafos direcionados
*/

//função insere vertice
GrafoD grafoD_criar(char *id, float x, float y);

//função insere uma aresta
void grafoD_insereAresta(Vertice v1, Vertice v2, char *leftCEP, char *rightCEP, float size, float speed);

//retorna true se a1 for adjacente a a2 e false se não for
int grafoD_Adjacente(Vertice a1, Vertice a2);

//função retorna uma lista de adjacentes - destinos
Lista grafoD_listaAdjacente(GrafoD gd, Vertice v1);

//função busca vértice por id
Vertice busca_vertice(GrafoD gd, char *id);

//função imprime grafo no SVG
char *print_SVG(GrafoD gd);

//função imprime caminho
char *print_caminho(GrafoD caminho); 

#endif
