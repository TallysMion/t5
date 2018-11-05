#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../../Item/item.h"
#include "../../Config/config.h"
#include "../../Circulo/circulo.h"
#include "../../Retangulo/retangulo.h"
#include "../../Anotacao/anotacao.h"
#include "../../Quadra/quadra.h"
#include "../../Radio-Base/radio-base.h"
#include "../../Semaforo/semaforo.h"
#include "../../Hidrante/hidrante.h"
#include "../../Pessoa/pessoa.h"
#include "../../Estabelecimento/estabelecimento.h"
#include "../../Utils/Point.h"
#include "../../Utils/proxPoint.h"
#include "../../KDTREE/kdtree.h"
#include "../../HashTable/hashtable.h"


/*Reporta quadras e equipamentos dentro do retanguo*/
void reportRec(char* text,Info *info){
        char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");
        char *aux;
        double x, y , w, h;
        double xi, yi, xf, yf;
        void* i;
        Posic t;

        aux = text; aux += 3;
        sscanf(aux, "%lf %lf %lf %lf", &x, &y, &w, &h);

        /*info->quadras*/
        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                rectangle rc = getRecQuad(it);                    

                xi = getXRec(rc);
                xf = xi + getWRec(rc);
                yi = getYRec(rc);
                yf = yi + getHRec(rc);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){

                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Quadra. -  id: %s  -  x: %lf  -  y: %lf\n", getCepQuad(it), getXRec(rc), getYCirc(rc));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(quadras, t);
            }else{
            break;
            }
        }

        /*hidrantes*/

        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(1){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                circulo circ = getCircHidr(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(hidrantes, t);
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(1){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                circulo circ = getCircRadioB(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Radios. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(radios, t);
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);
        while(1){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                circulo circ = getCircSemaf(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Semaf. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(semaforos, t);
            }else{
            break;
            }
        }

    //imprimir resultados
    Notation nt = createNotacao("Black", w, h, x, y, "");
    insert_Fila(info->notsQRY, nt);

}

/*Reporta quadras e quipamentos dentro do circulo*/
void reportCirc(char* text,Info* info){
        char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");

        char *aux;
        double x, y, r;
        double xi, yi, xf, yf, ri;
        void* i;
        Posic t;

        aux = text; aux += 3;
        sscanf(aux, "%lf %lf %lf", &r, &x, &y);

        /*info->quadras*/

        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                rectangle rc = getRecQuad(it);                    

                xi = getXRec(rc);
                xf = xi + getWRec(rc);
                yi = getYRec(rc);
                yf = yi + getHRec(rc);

                xi -= x;
                xf -= x;
                yi -= y;
                yf -= y;

                if(sqrt(xi*xi + yi*yi) <= r && sqrt(xf*xf + yf*yf) <= r
                    && sqrt(xi*xi + yf*yf) <= r && sqrt(xf*xf + yi*yi) <= r){
                    if(sqrt(xi*xi + yf*yf) <= r && sqrt(xf*xf + yi*yi) <= r){
                        char * temp; temp = (char*) calloc(155, sizeof(char));
                        sprintf(temp, "Quadra. -  id: %s  -  x: %lf  -  y: %lf\n", getCepQuad(it), getXRec(rc), getYCirc(rc));
                        insert_Fila(info->respQRY, temp);
                    }
                }

                t = Lista_getNext(quadras, t);
            }else{
            break;
            }
        }

        /*hidrantes*/

        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(1){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                circulo circ = getCircHidr(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);
                
                xi -= x;
                xf -= x;


                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                   
                
                t = Lista_getNext(hidrantes, t);
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(1){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                circulo circ = getCircRadioB(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);
                
                xi -= x;
                xf -= x;

                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Radios. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                   
                
                t = Lista_getNext(radios, t);
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);
        while(1){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                circulo circ = getCircSemaf(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);

                xi -= x;
                xf -= x;

                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Semaf. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                     
                
                t = Lista_getNext(semaforos, t);
            }else{
            break;
            }
        }

    //imprimir resultados
    Notation nt = createNotacao("Black", r, 0, x, y, "");
    insert_Fila(info->notsQRY, nt);

}

