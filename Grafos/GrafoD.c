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

typedef struct VerticeV{
    char *id;
    double x;
    double y;
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
void txtCaminho(void *listaArestas, void *inform){

    void *item;
    ArestaP *aresta;
    char str[200];
    int i;
    char strings[3][20] = {"Siga na rua ", ", depois siga rua ", ", até a rua "};
    Info *info;

    info = (Info *) inform;
    item = Lista_getFirst(listaArestas);
 
    strcpy(str, strings[0]);
    i = 1;

    while(item != NULL){

        aresta = (ArestaP *) item;

        printf("1\n");
        strcat(str,aresta->nome);
        insert_Fila(info->respQRY, str);

        item  = Lista_getNext(listaArestas, item);

        strcpy(str, strings[i]);

        if(i==2){
            i = 1;
            strcpy(str, "\n");
            strcat(str, strings[i]);
            strcat(str,aresta->nome);
            printf("%s", str);
            insert_Fila(info->respQRY, str);
            i = 2;
        }
        item = Lista_getNext(listaArestas, item);
    }
}

//print caminho no svg
void svgCaminho(void *listaArestas, void *inform, char *cor){

    void *itemA, *itemB, *svg, *notation, *posic;
    ArestaP *ar1, *ar2;
    Info *info;
    char *svgCode;

    FILE* test; //TESTE
    test = fopen("Teste/out/teste.svg", "w"); //TESTE
    fprintf(test, "%s\n", "<svg xmlns=\"http://www.w3.org/2000/svg\" width = \"5000\" height = \"5000\">"); //TESTE

    info = (Info *) inform;
    
    posic = Lista_getFirst(listaArestas);
    
    while(posic != NULL){

        ar1 = (ArestaP *) Lista_get(listaArestas, posic);
        if(ar1){
            notation = createNotacao("red", ar1->v1->x, ar1->v1->y, ar1->v2->x, ar1->v2->y, "");
            svgCode = createLine(notation);
            //insert_Fila(info->notsQRY, svgCode);

            fprintf(test, "%s", svgCode); //TESTE
        }
        posic  = Lista_getNext(listaArestas, posic);
    }    
    fprintf(test, "%s", "</svg>"); //TESTE
    fclose(test); //TESTE
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* getMINdist(void *inicio, void *fim){

    double v1[2], v2[2], menorD, *end, menor, ar2x, ar2y;
    ArestaP *aux, *Hid;
    aux = NULL;
    VerticeV *v;
    Grafo *gd;

    end = (double *) fim;

    v1[0] = getPos(inicio, 0);  
    v1[1] = getPos(inicio, 1);
    
    v = (VerticeV *) inicio;
    Hid = v->aresta;

    menorD = -1;
    do{
        if(Hid->disable == 0 && Hid->v2->disable == 0){
            ar2x = Hid->v2->x;
            ar2y = Hid->v2->y;

            menor = pontos_dist(ar2x, end[0], ar2y, end[1]);

            if(menorD == -1 || menor < menorD){
                menorD = menor;
                aux = Hid;
            }
        }
        Hid = Hid->next;   
    }while(Hid != NULL);

    
    return (void *) aux;
}

void* getCaminho(void* Hid, double *inicio, double *fim){

    double valor, v1[2];
    void *aux, *item, *vid, *list;
    ArestaP *auxi;
    VerticeV * itemAux, *closestInit;
    Grafo *gr; gr = (Grafo*) Hid;

    itemAux = (VerticeV*) calloc(1, sizeof(VerticeV));
    itemAux->x = *(inicio);
    itemAux->y = *(inicio+1);
    closestInit = closestEqualNeibord(gr->vertices , itemAux);
    
    list = Lista_createLista();


    double *end = (double *) fim;

    Grafo *gd = (Grafo *) Hid;

    while(1){
        item = getMINdist(closestInit, fim);
        if(item == NULL){
            void* auxRepair = Lista_getLast(list);
            auxi = (ArestaP*) Lista_get(list, auxRepair);
            if(auxi == NULL){
                printf("Caminho não encontrado");
                return NULL;
            }
            auxi->v2->disable = 1;
            auxi->disable = 1;
            closestInit = auxi->v1;
            Lista_remove(list, auxRepair);
            continue;
        }

        auxi = (ArestaP *) item;
        Lista_insert(list, item);  
        //parada: fim, contido em, v1 e v2 - aresta
        double xi, xf, yi, yf;
        xi = auxi->v1->x < auxi->v2->x ? auxi->v1->x : auxi->v2->x;  
        xf = auxi->v1->x < auxi->v2->x ? auxi->v2->x : auxi->v1->x;
        yi = auxi->v1->y < auxi->v2->y ? auxi->v1->y : auxi->v2->y;
        yf = auxi->v1->y < auxi->v2->y ? auxi->v2->y : auxi->v1->y;

        if(pontos_dist(auxi->v2->x, end[0], auxi->v2->y, end[1]) >= pontos_dist(auxi->v1->x, end[0], auxi->v1->y, end[1])){
            auxi->v2->disable = 1;
            auxi->disable = 1;
            Lista_remove(list, item);
            continue;
        }


        if( xi <= end[0] && end[0] <= xf && yi <= end[1] && end[1] <= yf ){
            vid = Lista_getLast(list);
            auxi = (ArestaP *) Lista_get(list, vid);
            auxi->v2->x = end[0];
            auxi->v2->y = end[1];

            vid = Lista_getFirst(list);
            auxi = (ArestaP *) Lista_get(list, vid);
            auxi->v1->x = v1[0];
            auxi->v1->y = v1[1];
            return list;
        }
        closestInit = auxi->v2;
    }

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


void buscaProfundidade(Grafo* grafo, VerticeV* inicio){
    Fila fila = create_Fila();
    insert_Fila(fila, inicio);
    while(!empty_Fila(fila)){
        VerticeV* atual = remove_Fila(fila);
        atual->estado = 1;
        ArestaP* percorre = atual->aresta;
        do{
            if(percorre->v2->estado == 0){
                
            }
        }


    }


}