#include <stdio.h>
#include <stdlib.h>
#include "../Config/config.h"
#include "../Fila/fila.h"
#include "../Comands/executeGeo.h"
#include "../Comands/executeQry.h"
#include "../Comands/executeEc.h"
#include "../Comands/executePm.h"


int main(int argc, const char *argv[]){
    //configurações
    Info *info;
    Fila comandos;    
    info = configIn(argc, argv);
    

    comandos = getCommandsForGEO(info);    
    if(comandos)
    startGEO(comandos, info);

    comandos = getCommandsForEC(info); 
    if(comandos)  
    startEc(comandos, info);

    comandos = getCommandsForPM(info); 
    if(comandos)  
    startPm(comandos, info);

    comandos = getCommandsForQRY(info); 
    if(comandos)  
    startQRY(comandos, info);

    freeFila(comandos);
    freeConfig(info);       
    return 1;
}