/*Remove as quadras dentro do retangulo*/
void deleteQuadrasRec(char* text,Info* info){
        char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");
        


        char *aux;
        double x, y , w, h;
        double xi, yi, xf, yf;
        void* i;
        Posic t;

        aux = text; aux += 3;
        sscanf(aux, "%lf %lf %lf %lf", &x, &y, &w, &h);


        /*info->quadras*/
        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        printf("/n/n___  %d  ___\n\n",Lista_lenght(quadras));
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                rectangle rc = getRecQuad(it);                    

                xi = getXRec(rc);
                xf = xi + getWRec(rc);
                yi = getYRec(rc);
                yf = yi + getHRec(rc);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp1; temp1 = (char*) calloc(155, sizeof(char));
                    sprintf(temp1, "%s\n", getCepQuad(it));
                    insert_Fila(info->respQRY, temp1);

                    void* temp = Lista_getNext(quadras, t);
                    KDT_remove(info->bd->QuadrasTree, it);
                    remove_hashtable(info->bd->cepQuadraHash, it);
                    Lista_remove(quadras, t);
                    t = temp;  
                }else{              
                    t = Lista_getNext(quadras, t);
                }
            }else{
            break;
            }
        }


    //imprimir resultados
    Notation nt = createNotacao("red", w, h, x, y, "");
    insert_Fila(info->notsQRY, nt);

}       

