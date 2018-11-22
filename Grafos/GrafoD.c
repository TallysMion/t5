#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../KDTREE/kdtree.h"
#include "../HashTable/hashtable.h"
#include "../Lista/lista.h"
#include "../Config/config.h"
#include "../Anotacao/anotacao.h"
#include "../Fila/fila.h"
#include "../Registrador/registrador.h"
#include <limits.h>

typedef struct VerticeV{
    char *id;
    double x;
    double y;
    struct ArestaP *aresta;
    int disable;
    int estado;
    int idDijkstra;
}VerticeV;

typedef struct ArestaP{
    VerticeV *v2;
    VerticeV *v1;
    int disable;
    char *nome; 
    char *cepR;
    char *cepL;
    double tam;
    double speed;
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

    V1 = (VerticeV *) v1;
    V2 = (VerticeV *) v2;
    dim = dim % 2;

    if(dim==0){
       return V1->x - V2->x;
    }else{
        return V1->y - V2->y;
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
void *grafoD_criar(GrafoD gd, char *id, double x, double y){

    Grafo *gr;
    gr = (Grafo*)gd;

    VerticeV *grafo;// = malloc(1*sizeof(VerticeV));
    grafo= calloc(1, sizeof(VerticeV));
    grafo->id = calloc(strlen(id)+2, sizeof(char));
    strcpy(grafo->id,id);
    grafo->disable = 0;
    grafo->x = x;
    grafo->y = y;
    grafo->aresta = NULL;

    KDT_insert(gr->vertices, grafo);
    insert_hashtable(gr->ID, grafo);

    return (void *) gd;
}

//função insere uma aresta
void grafoD_insereAresta(GrafoD gd, char *vID1,  char *vID2, char *leftCEP, char *rightCEP, double size, double speed, char* nome){

    VerticeV *V1, *aux;
    Grafo *GD;

    GD = (Grafo *) gd;

    aux = calloc(1, sizeof(VerticeV));
    aux->id = calloc(strlen(vID1)+2, sizeof(char));
    strcpy(aux->id, vID1);
    V1 = (VerticeV *) get_hashtable(GD->ID, aux);
    freeVerticeV(aux);


    ArestaP *atual;
    if(V1->aresta == NULL){
        V1->aresta = calloc(1, sizeof(ArestaP));
        atual = V1->aresta;
    }else{
        atual = V1->aresta;
        while(atual->next != NULL){
            atual = atual->next;
        }
        atual->next = calloc(1, sizeof(ArestaP));
        atual = atual->next;
    }

    atual->v1 = V1;
    
    aux = calloc(1, sizeof(VerticeV));
    aux->id = calloc(strlen(vID2)+2, sizeof(char));
    strcpy(aux->id, vID2);
    atual->v2 = (VerticeV *) get_hashtable(GD->ID, aux);
    freeVerticeV(aux);

    atual->disable = 0;
    atual->nome = calloc(strlen(nome)+2, sizeof(char));
    strcpy(atual->nome, nome);
    atual->cepR = calloc(strlen(rightCEP)+2,sizeof(char));
    strcpy(atual->cepR, rightCEP);
    atual->cepL = calloc(strlen(leftCEP)+2,sizeof(char));
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
void* txtCaminho(void *listaArestas){
    if(listaArestas == NULL)    return NULL;
    Fila result = create_Fila();
    void *item;
    ArestaP *aresta;
    char str[200];
    int i;
    char strings[3][20] = {"Siga na rua ", ", depois siga rua ", ", até a rua "};

    item = Lista_getFirst(listaArestas);
 
    strcpy(str, strings[0]);
    i = 1;

    while(item != NULL){

        aresta = (ArestaP *) item;

        printf("1\n");
        strcat(str,aresta->nome);
        insert_Fila(result, str);

        item  = Lista_getNext(listaArestas, item);

        strcpy(str, strings[i]);

        if(i==2){
            i = 1;
            strcpy(str, "\n");
            strcat(str, strings[i]);
            strcat(str,aresta->nome);
            printf("%s", str);
            insert_Fila(result, str);
            i = 2;
        }
        item = Lista_getNext(listaArestas, item);
    }
}

//print caminho no svg
void* svgCaminho(void *listaArestas, char *cor, double* inic, double* end){
    if(listaArestas == NULL)    return NULL;
    void *itemA, *itemB, *svg, *notation, *posic;
    ArestaP *ar1, *ar2;
    char *svgCode;

    Fila result = create_Fila();
    
    ar2 = Lista_get(listaArestas, Lista_getFirst(listaArestas));    
    notation = createNotacao(cor, *inic, *(inic+1), ar2->v2->x, ar2->v2->y, "");
    svgCode = createLine(notation);
    insert_Fila(result, svgCode);

    posic = Lista_getFirst(listaArestas);    
    while(posic != NULL){

        ar1 = (ArestaP *) Lista_get(listaArestas, posic);
        if(ar1){
            notation = createNotacao(cor, ar1->v1->x, ar1->v1->y, ar1->v2->x, ar1->v2->y, "");
            svgCode = createLine(notation);
            insert_Fila(result, svgCode);
        }
        posic  = Lista_getNext(listaArestas, posic);
    }    

    ar1 = Lista_get(listaArestas, Lista_getLast(listaArestas));    
    notation = createNotacao(cor, ar1->v2->x, ar1->v2->y, *end, *(end-1), "");
    svgCode = createLine(notation);
    insert_Fila(result, svgCode);

    return result;
}

void GrafoD_toWhite(void* grafo){
    Grafo *gr;
    gr = (Grafo*) grafo;
    Lista ls;
     ls = KDT_getAll(gr->vertices);
    void *posic; posic = Lista_getFirst(ls);
    while(1){
        void *item; item = Lista_get(ls, posic);
        if(item){
            VerticeV *aux;
            aux = (VerticeV*) item;
            aux->estado = 0;
        }else{break;}
        posic = Lista_getNext(ls, posic);
    }
}

//desbloqueia o grafo
void GrafoD_unlock(void* grafo){
    Grafo *gr;
    gr = (Grafo*) grafo;
    Lista ls;
     ls = KDT_getAll(gr->vertices);
    void *posic; posic = Lista_getFirst(ls);
    while(1){
        void *item; item = Lista_get(ls, posic);
        if(item){
            VerticeV *aux;
            aux = (VerticeV*) item;
            aux->disable = 0;
        }else{break;}
        posic = Lista_getNext(ls, posic);
    }
}

//Bloqueia os vertices que estao nessa area
void GrafoD_blockVertices(void* grafo,double w,double h,double x,double y){
    Grafo *gr;
    gr = (Grafo*) grafo;
    VerticeV *refA; refA = (VerticeV*) calloc(1, sizeof(VerticeV));
    VerticeV *refB; refB = (VerticeV*) calloc(1, sizeof(VerticeV));
    refA->x = x;
    refA->y = y;
    refB->x = x+w;
    refB->y = y+h;
    Lista toBlock = itensInsideArea(gr->vertices, refA, refB);
    void *posic; posic = Lista_getFirst(toBlock);
    while(1){
        void *item; item = Lista_get(toBlock, posic);
        if(item){
            VerticeV *aux;
            aux = (VerticeV*) item;
            aux->disable = 1;
        }else{break;}
        posic = Lista_getNext(toBlock, posic);
    }
}

int intercept(double alpha, double beta, double w, double h, double x, double y){
    double x2 = x+w;
    double y2 = y+h;
    double fx1, fx2, fy1, fy2;
    fx1 = alpha*x  + beta;
    fx2 = alpha*x2 + beta;
    fy1 = (y  - beta)/alpha;
    fy2 = (y2 - beta)/alpha;
    
    if(fx1 >= y && fx1 <= y2) return 1;
    if(fx2 >= y && fx2 <= y2) return 1;
    if(fy1 >= x && fy1 <= x2) return 1;
    if(fy2 >= x && fy2 <= x2) return 1;
    return 0;
 }

void blockArestas(VerticeV *vertA, double w,double h,double x,double y){
    //se vertA != braco -> retorna
    if(vertA->estado != 0){return;}
    vertA->estado = 1;
    ArestaP *atual;
    atual = vertA->aresta;
     //percorre arestas
    while(atual){
        //verifica se aresta intercepta
        double alpha, beta;
        alpha = (atual->v2->y - vertA->y) / (atual->v2->x - vertA->x);
        beta = vertA->y - (alpha*vertA->x);
        //se intercepta
        if(intercept(alpha, beta, w, h, x, y)){
            //blokeia a aresta
            atual->disable = 1;
            //blockArestas no destino
            blockArestas(atual->v2, w, h, x, y);
        }
        atual = atual->next;
     }
    //fim percorre
}

void GrafoD_blockArestas(void* grafo,double w,double h,double x,double y){
    Grafo *gr;
    gr = (Grafo*) grafo;
    VerticeV *ref, *inicial, *atual; 
    ref = (VerticeV*) calloc(1, sizeof(VerticeV));
    ref->x = x + (h/2);
    ref->y = y + (w/2);
    inicial = (VerticeV*) closestNeibord(gr->vertices, ref);
    freeVerticeV(ref);
    
    GrafoD_toWhite(grafo);
        
    blockArestas(inicial, w, h, x, y);
}

double getPos(void *ar, int i){

    VerticeV *vert;

    vert = (VerticeV *) ar;

    if(i==0){
        return vert->x;
    }else if(i==1){
        return vert->y;
    }
}

void* getKDT(void *grafo){

    Grafo *gd;

    gd = (Grafo *) grafo;

    return gd->vertices;
}

void* getAresta(void *v){
    VerticeV *ver;
    ver = (VerticeV*) v;
    return ver->aresta;
}


//Dijkstra
//------------------------------------------------------------------------------------------------------------------------------------------

//Recebe uma lista de Vertices, e retorna uma tabela de arestas
ArestaP ***arestTable(Lista vertices){
    void *posic, *item;
    ArestaP *atual;
    int i, j;
    int size = Lista_lenght(vertices);

    VerticeV **verts;
    verts = (VerticeV**) calloc(size, sizeof(VerticeV*));
    posic = Lista_getFirst(vertices);
    i = 0;

    while(posic!=NULL){
        item = Lista_get(vertices, posic);
        if(item){
            verts[i] = (VerticeV*) item;
            verts[i]->idDijkstra = i;
            i++;
        }
        posic = Lista_getNext(vertices, posic);
    }
    
    ArestaP ***result;
    result = (ArestaP***) calloc(size, sizeof(ArestaP**));
    for(i=0; i<size; i++)
        result[i] = (ArestaP**) calloc(size, sizeof(ArestaP*));

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            result[i][j] = NULL;
        }
    }
    
    for(i = 0; i < size; i++){
        atual = verts[i]->aresta;
        while(atual!=NULL){
            if(atual->disable == 0 && atual->v2->disable == 0){
                result[i][atual->v2->idDijkstra] = atual; 
            }
            atual = atual->next;
        }
    }

    return result;

}

//Recebe uma tabela de arestas e o numero de vertices, retorna uma tabela do tamanho das arestas
double **distTable(ArestaP ***arestas, int sizeVertices){
    void *posic, *item;
    ArestaP *atual;
    int i, j;

    
    double **result;
    result = (double**) calloc(sizeVertices, sizeof(double*));
    for(i=0; i<sizeVertices; i++)
        result[i] = (double*) calloc(sizeVertices, sizeof(double));

    for(i = 0; i < sizeVertices; i++){
        for(j = 0; j < sizeVertices; j++){
            if(arestas[i][j] == NULL){
                result[i][j] = 0.0;
            }else{
                result[i][j] = arestas[i][j]->tam;
            }
            
        }
    }
    return result;
}


//Recebe uma tabela de arestas e o numero de vertices, retorna uma tabela do tempo das arestas
double **timeTable(ArestaP ***arestas, int sizeVertices){
    void *posic, *item;
    ArestaP *atual;
    int i, j;

    
    double **result;
    result = (double**) calloc(sizeVertices, sizeof(double*));
    for(i=0; i<sizeVertices; i++)
        result[i] = (double*) calloc(sizeVertices, sizeof(double));

    for(i = 0; i < sizeVertices; i++){
        for(j = 0; j < sizeVertices; j++){
            if(arestas[i][j] == NULL){
                result[i][j] = 0.0;
            }else{
                result[i][j] = (arestas[i][j]->tam / arestas[i][j]->speed);
            }
            
        }
    }
    return result;
}

int minDistance(int dist[], int sptSet[], int qtd) 
{ 
    int min, min_index; 
    min = INT_MAX;

    for (int v = 0; v < qtd; v++) 
        if (sptSet[v] == 0 && dist[v] <= min) 
            min = dist[v], min_index = v; 

    return min_index; 
} 

void* dijkstra(void*** arestas, double ** pesos, int inicial, int final, int qtd)
{
	int dist[qtd];
	int sptSet[qtd];
    void *list;
    list = Lista_createLista();

	for (int i = 0; i < qtd; i++) 
		dist[i] = INT_MAX, sptSet[i] = 0; 

	dist[inicial] = 0; 

	for (int count = 0; count < qtd-1; count++)                                   
	{ 
	    int u = minDistance(dist, sptSet, qtd);
        if(u == final){
            break;
        }
        sptSet[u] = 1; 
        for (int v = 0; v < qtd; v++){
            if (!sptSet[v] && pesos[u][v] && dist[u] != INT_MAX && dist[u]+pesos[u][v] < dist[v]){
                if(u == final){
                    Lista_insertLista(list, arestas[u][v]);
                }
                dist[v] = dist[u] + pesos[u][v];                          
            }
        }
        
    }
    return list;
}


Lista caminho(void* grafo,double* idStart,double* idEnd, int mod){
    Grafo* gr;
    ArestaP*** arestas;
    double** pesos;
    int inicial;
    int final;
    gr = (Grafo*) grafo;
    Lista vertices;
    vertices = KDT_getAll(gr->vertices);
    int size = Lista_lenght(vertices);
    //chamar rota -> requisitos
    //tabela de arestas
    arestas = arestTable(vertices);
    //tabela de pesos
    pesos = mod == 0 ? distTable(arestas, size) : timeTable(arestas, size);
    
    VerticeV aux;
    VerticeV* auxV;

    //indice do vertice inicial
    aux.x = *(idStart);
    aux.y = *(idStart+1);
    auxV = (VerticeV*) closestNeibord(gr->vertices, &aux);
    inicial = auxV->idDijkstra; 
    
    //indice do vertice final
    aux.x = *(idEnd);
    aux.y = *(idEnd+1);
    auxV = (VerticeV*) closestNeibord(gr->vertices, &aux);
    final = auxV->idDijkstra;

    return dijkstra((void***)arestas, pesos, inicial, final, size);
}


}

int minDistance(int dist[], int sptSet[], int INT_MAX, int qtd) 
{ 
    int min, min_index; 
    int min = INT_MAX;

    for (int v = 0; v < qtd; v++) 
        if (sptSet[v] == 0) 
            min = dist[v], min_index = v; 

    return min_index; 
} 

void* Djikstra(void*** arestas, double ** pesos, int inicial, int final, int qtd)
{
	int dist[qtd];
	int sptSet[qtd];
    void *list;
    int INT_MAX;

    INT_MAX = -1;
    list = Lista_createLista();

	for (int i = 0; i < qtd; i++) 
		dist[i] = INT_MAX, sptSet[i] = 0; 

	dist[inicial] = 0; 

	for (int count = 0; count < qtd-1; count++)                                   
	{ 
	int u = minDistance(dist, sptSet, INT_MAX, qtd);

	sptSet[u] = 1; 
    if(u == final){
        for (int v = 0; v < qtd; v++) 
            if (!sptSet[v] && pesos[u][v] && dist[u] != INT_MAX && dist[u]+pesos[u][v] < dist[v]) 
                Lista_insertLista(list, arestas[u][v]), dist[v] = dist[u] + pesos[u][v];                          
	} 
    return list;
}
