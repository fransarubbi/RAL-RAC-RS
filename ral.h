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

    int i, hash, contador = 0, celda = -1;
    float costLoc = 0.0;
    i = hashing(c, MRAL);

    while((contador < MRAL) && (strcmp(ral->dev[i].code, VIRGEN) != 0) && (strcmp(ral->dev[i].code, c) != 0)){
       
        if(celda == -1){   //Guardar la primer celda libre encontrada
            if(strcmp(ral->dev[i].code, LIBRE) == 0){
                celda = i;
            }
        }
        costLoc += 1;
        i = (i + 1) % MRAL;
        contador += 1;
    }

    if(contador < MRAL){    //No se ha recorrido todos los baldes
        costLoc += 1;
        *costo = costLoc;
        if(strcmp(ral->dev[i].code, VIRGEN) != 0){   //Exito en la busqueda
            *position = i;
            return 1;
        }
        else{     //No tuvo exito, pero hay una celda virgen donde puede ir el elemento
            if(celda != -1){     //Si habia una celda libre, retornar esa posicion para cargar el elemento
                *position = celda;
                return 0;
            }
            else{     //No habia celdas libres, retornar posicion con celda virgen
                *position = i;
                return 0;
            }
        }
    }
    else{     //Se recorrieron todos los baldes. Fracaso
        return 0;
    }
}


/*
return 0 - Fracaso
return 1 - Exito
*/
int altaRAL(rebAbLin *ral, Deliveries dev){

    float costLoc = 0.0;
    int position;

    if(localizarRAL(ral, dev.code, &position, &costLoc) == 0){
        ral->dev[position] = dev;
        ral->cant += 1;
        return 1;
    }
    else{
        return 0;
    }
}


/*
return 0 - Fracaso
return 1 - Exito
*/
int bajaRAL(rebAbLin *ral, Deliveries dev){

    int position;
    float costLoc = 0.0;

    if(localizarRAL(ral, dev.code, &position, &costLoc) == 1){
        int a = strcmp((dev).code, ral->dev[position].code);
        int b = strcmp((dev).name, ral->dev[position].name);
        int c = strcmp((dev).nameSender, ral->dev[position].nameSender);
        int d = strcmp((dev).address, ral->dev[position].address);
        int e = strcmp((dev).dateSender, ral->dev[position].dateSender);
        int f = strcmp((dev).dateReceived, ral->dev[position].dateReceived);
        if(a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && (dev.doc == ral->dev[position].doc) && (dev.docSender == ral->dev[position].docSender)){
            strcpy(ral->dev[position].code, LIBRE);
            ral->cant -= 1;
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}


/*
return 0 - Fracaso
return 1 - Exito
*/
int evocacionRAL(rebAbLin ral, Deliveries *dev, float *costo){

    int position;
    float costLoc = 0.0;
    if(localizarRAL(&ral, dev->code, &position, &costLoc) == 0){
        *costo = costLoc;
        return 0;
    }
    else{
        strcpy((*dev).code, ral.dev[position].code);
        (*dev).doc = ral.dev[position].doc;
        (*dev).docSender = ral.dev[position].docSender;
        strcpy((*dev).name, ral.dev[position].name);
        strcpy((*dev).nameSender, ral.dev[position].nameSender);
        strcpy((*dev).address, ral.dev[position].address);
        strcpy((*dev).dateSender, ral.dev[position].dateSender);
        strcpy((*dev).dateReceived, ral.dev[position].dateReceived);
        *costo = costLoc;
        return 1;
    }
}


void mostrarRAL(rebAbLin ral){

    int i;

    for(i = 0; i < MRAL; i++){
        if(strcmp(ral.dev[i].code, VIRGEN) == 0){
            printf("| Mostrando balde: %d. Celda Virgen\n", i);
        }
        if(strcmp(ral.dev[i].code, LIBRE) == 0){
            printf("| Mostrando balde: %d. Celda Libre\n", i);
        }
        if(strcmp(ral.dev[i].code, VIRGEN) != 0){
            if(strcmp(ral.dev[i].code, LIBRE) != 0){
                printf("\n| Mostrando Elemento del Balde: %d\n", i);
                printf("\n|========================================|");
                printf("\n| Codigo: %s", ral.dev[i].code);
                printf("\n| Dni receptor: %ld", ral.dev[i].doc);
                printf("\n| Dni remitente: %ld", ral.dev[i].docSender);
                printf("\n| Nombre y apellido del receptor: %s", ral.dev[i].name);
                printf("\n| Nombre y apellido del remitente: %s", ral.dev[i].nameSender);
                printf("\n| Domicilio del envio: %s", ral.dev[i].address);
                printf("\n| Fecha de envio: %s", ral.dev[i].dateSender);
                printf("\n| Fecha de recepcion: %s", ral.dev[i].dateReceived);
                printf("\n|========================================|\n\n");
            }
        }
    }
}


#endif