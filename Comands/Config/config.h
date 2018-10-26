#ifndef CONFIG_H
#define CONFIG_H

/*imports*/
#include "../Lista/lista.h"
#include "../Fila/fila.h"
#include "../KDTREE/kdtree.h"
#include "../HashTable/hashtable.h"


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
    KDT         Endereco;
    KDT         EstabelecimentoTree;

    HashTable   EstabelecimentoType;
    HashTable   *HidrantesHash; /*id*/
    HashTable   *SemaforosHash; /*id*/ 
    HashTable   *RadioBaseHash; /*id*/

    HashTable   *PessoaCepHash; /*cpf*/
    HashTable   *EstabHash; /*id*/
    HashTable   *cepQuadraHash; /*cep*/


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



/*formata um path*/
char* format(char *value);

/*le o argv e inicia as informações do programa*/
Info* configIn(int argc, const char *argv[]);

/*inclui as linhas do arquivo de entrada na fila e retorna a fila*/
Fila getCommandsForGEO(Info* info);

/*inclui as linhas do arquivo de entrada na fila e retorna a fila*/
Fila getCommandsForQRY(Info* info);

/*inclui as linhas do arquivo de entrada na fila e retorna a fila*/
Fila getCommandsForEC(Info* info);

/*inclui as linhas do arquivo de entrada na fila e retorna a fila*/
Fila getCommandsForPM(Info* info);

/*libera a memoria de config*/
void freeConfig(Info *info);

#endif