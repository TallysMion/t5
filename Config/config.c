#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lista/lista.h"
#include "../Fila/fila.h"
#include "../KDTREE/kdtree.h"
#include "../HashTable/hashtable.h"

#include "../Estabelecimento/estabelecimento.h"
#include "../Hidrante/hidrante.h"
#include "../Pessoa/pessoa.h"
#include "../Quadra/quadra.h"
#include "../Radio-Base/radio-base.h"
#include "../Semaforo/semaforo.h"

#define MODULOHASH 15

/*entrutura das informações base do programa*/
typedef struct config{
    
    char *cstrkQuad;
    char *cfillQuad;
    char *cstrkHidr;
    char *cfillHidr;
    char *cstrkSemaf;
    char *cfillSemaf;
    char *cstrkRBase;
    char *cfillRBase;

    char *colorMud;
    char *colorMudec;

}Config;

typedef struct database{
    int maxDrawerSize;
    Lista   *Drawer;

    KDT         HidrantesTree;
    KDT         SemaforosTree;
    KDT         QuadrasTree;
    KDT         RadioBaseTree;
    KDT         PessoaTree;
    KDT         EstabelecimentoTree;

    HashTable   EstabelecimentoType;
    HashTable   *HidrantesHash; /*id*/
    HashTable   *SemaforosHash; /*id*/ 
    HashTable   *RadioBaseHash; /*id*/

    HashTable   *PessoaCepHash; /*cpf*/
    HashTable   *EstabHash; /*id*/
    HashTable   *cepQuadraHash; /*cep*/
    HashTable   *enderecoEstab; /*cep*/
    HashTable   *enderecoPessoa; /*cep*/

}Database;

typedef struct Info{
    char *e;
    char *f;
    char *o;
    char *q;
    char *ec;
    char *pm;

    int size;

    Fila notsGeo; /*Anotações para SVG*/
    Fila notsQRY; /*Anotações para SVG*/
    Fila notsEC;  /*Anotações para SVG*/
    Fila notsPm;  /*Anotações para SVG*/

    Fila respGEO; /*Anotações para TXT*/
    Fila respQRY; /*Anotações para TXT*/
    Fila respEC;  /*Anotações para TXT*/
    Fila respPM;  /*Anotações para TXT*/

    Database *bd; /*banco de dados*/

    Config *conf; /*configurações*/

}Info;

char* format(char *value){

    if(*value != '.' && *value != '/'){
        char*aux; aux = (char*) calloc(255, sizeof(char));
        sprintf(aux, "/%s", value);
        value = aux;
    }
    if(*(value +strlen(value)-1) == '/' || *(value +strlen(value)-1) == '/' == '\n'){
        *(value +strlen(value) -1) = 0;
    }
    if(*(value +strlen(value)-1) == '/' || *(value +strlen(value)-1) == '/' == '\n'){
        *(value +strlen(value) -1) = 0;
    }
    
    return value;
}