/*Remove equipamentos dentro do retangulo*/
void deleteUrbamEqRec(char* text,Info* info){
    char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");


        char *aux, *cont;
        int hd = 0, sm = 0, rb = 0;
        double x, y , w, h;
        double xi, yi, xf, yf;
        void* i;
        Posic t;

        cont = (char*) calloc(155, sizeof(char));

        aux = text; aux += 4;
        sscanf(aux, "%s %lf %lf %lf %lf", cont, &x, &y, &w, &h);

        while(*cont){
            switch(*cont){
                case 'h':hd = 1;
                break;
                case 's':sm = 1;
                break;
                case 'r':rb = 1;
                break;
            }
            cont++;
        }

        /*hidrantes*/
        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(hd){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                circulo circ = getCircHidr(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(hidrantes, t);
                    Lista_remove(hidrantes, t);
                    KDT_remove(info->bd->HidrantesTree, it);
                    remove_hashtable(info->bd->HidrantesHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(hidrantes, t);
                }
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(rb){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                circulo circ = getCircRadioB(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Radios. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(radios, t);
                    Lista_remove(radios, t);
                    KDT_remove(info->bd->RadioBaseTree, it);
                    remove_hashtable(info->bd->RadioBaseHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(radios, t);
                }
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);
        while(sm){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                circulo circ = getCircSemaf(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Semaf. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(semaforos, t);
                    Lista_remove(semaforos, t);
                    KDT_remove(info->bd->SemaforosTree, it);
                    remove_hashtable(info->bd->SemaforosHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(semaforos, t);
                }                
                
            }else{
            break;
            }
        }

    //imprimir resultados
    Notation nt = createNotacao("Black", w, h, x, y, "");
    insert_Fila(info->notsQRY, nt);

}

/*Remove as quadras dentro do circulo*/
void deleteQuadrasCirc(char* text,Info* info){
        char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");


        char *aux;
        double x, y , r;
        double xi, yi, xf, yf;
        void* i;
        Posic t;

        aux = text; aux += 3;
        sscanf(aux, "%lf %lf %lf", &r, &x, &y);

        Lista list;
        list = Lista_createLista();

        /*info->quadras*/

        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                rectangle rc = getRecQuad(it);                    

                xi = getXRec(rc);
                xf = xi + getWRec(rc);
                yi = getYRec(rc);
                yf = yi + getHRec(rc);

                xi -= x;
                xf -= x;
                yi -= y;
                yf -= y;

                if(sqrt(xi*xi + yi*yi) <= r && sqrt(xf*xf + yf*yf) <= r
                    && sqrt(xi*xi + yf*yf) <= r && sqrt(xf*xf + yi*yi) <= r){
                    char * temp1; temp1 = (char*) calloc(155, sizeof(char));
                    sprintf(temp1, "%s\n", getCepQuad(it));
                    insert_Fila(info->respQRY, temp1);
                    void* temp = Lista_getNext(quadras, t);
                    Lista_remove(quadras, t);
                    KDT_remove(info->bd->QuadrasTree, it);
                    remove_hashtable(info->bd->cepQuadraHash, it);
                    t = temp;                    
                }else{
                    t = Lista_getNext(quadras, t);
                }                            
            }else{
            break;
            }
        }


    //imprimir resultados
    Notation nt = createNotacao("red", r, 0, x, y, "");
    insert_Fila(info->notsQRY, nt);



}   
/*Remove os equipamentos dentro do circulo*/
void deleteUrbamEqCirc(char* text,Info* info){
        char* temp;
        temp = (char*) calloc (155, sizeof(char));
        strcpy(temp, text);
        insert_Fila(info->respQRY, temp);
        insert_Fila(info->respQRY, "\n");


        char *aux, *cont;
        int hd = 0, sm = 0, rb = 0;
        double x, y , r;
        double xi, yi, xf, yf, ri;
        void* i;
        Posic t;
        cont = (char*) calloc(155, sizeof(char));

        aux = text; aux += 4;
        sscanf(aux, "%s %lf %lf %lf", cont, &x, &y, &r);

        while(*cont){
            switch(*cont){
                case 'h':hd = 1;
                break;
                case 's':sm = 1;
                break;
                case 'r':rb = 1;
                break;
            }
            cont++;
        }


        /*hidrantes*/

        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(hd){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                circulo circ = getCircHidr(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);

                xi -= x;
                yi -= y;

                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(hidrantes, t);
                    Lista_remove(hidrantes, t);
                    KDT_remove(info->bd->HidrantesTree, it);
                    remove_hashtable(info->bd->HidrantesHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(hidrantes, t);
                }
                
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(rb){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                circulo circ = getCircRadioB(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);

                xi -= x;
                yi -= y;

                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Radios. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(radios, t);
                    Lista_remove(radios, t);
                    KDT_remove(info->bd->RadioBaseTree, it);
                    remove_hashtable(info->bd->RadioBaseHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(radios, t);
                }
                
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);
        while(sm){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                circulo circ = getCircSemaf(it);

                xi = getXCirc(circ);
                yi = getYCirc(circ);
                ri = getRCirc(circ);

                xi -= x;
                yi -= y;

                if(sqrt(xi*xi + yi*yi)+ri <= r){
                    char * temp2; temp2 = (char*) calloc(155, sizeof(char));
                    sprintf(temp2, "Semaf. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp2);
                    void* temp = Lista_getNext(semaforos, t);
                    Lista_remove(semaforos, t);
                    KDT_remove(info->bd->SemaforosTree, it);
                    remove_hashtable(info->bd->SemaforosHash, it);
                    t = temp;  
                }else{
                    t = Lista_getNext(semaforos, t);
                }
                
            }else{
            break;
            }
        }

    //imprimir resultados
    Notation nt = createNotacao("Black", r, 0, x, y, "");
    insert_Fila(info->notsQRY, nt);

}

/*Muda a cor do item relacionado ao id/cep passado*/
void colorChange(char* text,Info* info){
        char *aux, *ident, *strok, *fill;
        Posic t;
        void* i;

        aux = text; aux += 3;

        ident = (char*) calloc(155, sizeof(char));
        strok = (char*) calloc(155, sizeof(char));
        fill = (char*) calloc(155, sizeof(char));
        
        sscanf(aux, "%s %s %s", ident, strok, fill);

            /*info->quadras*/

        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                if(!strcmp(getCepQuad(it), ident)){
                    setColorStrokeQuad(it, strok);
                    setColorFillQuad(it, fill);
                    return;
                }                                        
                t = Lista_getNext(quadras, t);
            }else{
            break;
            }
        }

        /*hidrantes*/

        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(1){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                sprintf(temp, "%s", getIdHidr(it));

                if(!strcmp(temp, ident)){
                    setColorStrokeHidr(it, strok);
                    setColorFillHidr(it, fill);
                    return;
                }                    
                
                t = Lista_getNext(hidrantes, t);
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(1){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                sprintf(temp, "%s", getIdRadioB(it));

                if(!strcmp(temp, ident)){
                    setColorStrokeRB(it, strok);
                    setColorFillRB(it, fill);
                    return;
                }            
                
                t = Lista_getNext(radios, t);
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);    
        while(1){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                sprintf(temp, "%s", getIdSemaf(it));

                if(!strcmp(temp, ident)){
                    setColorStrokeSemaf(it, strok);
                    setColorFillSemaf(it, fill);
                    return;
                }                  
                
                t = Lista_getNext(semaforos, t);
            }else{
            break;
            }
        }

}

