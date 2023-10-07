#ifndef RS_H_INCLUDED
#define RS_H_INCLUDED
#define MRS 437  //numero primo proximo a 435
#include <stdlib.h>
#include "envios.h"


typedef struct balde{
    Deliveries dev;
    struct balde *prox;
}balde;


typedef struct{
    int cant;
    balde *access[MRS];
    balde *antCursor;
    balde *cursor;
}rebSep;


//Inicializacion del rebalse separado
void initRebSep(rebSep *rs){

    int i;
    for(i = 0; i < MRS; i++){
        rs->access[i] = NULL;
    }
    rs->cant = 0;
}


//Resetear rebalse para comparar estructuras
void resetRebSep(rebSep *rs){

    int i;
    for(i = 0; i < MRS; i++){
        rs->cursor = rs->access[i];
        while(rs->cursor != NULL){
            rs->antCursor = rs->cursor;
            rs->cursor = rs->cursor->prox;
            free((void*)(rs->antCursor));
        }
        rs->access[i] = NULL;
    }
}


/*
Codificacion de Localizacion:
return 0 - Fracaso, no esta el elemento
return 1 - Exito
*/
int localizarRebSep(rebSep *rs, char c[], int *position, float *costo){

    float costLoc = 0.0;
    *position = hashing(c, MRS);
    rs->antCursor = rs->access[*position];
    rs->cursor = rs->access[*position];

    while((rs->cursor != NULL) && (strcmp(rs->cursor->dev.code, c) != 0)){
        rs->antCursor = rs->cursor;
        rs->cursor = rs->cursor->prox;
        costLoc += 1;
    }

    *costo = costLoc;
    if(rs->cursor != NULL){
        return 1;
    }
    else{
        return 0;
    }
}


/*
Codificacion de Alta:
return 0 - Fracaso, no hay mas memoria
return 1 - Fracaso, elemento existente
return 2 - Exito
*/
int altaRebSep(rebSep *rs, Deliveries dev){

    int position;
    float costLoc;
    balde *newBalde = (balde*)malloc(sizeof(balde));

    if(newBalde == NULL){
        return 0;
    }
    else{
        if(localizarRebSep(rs, dev.code, &position, &costLoc) == 1){
            free((void*)newBalde);
            return 1;
        }
        else{
            newBalde->dev = dev;
            newBalde->prox = rs->access[position];
            rs->access[position] = newBalde;
            rs->cant += 1;
            return 2;
        }
    }
}


/*
Codificacion de Baja:
return 0 - Fracaso, elemento inexistente
return 1 - Fracaso, no coincide la nupla completa
return 2 - Exito
*/
int bajaRebSep(rebSep *rs, Deliveries dev){

    int position;
    float costLoc;
    if(localizarRebSep(rs, dev.code, &position, &costLoc) == 0){
        return 0;
    }
    else{
        int a = strcmp((dev).code, rs->cursor->dev.code);
        int b = strcmp((dev).name, rs->cursor->dev.name);
        int c = strcmp((dev).nameSender, rs->cursor->dev.nameSender);
        int d = strcmp((dev).address, rs->cursor->dev.address);
        int e = strcmp((dev).dateSender, rs->cursor->dev.dateSender);
        int f = strcmp((dev).dateReceived, rs->cursor->dev.dateReceived);
        if(a == 0 && b == 0 && c == 0 && d == 0 && e == 0 && f == 0 && (dev.doc == rs->cursor->dev.doc) && (dev.docSender == rs->cursor->dev.docSender)){
            if(rs->cursor == rs->access[position]){
                rs->access[position] = rs->cursor->prox;
            }
            else{
                rs->antCursor->prox = rs->cursor->prox;
            }
            free((void*)rs->cursor);
            rs->cant = rs->cant - 1;
            return 2;
        }
        else{
            return 1;
        }
    }
}


/*
Codificacion de Evocacion:
return 0 - Fracaso, elemento inexistente
return 1 - Exito
*/
int evocacionRebSep(rebSep rs, Deliveries *dev, float *costo){

    int position;
    float costLoc;
    if(localizarRebSep(&rs, (*dev).code, &position, &costLoc) == 0){
        *costo = costLoc;
        return 0;
    }
    else{
        strcpy((*dev).code, rs.cursor->dev.code);
        (*dev).doc = rs.cursor->dev.doc;
        (*dev).docSender = rs.cursor->dev.docSender;
        strcpy((*dev).name, rs.cursor->dev.name);
        strcpy((*dev).nameSender, rs.cursor->dev.nameSender);
        strcpy((*dev).address, rs.cursor->dev.address);
        strcpy((*dev).dateSender, rs.cursor->dev.dateSender);
        strcpy((*dev).dateReceived, rs.cursor->dev.dateReceived);
        *costo = costLoc;
        return 1;
    }
}


void mostrarRs(rebSep rs){

    int i;
    for(i = 0; i < MRS; i++){
        rs.cursor = rs.access[i];
        if(rs.cursor != NULL){
            while(rs.cursor != NULL){
                printf("\n|========================================|");
                printf("\n| Codigo: %s", rs.cursor->dev.code);
                printf("\n| Dni receptor: %ld", rs.cursor->dev.doc);
                printf("\n| Dni remitente: %ld", rs.cursor->dev.docSender);
                printf("\n| Nombre y apellido del receptor: %s", rs.cursor->dev.name);
                printf("\n| Nombre y apellido del remitente: %s", rs.cursor->dev.nameSender);
                printf("\n| Domicilio del envio: %s", rs.cursor->dev.address);
                printf("\n| Fecha de envio: %s", rs.cursor->dev.dateSender);
                printf("\n| Fecha de recepcion: %s", rs.cursor->dev.dateReceived);
                printf("\n|========================================|\n\n");
                rs.cursor = rs.cursor->prox;
            }
        }
        else{
            printf("La lista esta vacia. Hashing: %d\n", i);
        }
    }
}

#endif