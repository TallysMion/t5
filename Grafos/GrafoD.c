#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../KDTREE/kdtree.h"
#include "../HashTable/hashtable.h"
#include "../Lista/lista.h"
#include "../Config/config.h"
#include "../Anotacao/anotacao.h"
#include "../Fila/fila.h" 

typedef struct VerticeV{
    char *id;
    float x;
    float y;
    struct ArestaP *aresta;
    int disable;
    int estado;
}VerticeV;

typedef struct ArestaP{
    VerticeV *v2;
    VerticeV *v1;
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

void freeArestaP(void* a){
    ArestaP *ar;
    ar = (ArestaP*) a;
    if(ar == NULL) return;
    free(ar->cepL);
    free(ar->cepR);
    free(ar->nome);
    ar->v2 = NULL;
    void* a2 = ar->next;
    free(ar);
    freeArestaP(a2);
}

void freeVerticeV(void* v){
    VerticeV *vt;
    vt = (VerticeV*) v;
    free(vt->id);
    freeArestaP(vt->aresta);
}


void freeGrafoD(void* grafo){
    Grafo *g;
    g = (Grafo*)grafo;
    if(g == NULL)
    return;
    if(g->vertices != NULL)
    freeKDTree(g->vertices);
    if(g->left != NULL)
    free_hashtable(g->left);
    if(g->right != NULL)
    free_hashtable(g->right);
    if(g->ID != NULL)
    free_hashtable(g->ID);
    free(g);
}

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
    result->vertices = KDT_create(compareGD,2, freeVerticeV);
    result->left = create_hashtable(n,compareH_CEP, hashFunctionArest);
    result->right = create_hashtable(n,compareH_CEP, hashFunctionArest);
    result->ID = create_hashtable(n,compareH_ID, hashFunction);
    return (void*) result;
}

//cria e insere vertice
void *grafoD_criar(GrafoD gd, char *id, float x, float y){

    Grafo *gr;
    gr = (Grafo*)gd;

    VerticeV *grafo;// = malloc(1*sizeof(VerticeV));
    grafo= calloc(1, sizeof(VerticeV));
    grafo->id = calloc(strlen(id)+2, sizeof(char));
    strcpy(grafo->id,id);
    grafo->disable = 0;
    grafo->x = x;
    grafo->y = y;

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

    ArestaP *atual;
    if(V1->aresta == NULL){
        V1->aresta = malloc(sizeof(ArestaP));
        atual = V1->aresta;
    }else{
        atual = V1->aresta;
        while(atual->next != NULL){
            atual = atual->next;
        }
        atual->next = malloc(sizeof(ArestaP));
        atual = atual->next;
    }

    V1->aresta->v1 = V1;
    
    aux->id = vID2;
    atual->v2 = (VerticeV *) get_hashtable(GD->ID, aux);

    atual->disable = 0;
    atual->nome = malloc(strlen(nome)*sizeof(char));
    strcpy(atual->nome, nome);
    atual->cepR = malloc(strlen(rightCEP)*sizeof(char));
    strcpy(atual->cepR, rightCEP);
    atual->cepL = malloc(strlen(leftCEP)*sizeof(char));
    strcpy(atual->cepL, leftCEP);
    atual->tam = size;
    atual->speed = speed;
    atual->next = NULL;

    insert_hashtable(GD->left, atual);
    insert_hashtable(GD->right, atual);

}

//função retorna uma lista de adjacentes - destinos
void *grafoD_listaAdjacente(Vertice v1){

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

//print caminho no txt
void txtCaminho(void *listaArestas, void *inform){

    void *item;
    ArestaP *aresta;
    char str[200];
    int i;
    char strings[3][20] = {"Siga na rua ", ", depois siga rua ", ", até a rua "};
    Info *info;

    info = (Info *) inform;
    item = listaArestas;
 
    strcpy(str, strings[0]);
    i = 1;
    while(item != NULL){

        item = Lista_get(listaArestas, item);
        aresta = (ArestaP *) item;

        strcat(str,aresta->nome);
        insert_Fila(info->respQRY, str);

        item  = Lista_getNext(listaArestas, item);

        strcpy(str, strings[i]);

        if(i==2){
            i = 1;
            strcpy(str, "\n");
            strcat(str, strings[i]);
            strcat(str,aresta->nome);
            insert_Fila(info->respQRY, str);
            i = 2;
        }

    }
}

//print caminho no svg
void svgCaminho(void *listaArestas, void *inform){

    void *item, *svg, *notation;
    ArestaP *aresta;
    Info *info;
    char *svgCode;

    info = (Info *) inform;
    item = listaArestas;
 
    while(item != NULL){

        item = Lista_get(listaArestas, item);
        aresta = (ArestaP *) item;

        notation = createNotacao("red", -1, -1, -1*aresta->v1->x, -1*aresta->v1->y, NULL);
        svgCode = createNotacaoSvg(notation);
        insert_Fila(info->notsQRY, svgCode);
        
        item  = Lista_getNext(listaArestas, item);
    }    

}