/*informa as cordenadas e o ID do item passado, se for uma quadra, 
retorna as cordenadas e id's de todos os equipamentos presentes nela*/
void equipOnCepOrId(char* text,Info* info){
    char* temp;
    temp = (char*) calloc (155, sizeof(char));
    strcpy(temp, text);
    insert_Fila(info->respQRY, temp);
    insert_Fila(info->respQRY, "\n");


    char *aux, *ident;
    Posic t;
    void* i;

    aux = text; aux += 5;
    ident = (char*) calloc(155, sizeof(char));
    sscanf(aux, "%s", ident);


    /*hidrantes*/

        Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
        t=Lista_getFirst(hidrantes);
        while(1){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                
                sprintf(temp, "%s", getIdHidr(it));

                if(!strcmp(temp, ident)){
                    sprintf(temp, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(getCircHidr(it)), getYCirc(getCircHidr(it)));
                    insert_Fila(info->respQRY, temp);
                    return;
                }                    
                
                t = Lista_getNext(hidrantes, t);
            }else{
            break;
            }
        }

        /*info->radio-base*/

        Lista radios = KDT_getAll(info->bd->RadioBaseTree);
        t=Lista_getFirst(radios);
        while(1){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                sprintf(temp, "%s", getIdRadioB(it));

                if(!strcmp(temp, ident)){
                    sprintf(temp, "Radio-Base. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(getCircRadioB(it)), getYCirc(getCircRadioB(it)));
                    insert_Fila(info->respQRY, temp);
                    return;
                }                
                
                t = Lista_getNext(radios, t);
            }else{
            break;
            }
        }

        /*info->semaforo*/

        Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
        t=Lista_getFirst(semaforos);
        while(1){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                char *temp;                     
                temp = (char*) calloc(155, sizeof(char));
                sprintf(temp, "%s", getIdSemaf(it));

                if(!strcmp(temp, ident)){
                    sprintf(temp, "Semaforo. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(getCircSemaf(it)), getYCirc(getCircSemaf(it)));
                    insert_Fila(info->respQRY, temp);
                    return;
                }                 
                
                t = Lista_getNext(semaforos, t);
            }else{
            break;
            }
        }


        //achar cep

        rectangle rt;
        rt = NULL;
        Lista quadras = KDT_getAll(info->bd->QuadrasTree);
        t=Lista_getFirst(quadras);
        while(1){
            i = Lista_get(quadras,t);
            if(i){

                Item it = Lista_get(quadras, t);
                if(!strcmp(getCepQuad(it), ident)){
                    rt = getRecQuad(it);
                    break;
                }                                        
                t = Lista_getNext(quadras, t);
            }else{
            break;
            }
        }

        if(rt == NULL){
            return;
        }
        double x, y, w, h;
        x = getXRec(rt);
        y = getYRec(rt);
        w = getWRec(rt);
        h = getHRec(rt);

        double xi, xf, yi, yf;

        t=Lista_getFirst(hidrantes);
        while(1){
            i = Lista_get(hidrantes,t);
            if(i){
                
                Item it = Lista_get(hidrantes, t);
                circulo circ = getCircHidr(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Hidr. -  id: %s  -  x: %lf  -  y: %lf\n", getIdHidr(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(hidrantes, t);
            }else{
            break;
            }
        }

        /*info->radio-base*/


        t=Lista_getFirst(radios);
        while(1){
            i = Lista_get(radios,t);
            if(i){
                
                Item it = Lista_get(radios, t);
                circulo circ = getCircRadioB(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Radio-Base. -  id: %s  -  x: %lf  -  y: %lf\n", getIdRadioB(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(radios, t);
            }else{
            break;
            }
        }

        /*info->semaforo*/


        t=Lista_getFirst(semaforos);
        while(1){
            i = Lista_get(semaforos,t);
            if(i){
                
                Item it = Lista_get(semaforos, t);
                circulo circ = getCircSemaf(it);

                xi = getXCirc(circ) - getRCirc(circ);
                xf = getXCirc(circ) + getRCirc(circ);
                yi = getYCirc(circ) - getRCirc(circ);
                yf = getYCirc(circ) + getRCirc(circ);

                if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                    char * temp; temp = (char*) calloc(155, sizeof(char));
                    sprintf(temp, "Semaforo. -  id: %s  -  x: %lf  -  y: %lf\n", getIdSemaf(it), getXCirc(circ), getYCirc(circ));
                    insert_Fila(info->respQRY, temp);
                }                    
                
                t = Lista_getNext(semaforos, t);
            }else{
            break;
            }
        }


}