/*atualizar*/
Info* configIn(int argc, const char *argv[]){
    int i;
    char *outTxt, *aux2;
    Info *result;

    result       = (Info*) calloc(1, sizeof(Info));
    result->bd   = (Database*) calloc(1, sizeof(Database));
    result->conf = (Config*)   calloc(1, sizeof(Config));

    result->size = 1000;

    for(i=0; i<argc; i++){
        char* aux;
        aux = (char*) calloc(strlen(argv[i])+1, sizeof(char));
        strcpy(aux, argv[i]);

        if(!strcmp(aux, "-e")){
            result->e = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->e, argv[i+1]);
            i++; continue;
        }
        if(!strcmp(aux, "-f")){
            result->f = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->f, argv[i+1]);
            i++; continue;
        }
        if(!strcmp(aux, "-o")){
            result->o = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->o, argv[i+1]);
            i++; continue;
        }
        if(!strcmp(aux, "-q")){
            result->q = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->q, argv[i+1]);
            i++; continue;
        }
        if(!strcmp(aux, "-ec")){
            result->ec = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->ec, argv[i+1]);
            i++; continue;
        }
        if(!strcmp(aux, "-pm")){
            result->pm = (char*) calloc(strlen(argv[i+1]) + 2, sizeof(char));
            strcpy(result->pm, argv[i+1]);
            i++; continue;
        }
        free(aux);
    }


    //char *cstrk;
    result->conf->cstrkQuad = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cstrkQuad, "black");

    //char *cfill;
    result->conf->cfillQuad = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cfillQuad, "red");

    //char *cstrk
    result->conf->cstrkHidr = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cstrkHidr, "black");

    //char *cfill;
    result->conf->cfillHidr = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cfillHidr, "red");      

    //char *cstrk
    result->conf->cstrkSemaf = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cstrkSemaf, "black");

    //char *cfill;
    result->conf->cfillSemaf = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cfillSemaf, "red");  

    //char *cstrk
    result->conf->cstrkRBase = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cstrkRBase, "black");
    
    //char *cfill;
    result->conf->cfillRBase = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->cfillRBase, "red");  


    //char *colorMud
    result->conf->colorMud = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->colorMud, "violet");
    
    //char *colorMudec;
    result->conf->colorMudec = (void*) calloc(255, sizeof(char));
    strcpy(result->conf->colorMudec, "pink");

    
    //LISTAS
    result->bd->Drawer  = Lista_createLista();

    //KDTREE
    //void*  KDT_create(int (*compare)(void*, void*, int), int dimension);
    result->bd->HidrantesTree       = KDT_create(compareHidrante , 2);
    result->bd->SemaforosTree       = KDT_create(compareSemaforo , 2);
    result->bd->QuadrasTree         = KDT_create(compareQuadra   , 2);
    result->bd->RadioBaseTree       = KDT_create(compareRadioB   , 2);
    result->bd->PessoaTree          = KDT_create(Pessoa_compare  , 2);
    result->bd->EstabelecimentoTree = KDT_create(Estab_compare   , 2);

    //HASHTABLE
    //HashTable create_hashtable(int modulo, int (*compare)(void*, char*), int hash(void*, int));
    result->bd->EstabelecimentoType = create_hashtable(MODULOHASH, Estab_Type_HashCompare,  Estab_Type_HashCode);
    result->bd->HidrantesHash       = create_hashtable(MODULOHASH, HashCompareHidrante,     hashCodeHidrante);
    result->bd->SemaforosHash       = create_hashtable(MODULOHASH, HashCompareSemaf,        hashCodeSemaforo);
    result->bd->RadioBaseHash       = create_hashtable(MODULOHASH, HashCompareRadioB,       hashCodeRadioB);
    result->bd->PessoaCepHash       = create_hashtable(MODULOHASH, Pessoa_HashCompare,      Pessoa_HashCode);
    result->bd->EstabHash           = create_hashtable(MODULOHASH, Estab_HashCompare,       Estab_HashCode);
    result->bd->cepQuadraHash       = create_hashtable(MODULOHASH, HashCompareQuadra,       hashCodeQuadra);
    result->bd->enderecoEstab       = create_hashtable(MODULOHASH, Estab_Ende_HashCompare,  Estab_Ende_HashCode);
    result->bd->enderecoPessoa      = create_hashtable(MODULOHASH, Pessoa_Ende_HashCompare, Pessoa_Ende_HashCode);

    //FILAS
    result->notsGeo     = create_Fila();
    result->notsQRY     = create_Fila();
    result->notsEC      = create_Fila();
    result->notsPm      = create_Fila();
    result->respGEO     = create_Fila();
    result->respQRY     = create_Fila();
    result->respEC      = create_Fila();
    result->respPM      = create_Fila();


    result->bd->maxDrawerSize   = 1000;



    if(result->e == NULL){
        result->e = (char*)calloc(1,sizeof(char));
        strcpy(result->e, "");
    }
    if(result->f == NULL){
        result->f = (char*)calloc(1,sizeof(char));
        strcpy(result->f, "");
    }
    if(result->o == NULL){
        result->o = (char*)calloc(1,sizeof(char));
        strcpy(result->o, "");
    }
    if(result->q == NULL){
        result->q = (char*)calloc(1,sizeof(char));
        strcpy(result->q, "");
    }
    if(result->ec == NULL){
        result->ec = (char*)calloc(1,sizeof(char));
        strcpy(result->ec, "");
    }
    if(result->pm == NULL){
        result->pm = (char*)calloc(1,sizeof(char));
        strcpy(result->pm, "");
    }

    return result;

}

