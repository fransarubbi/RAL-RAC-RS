#ifndef RAL_H_INCLUDED
#define RAL_H_INCLUDED
#define MRAL 358   //usando un p=0.84 
#include <stdlib.h>
#include "envios.h"


typedef struct{
    int cant;
    Deliveries dev[MRAL];
}rebAbLin;


//Colocar marcas de VIRGEN en todos los baldes
void initRAL(rebAbLin *ral){
    int i;
    for(i = 0; i < MRAL; i++){
        strcpy(ral->dev[i].code, VIRGEN);
    }
    ral->cant = 0;
}


/*
return 0 - Fracaso
return 1 - Exito
*/
int localizarRAL(rebAbLin *ral, char c[], int *position, float *costo){

    int hash, contador = 0, celda = -1;
    float costLoc = 0.0;
    hash = hashing(c, MRAL);

    while((contador < MRAL) && (strcmp(ral->dev[hash].code, VIRGEN) != 0) && (strcmp(ral->dev[hash].code, c) != 0)){
       
        if(celda == -1){   //Guardar la primer celda libre encontrada
            if(strcmp(ral->dev[hash].code, LIBRE) == 0){
                celda = hash;
            }
        }
        costLoc += 1;
        hash = (hash + 1) % MRAL;
        contador += 1;
    }

    if(contador < MRAL){
        costLoc += 1;
        if(strcmp(ral->dev[hash].code, VIRGEN) != 0){
            *position = hash;
            return 0;
        }
        else{
            if(celda != -1){
                *position = celda;
                return 0;
            }
            else{
                *position = hash;
                return 0;
            }
        }
    }
    else{
        if(celda != -1){
            *position = celda;
            return 0;
        }
        return 1;
    }
}


int altaRAL(rebAbLin *ral, Deliveries dev){

    float costLoc = 0.0;
    int position;

    if(localizarRAL(ral, dev.code, &position, &costLoc) == 0){
        
    }

}


#endif