/*encontra as radio-base mais proximas, e informa id's e distancia*/
void closestRBase(char* text,Info* info){

    void* i;
    int cont = 0;
    Lista radios = KDT_getAll(info->bd->RadioBaseTree);
    int n = Lista_lenght(radios);
    Posic t;
    Point *points[n];
    insert_Fila(info->respQRY, "crb?\n");

    t=Lista_getFirst(radios);
    while(1){
        i = Lista_get(radios,t);
        if(i){
            
            Item it = Lista_get(radios, t);
            points[cont] = (Point*) calloc(1, sizeof(Point));
            *points[cont] = *(pointRB(it));
            cont++;
            t = Lista_getNext(radios, t);
        }else{
        break;
        }
    }

    char *rbases;
    rbases = (char*) calloc(155, sizeof(char));
    
    double dist = closest(points, n, rbases);    

    char *id1, *id2;
    id1 = (char*) calloc(155, sizeof(char));
    id2 = (char*) calloc(155, sizeof(char));
    sscanf(rbases, "%s %s", id1, id2);

    char* temp;
    temp = (char*) calloc(155, sizeof(char));
    sprintf(temp, "id: %s e %s  -   Dist: %lf", id1, id2, dist);
    insert_Fila(info->respQRY, temp);

    t=Lista_getFirst(radios);
    while(1){
        i = Lista_get(radios,t);
        if(i){
            char* temps;
            temps = getIdRadioB(i);
            if(!strcmp(temps, id1) || !strcmp(temps, id2)){
                circulo cr = getCircRadioB(i);
                double xt, yt;
                xt = getXCirc(cr);
                yt = getYCirc(cr);
                Notation nt = createNotacao("BLACK", 15, 0, xt, yt, "");
                Lista_insert(info->notsQRY, nt);
            }

            
            t = Lista_getNext(radios, t);
        }else{
        break;
        }
    
    }

}

