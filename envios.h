#ifndef DELIVERIES_H_INCLUDED
#define DELIVERIES_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define CODE 8
#define NAME 81
#define DATE 11


typedef struct{
    
    float maxCostSucEvo;   //Costo maximo Exito evocacion
    float medCostSucEvo;   //Costo medio Exito evocacion
    float costAcumSucEvo;  //Costo acumulado exito evocacion
    int cantSucEvo;        //Cantidad de costos exito evocacion

    float maxCostFailEvo;  //Costo maximo Fracaso evocacion
    float medCostFailEvo;  //Costo medio Fracaso evocacion
    float costAcumFailEvo; //Costo acumulado fracaso evocacion
    int cantFailEvo;       //Cantidad de costos fracaso evocacion
}Cost;


typedef struct{
   Cost rs;
   Cost ral; 
   Cost rac;
}StructCost;


void initStructCost(StructCost *st){
    
    st->rs.maxCostSucEvo = 0;
    st->rs.medCostSucEvo = 0;
    st->rs.maxCostFailEvo = 0;
    st->rs.medCostFailEvo = 0;
    st->rs.costAcumSucEvo = 0;
    st->rs.costAcumFailEvo = 0;
    st->rs.cantSucEvo = 0;
    st->rs.cantFailEvo = 0;

    st->ral.maxCostSucEvo = 0;
    st->ral.medCostSucEvo = 0;
    st->ral.maxCostFailEvo = 0;
    st->ral.medCostFailEvo = 0; 
    st->ral.costAcumSucEvo = 0;
    st->ral.costAcumFailEvo = 0;
    st->ral.cantSucEvo = 0;
    st->ral.cantFailEvo = 0; 
    
    st->rac.maxCostSucEvo = 0;
    st->rac.medCostSucEvo = 0;
    st->rac.maxCostFailEvo = 0;
    st->rac.medCostFailEvo = 0;  
    st->rac.costAcumSucEvo = 0;
    st->rac.costAcumFailEvo = 0;
    st->rac.cantSucEvo = 0;
    st->rac.cantFailEvo = 0;
}

typedef struct{
    char code[CODE];
    long doc, docSender;
    char name[NAME];
    char nameSender[NAME];
    char address[NAME];
    char dateSender[DATE];
    char dateReceived[DATE];
}Deliveries;


int hashing(char c[], int M){
    
    int longitud, i, contador = 0;
    longitud = strlen(c);
    for(i = 0; i < longitud; i++){
        contador += ((int)c[i]) * (i + 1);
    }
    return (contador % M);
}


#endif