Fila getCommandsForGEO(Info* info){
    Fila result;
    FILE *arq;
    char *path;
    result = create_Fila();
    path = (char*) calloc(200, sizeof(char));
    sprintf(path, "%s%s", format(info->e), format(info->f));
    if(*path == '/')path++;
    arq = fopen(path, "r");
    if(!arq){
        printf("I AM GROOT\n");
        return 0;
    }  

    while(!feof(arq)){
        char*aux; aux = (char*) calloc(200, sizeof(char));
        if(fgets(aux, 2000, arq)==NULL){continue;}
        insert_Fila(result,(Value) aux);

    }

    fclose(arq);

    return result;

}

Fila getCommandsForQRY(Info* info){
    Fila result;
    FILE *arq;
    char *path;
    result = create_Fila();
    path = (char*) calloc(200, sizeof(char));
    if(!strcmp(info->q,"")){
        return NULL;
    }
    sprintf(path, "%s%s", format(info->e), format(info->q));
    if(*path == '/')path++;
    arq = fopen(path, "r");
    if(!arq){
        printf("I AM GROOT\n");
        return 0;
    }  

    while(!feof(arq)){
        char*aux; aux = (char*) calloc(200, sizeof(char));
        if(fgets(aux, 2000, arq)==NULL){continue;}
        insert_Fila(result,(Value) aux);
    }

    fclose(arq);

    return result;

}

/*Atualizar*/
Fila getCommandsForEC(Info* info){
    Fila result;
    FILE *arq;
    char *path;
    result = create_Fila();
    path = (char*) calloc(200, sizeof(char));
    if(!strcmp(info->ec,"")){
        return NULL;
    }
    sprintf(path, "%s%s", format(info->e), format(info->ec));
    if(*path == '/')path++;
    arq = fopen(path, "r");
    if(!arq){
        printf("I AM GROOT\n");
        return 0;
    }  

    while(!feof(arq)){
        char*aux; aux = (char*) calloc(200, sizeof(char));
        if(fgets(aux, 2000, arq)==NULL){continue;}
        insert_Fila(result,(Value) aux);
    }

    fclose(arq);

    return result;

}

/*Atualizar*/
Fila getCommandsForPM(Info* info){
    Fila result;
    FILE *arq;
    char *path;
    result = create_Fila();
    path = (char*) calloc(200, sizeof(char));
    if(!strcmp(info->pm,"")){
        return NULL;
    }
    sprintf(path, "%s%s", format(info->e), format(info->pm));
    if(*path == '/')path++;
    arq = fopen(path, "r");
    if(!arq){
        printf("I AM GROOT\n");
        return 0;
    }  

    while(!feof(arq)){
        char*aux; aux = (char*) calloc(200, sizeof(char));
        if(fgets(aux, 2000, arq)==NULL){continue;}
        insert_Fila(result,(Value) aux);
    }

    fclose(arq);

    return result;

}

/*atualizar*/
void freeConfig(Info *info){

    free_hashtable(info->bd->cepQuadraHash);
    free_hashtable(info->bd->HidrantesHash);
    free_hashtable(info->bd->PessoaCepHash);
    free_hashtable(info->bd->RadioBaseHash);
    free_hashtable(info->bd->SemaforosHash);
    free_hashtable(info->bd->EstabHash);
    //freeTree(info->bd->EstabelecimentoTree);
    //freeTree(info->bd->HidrantesTree);
    //freeTree(info->bd->PessoaTree);
    //freeTree(info->bd->QuadrasTree);
    //freeTree(info->bd->RadioBaseTree);
    //freeTree(info->bd->SemaforosTree);
    //freeLista(info->bd->Drawer);
    free(info->bd);

    free(info->conf->cfillHidr);
    free(info->conf->cfillQuad);
    free(info->conf->cfillRBase);
    free(info->conf->cfillSemaf);
    free(info->conf->colorMud);
    free(info->conf->colorMudec);
    free(info->conf->cstrkHidr);
    free(info->conf->cstrkQuad);
    free(info->conf->cstrkRBase);
    free(info->conf->cstrkSemaf);
    free(info->conf);

    free(info->e);
    free(info->ec);
    free(info->f);
    free(info->o);
    free(info->pm);
    free(info->q);
}