void closeQRY(Info* info){
    int i;
    char* path, *aux;
    FILE *arqSVG_QRY, *arqTXT;

    void *auxN, *auxF, *t, *temp; 

    //configurar path e abrir arquivo
    path = (char*) calloc (255, sizeof(char));
    sprintf(path, "%s/%s", info->o, info->f);
    if(*path == '/') path++;
    aux = path;
    aux += strlen(path);
    while(*aux != '.')aux--;
    *aux = 0;
    aux = info->q;
    while(*aux){
        if(*aux == '/'){
            info->q = aux+1;
        }
        aux++;
    }
    sprintf(path, "%s-%s", path, info->q);
    aux = path;
    aux += strlen(path);
    while(*aux != '.')aux--;
    strcpy(aux, ".svg");
    arqSVG_QRY = fopen(path, "w");

    //imprimir circulos e retangulos
    fprintf(arqSVG_QRY,"<svg xmlns=\"http://www.w3.org/2000/svg\" width = \"5000\" height = \"5000\">\n");

    t=Lista_getFirst(info->bd->Drawer);
    while(1){
        temp = Lista_get(info->bd->Drawer,t);
        if(temp){
            
            Item it = Lista_get(info->bd->Drawer, t);
            fprintf(arqSVG_QRY, "%s\n", createSvg(it));
            t = Lista_getNext(info->bd->Drawer, t);
        }else{
        break;
        }
    }

    //Imprimir notas GEO
    auxF = create_Fila();
    while(!empty_Fila(info->notsGeo)){
        auxN = (Notation) remove_Fila(info->notsGeo);
        insert_Fila(auxF,(void*) auxN);
        fprintf(arqSVG_QRY, "%s\n", createNotacaoSvg(auxN));
    }
    free(info->notsGeo);
    info->notsGeo = auxF;

    //imprimir notas QRY
    auxF = create_Fila();
    while(!empty_Fila(info->notsQRY)){
        auxN = (Notation) remove_Fila(info->notsQRY);
        insert_Fila(auxF,(void*) auxN);
        fprintf(arqSVG_QRY, "%s\n", createNotacaoSvg(auxN));
    }
    free(info->notsQRY);
    info->notsQRY = auxF;

    //imprimir quadras
    Lista quadras = KDT_getAll(info->bd->QuadrasTree);
    t=Lista_getFirst(quadras);
    while(1){
        temp = Lista_get(quadras,t);
        if(temp){
            
            Item it = Lista_get(quadras, t);
            fprintf(arqSVG_QRY, "%s\n", createQuadraSvg(it));
            t = Lista_getNext(quadras, t);
        }else{
        break;
        }
    }

    //imprimir semaforos
    Lista semaforos = KDT_getAll(info->bd->SemaforosTree);
    t=Lista_getFirst(semaforos);
    while(1){
        temp = Lista_get(semaforos,t);
        if(temp){
            
            Item it = Lista_get(semaforos, t);
            fprintf(arqSVG_QRY, "%s\n", createSemaforoSvg(it));
            t = Lista_getNext(semaforos, t);
        }else{
        break;
        }
    }

    //imprimir hidrantes
    Lista hidrantes = KDT_getAll(info->bd->HidrantesTree);
    t=Lista_getFirst(hidrantes);
    while(1){
        temp = Lista_get(hidrantes,t);
        if(temp){
            
            Item it = Lista_get(hidrantes, t);
            fprintf(arqSVG_QRY, "%s\n", createHidranteSvg(it));
            t = Lista_getNext(hidrantes, t);
        }else{
        break;
        }
    }

    //imprimir torres-base
    Lista radios = KDT_getAll(info->bd->RadioBaseTree);
    t=Lista_getFirst(radios);
    while(1){
        temp = Lista_get(radios,t);
        if(temp){
            
            Item it = Lista_get(radios, t);
            fprintf(arqSVG_QRY, "%s\n", createRadioBSvg(it));
            t = Lista_getNext(radios, t);
        }else{
        break;
        }
    }

    fprintf(arqSVG_QRY,"</svg>");

    fclose(arqSVG_QRY);

    aux = path;
    aux += strlen(path);
    while(*aux != '.')aux--;
    strcpy(aux, ".txt");

    arqTXT = fopen(path, "w");


    while(!empty_Fila(info->respGEO)){
        aux = (char*) remove_Fila(info->respGEO);
        fprintf(arqTXT, "%s", aux);
    }


    while(!empty_Fila(info->respQRY)){
        aux = (char*) remove_Fila(info->respQRY);
        fprintf(arqTXT, "%s", aux);
    }
    freeFila(info->respQRY);
    fclose(arqTXT);
    free(path);

}

char* moradoresQuadra(char* cep, Info* info){
    void* ident = Pessoa_IdentEndereco(cep);
    Lista enderecos = getList_hashtable(info->bd->enderecoPessoa, ident);
    char *result;
    result = (char*) calloc(Lista_lenght(enderecos)*510, sizeof(char));
    strcpy(result, "");
    void* t=Lista_getFirst(enderecos);
    while(1){
        void* temp = Lista_get(enderecos,t);
        if(temp){
            void* pessoa = Pessoa_getPessoaEndereco(temp);
            if(pessoa!=NULL)
                strcat(result, Pessoa_relatorio(pessoa));
            t = Lista_getNext(enderecos, t);
        }else{
        break;
        }
    }
    return result;
}

