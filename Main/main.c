#include <stdio.h>
#include <stdlib.h>
#include "../Config/config.h"
#include "../Fila/fila.h"
#include "../Comands/Geo/executeGeo.h"
#include "../Comands/Qry/executeQry.h"
#include "../Comands/Ec/executeEc.h"
#include "../Comands/Pm/executePm.h"


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