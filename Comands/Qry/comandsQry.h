#ifndef COMANDS_QRY_H
#define COMANDS_QRY_H
#include "../../Config/config.h"

    /*Reporta quadras e equipamentos dentro do retanguo*/
    void reportRec(char* text,Info* info);
    
    /*Reporta quadras e quipamentos dentro do circulo*/
    void reportCirc(char* text,Info* info);

    /*Remove as quadras dentro do retangulo*/
    void deleteQuadrasRec(char* text,Info* info);        

    /*Remove equipamentos dentro do retangulo*/
    void deleteUrbamEqRec(char* text,Info* info);

    /*Remove as quadras dentro do circulo*/
    void deleteQuadrasCirc(char* text,Info* info);

    /*Remove os equipamentos dentro do circulo*/
    void deleteUrbamEqCirc(char* text,Info* info);

    /*Muda a cor do item relacionado ao id/cep passado*/
    void colorChange(char* text,Info* info);

    /*informa as cordenadas e o ID do item passado, se for uma quadra, 
    retorna as cordenadas e id's de todos os equipamentos presentes nela*/
    void equipOnCepOrId(char* text,Info* info);

    /*encontra as radio-base mais proximas, e informa id's e distancia*/
    void closestRBase(char* text,Info* info);

    /*Finaliza os Processos de QRY e imprime o arquivo de saida */
    void closeQRY(Info* info);

    /*Imprime um relatorio dos moradores da quadra <m? 'cep'> */
    void whoIsHere(char* text, Info* info);

    /*Imprime um m? de todos as quadras inteiramente nessa area*/
    void whoIsInThisArea(char* text, Info* info);

    /*Imprime um relatorio da pessoa, incluindo a cordenada geografica, e imprime uma marcação no mapa*/
    void whoAreYou(char* text, Info* info);

    /*Imprime um relatorio do Estabelecimento, incluindo a cordenada geografica, e imprime uma marcação no mapa*/
    void whoAreYouEstab(char* text,Info* info);

    /*Mãe, no céu tem Pão? e Morreu...*/
    void bread(char* text, Info* info);

    /*Imprime um relatorio dos estabelecimentos existentes nessa quadra*/
    void whatHaveHere(char* text, Info* info);

    /*Imprime um relatorio dos estabelecimentos de um determinado tipo em uma area*/
    void whatHaveInThisArea(char* text, Info* info);

#endif