void whoIsHere(char* text, Info* info){
    char* aux, *cep;
    aux = (char*) calloc(strlen(text)+2, sizeof(char));
    strcpy(aux, text);
    insert_Fila(info->respQRY, aux);
    insert_Fila(info->respQRY, "\n");
    aux = text;
    aux+=3;
    cep = (char*) calloc(1, sizeof(char));
    sscanf(aux, "%s", cep);
    char* result;
    result = moradoresQuadra(cep, info);
    insert_Fila(info->respQRY, result);
}

void whoIsInThisArea(char* text, Info* info){
    char* temp;
    temp = (char*) calloc (155, sizeof(char));
    strcpy(temp, text);
    insert_Fila(info->respQRY, temp);
    insert_Fila(info->respQRY, "\n");
    char *aux;
    double x, y , w, h;
    double xi, yi, xf, yf;
    void* i;
    Posic t;

    aux = text; aux += 4;
    sscanf(aux, "%lf %lf %lf %lf", &x, &y, &w, &h);

    /*info->quadras*/
    Lista quadras = KDT_getAll(info->bd->QuadrasTree);
    t=Lista_getFirst(quadras);
    while(1){
        i = Lista_get(quadras,t);
        if(i){

            Item it = Lista_get(quadras, t);
            rectangle rc = getRecQuad(it);                    

            xi = getXRec(rc);
            xf = xi + getWRec(rc);
            yi = getYRec(rc);
            yf = yi + getHRec(rc);

            if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                char* result;
                result = moradoresQuadra(getCepQuad(it), info);
                insert_Fila(info->respQRY, result);
            }                    
            
            t = Lista_getNext(quadras, t);
        }else{
        break;
        }
    }
}

void whoAreYou(char* text, Info* info){
    char* temp;
    temp = (char*) calloc (155, sizeof(char));
    strcpy(temp, text);
    insert_Fila(info->respQRY, temp);
    insert_Fila(info->respQRY, "\n");
    char *aux, *cpf;
    cpf = (char*) calloc(55, sizeof(char));
    aux = text; aux += 4;
    sscanf(aux, "%s", cpf);
    Pessoa *p = Pessoa_create(cpf, "", "", "", "");
    Pessoa pessoa = get_hashtable(info->bd->PessoaCepHash, p);
    double* cord = Pessoa_getCordGeo(pessoa, info);
    if(cord == NULL){
       char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s\n", Pessoa_relatorio(pessoa));
        insert_Fila(info->respQRY, result);
    }else{
        char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s (Cordenadas [%lf,%lf])\n", Pessoa_relatorio(pessoa), cord[0], cord[1]);
        insert_Fila(info->respQRY, result);
        Notation nt = createNotacao("RED", 5, 0, cord[0], cord[1], cpf);
        insert_Fila(info->notsQRY, nt);
    }
}

void whoAreYouEstab(char* text, Info* info){
    char* temp;
    temp = (char*) calloc (155, sizeof(char));
    strcpy(temp, text);
    insert_Fila(info->respQRY, temp);
    insert_Fila(info->respQRY, "\n");
    char *aux, *cnpj;
    cnpj = (char*) calloc(55, sizeof(char));
    aux = text; aux += 4;
    sscanf(aux, "%s", cnpj);
    Estab *e = Estab_create(cnpj, NULL, "", "", "", "");
    Estab estab = get_hashtable(info->bd->EstabHash, e);
    double* cord = Estab_getCordGeo(estab, info);
    if(cord == NULL){
       char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s\n", Estab_relatorio(estab));
        insert_Fila(info->respQRY, result);
    }else{
        char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s (Cordenadas [%lf,%lf])\n", Estab_relatorio(estab), cord[0], cord[1]);
        insert_Fila(info->respQRY, result);
        Notation nt = createNotacao("PURPLE", 5, 0, cord[0], cord[1], cnpj);
        insert_Fila(info->notsQRY, nt);
    }
}

