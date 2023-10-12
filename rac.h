#ifndef RAC_H_INCLUDED
#define RAC_H_INCLUDED
#define MRAC 401  //usando p=0.75, es el primer numero primo mas grande que 400
#include <stdlib.h>
#include "envios.h"


typedef struct{
    int cant;
    Deliveries dev[MRAC];
}rebAbCua;


//Colocar marcas de VIRGEN en todos los baldes
void initRAC(rebAbCua *rac){
    int i;
    for(i = 0; i < MRAC; i++){
        strcpy(rac->dev[i].code, VIRGEN);
    }
    rac->cant = 0;
}


int localizarRAC(rebAbCua *rac, char c[], int *position, float *costo){

    int i, contador = 0, celda = -1;
    float costLoc = 0.0;
    i = hashing(c, MRAC);

    while((contador < MRAC) && (strcmp(rac->dev[i].code, VIRGEN) != 0) && (strcmp(rac->dev[i].code, c) != 0)){
        if(celda == -1){
            if(strcmp(rac->dev[i].code, LIBRE) == 0){
                celda = i;
            }
            costLoc += 1;
            i = i + contador;
            contador += 1;
        }
    }

    if(contador < MRAC){
        costLoc += 1;
        *costo = costLoc;
        if(strcmp(rac->dev[i].code, c) == 0){
            *position = i;
            return 1;
        }
        if(strcmp(rac->dev[i].code, VIRGEN) != 0){
            *position = i;
            return 0; 
        }
        else{
            if(celda != -1){
                *position = celda;
                return 0;
            }
            else{
                *position = i;
                return 0;
            }
        }
    }
    else{
        if(celda != 1){
            *position = celda;
            return 0;
        }
        return 1;
    }
}


int altaRAC(rebAbCua *rac, Deliveries dev){

    int position, hash;
    float costLoc;

    if(localizarRAC(rac, dev.code, &position, &costLoc) == 0){
        rac->dev[position] = dev;
        rac->cant += 1;
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
int bajaRAC(rebAbCua *rac, Deliveries dev){

    int position;
    float costLoc = 0.0;

    if(localizarRAC(rac, dev.code, &position, &costLoc) == 1){
        int a = strcmp((dev).code, rac->dev[position].code);
        int b = strcmp((dev).name, rac->dev[position].name);
        int c = strcmp((dev).nameSender, rac->dev[position].nameSender);
        int d = strcmp((dev).address, rac->dev[position].address);
        int e = strcmp((dev).dateSender, rac->dev[position].dateSender);
        int f = strcmp((dev).dateReceived, rac->dev[position].dateReceived);
        if(a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && (dev.doc == rac->dev[position].doc) && (dev.docSender == rac->dev[position].docSender)){
            strcpy(rac->dev[position].code, LIBRE);
            rac->cant -= 1;
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
int evocacionRAC(rebAbCua rac, Deliveries *dev, float *costo){

    int position;
    float costLoc = 0.0;
    if(localizarRAC(&rac, dev->code, &position, &costLoc) == 0){
        *costo = costLoc;
        return 0;
    }
    else{
        strcpy((*dev).code, rac.dev[position].code);
        (*dev).doc = rac.dev[position].doc;
        (*dev).docSender = rac.dev[position].docSender;
        strcpy((*dev).name, rac.dev[position].name);
        strcpy((*dev).nameSender, rac.dev[position].nameSender);
        strcpy((*dev).address, rac.dev[position].address);
        strcpy((*dev).dateSender, rac.dev[position].dateSender);
        strcpy((*dev).dateReceived, rac.dev[position].dateReceived);
        *costo = costLoc;
        return 1;
    }
}


void mostrarRAC(rebAbCua rac){

    int i;

    for(i = 0; i < MRAC; i++){
        if(strcmp(rac.dev[i].code, VIRGEN) == 0){
            printf("| Mostrando balde: %d. Celda Virgen\n", i);
        }
        if(strcmp(rac.dev[i].code, LIBRE) == 0){
            printf("| Mostrando balde: %d. Celda Libre\n", i);
        }
        if(strcmp(rac.dev[i].code, VIRGEN) != 0){
            if(strcmp(rac.dev[i].code, LIBRE) != 0){
                printf("\n| Mostrando Elemento del Balde: %d\n", i);
                printf("\n|========================================|");
                printf("\n| Codigo: %s", rac.dev[i].code);
                printf("\n| Dni receptor: %ld", rac.dev[i].doc);
                printf("\n| Dni remitente: %ld", rac.dev[i].docSender);
                printf("\n| Nombre y apellido del receptor: %s", rac.dev[i].name);
                printf("\n| Nombre y apellido del remitente: %s", rac.dev[i].nameSender);
                printf("\n| Domicilio del envio: %s", rac.dev[i].address);
                printf("\n| Fecha de envio: %s", rac.dev[i].dateSender);
                printf("\n| Fecha de recepcion: %s", rac.dev[i].dateReceived);
                printf("\n|========================================|\n\n");
            }
        }
    }
}


#endif