/*Mãe, no céu tem Pão? e Morreu...*/
void bread(char* text, Info* info){
    char *aux, *cpf;
    aux = (char*) calloc (155, sizeof(char));
    strcpy(aux, text);
    insert_Fila(info->respQRY, aux);
    insert_Fila(info->respQRY, "\n");
    cpf = (char*) calloc(55, sizeof(char));
    aux = text; aux += 4;
    sscanf(aux, "%s", cpf);
    Pessoa *p = Pessoa_create(cpf, "", "", "", "");
    Pessoa pessoa = get_hashtable(info->bd->PessoaCepHash, p);
    if(pessoa == NULL){
        insert_Fila(info->respQRY, "Esta pessoa não existe\n");
        return;
    }
    double* cord = Pessoa_getCordGeo(pessoa, info);
    if(cord == NULL){
       char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s\n", Pessoa_relatorio(pessoa));
        insert_Fila(info->respQRY, result);
    }else{
        char* result;
        result = (char*) calloc(255, sizeof(char));
        sprintf(result, "%s\n", Pessoa_relatorio(pessoa));
        insert_Fila(info->respQRY, result);
        Notation nt = createNotacao("RED", 0, 1, cord[0], cord[1], cpf);
        insert_Fila(info->notsQRY, nt);
    }
    //remove
    remove_hashtable(info->bd->PessoaCepHash, pessoa);
    KDT_remove(info->bd->PessoaTree, pessoa);
    void* end = Pessoa_getEndereco(pessoa);
    if(end != NULL)
    remove_hashtable(info->bd->enderecoPessoa, end);
}


char* estabsQuadra(char* cep, Info* info, char* tipo){
    void* ident = Estab_IdentEndereco(cep);
    Lista enderecos = getList_hashtable(info->bd->enderecoEstab, ident);
    char *result;
    result = (char*) calloc(Lista_lenght(enderecos)*510, sizeof(char));
    strcpy(result, "");
    void* t=Lista_getFirst(enderecos);
    while(1){
        void* temp = Lista_get(enderecos,t);
        if(temp){
            void* estab = Estab_getEstabEndereco(temp);
            if(estab!=NULL && (tipo == NULL || strcmp(tipo, Estab_getTipoCod(estab)) == 0)){
                strcat(result, Estab_relatorio(estab));
                strcat(result, "\n");
            }
            t = Lista_getNext(enderecos, t);
        }else{
        break;
        }
    }
    return result;
}

void whatHaveHere(char* text, Info* info){
    char* aux, *cep;
    aux = (char*) calloc(strlen(text)+2, sizeof(char));
    strcpy(aux, text);
    insert_Fila(info->respQRY, aux);
    insert_Fila(info->respQRY, "\n");
    aux = text;
    aux+=5;
    cep = (char*) calloc(1, sizeof(char));
    sscanf(aux, "%s", cep);
    char* result;
    result = estabsQuadra(cep, info, NULL);
    insert_Fila(info->respQRY, result);
}

void whatHaveInThisArea(char* text, Info* info){
    char* temp;
    temp = (char*) calloc (155, sizeof(char));
    strcpy(temp, text);
    insert_Fila(info->respQRY, temp);
    insert_Fila(info->respQRY, "\n");
    char *aux, *tipo;
    double x, y , w, h;
    double xi, yi, xf, yf;
    void* i;
    Posic t;
    tipo = (char*) calloc(55, sizeof(char));

    aux = text; aux += 5;
    sscanf(aux, "%s %lf %lf %lf %lf", tipo, &x, &y, &w, &h);

    /*info->quadras*/
    Lista quadras = KDT_getAll(info->bd->QuadrasTree);
    t=Lista_getFirst(quadras);
    while(1){
        i = Lista_get(quadras,t);
        if(i){

            Item it = Lista_get(quadras, t);
            rectangle rc = getRecQuad(it);                    

            xi = getXRec(rc);
            xf = xi + getWRec(rc);
            yi = getYRec(rc);
            yf = yi + getHRec(rc);

            if(xi >= x && yi >= y && xf <= x+w && yf <= y+h){
                char* result;
                result = estabsQuadra(getCepQuad(it), info, tipo);
                insert_Fila(info->respQRY, result);
            }                    
            
            t = Lista_getNext(quadras, t);
        }else{
        break;
        }
    }
}