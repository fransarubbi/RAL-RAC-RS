#include <stdio.h>
#include "rs.h"
#include "ral.h"
#include "rac.h"


/*
Conclusiones:
    |-------------------------------------------|   |-------------------------------------------|   |-------------------------------------------|    
    |             Rebalse Separado              |   |           Rebalse Abierto Lineal          |   |         Rebalse Abierto Cuadratico        |
    |-------------------------------------------|   |-------------------------------------------|   |-------------------------------------------|
    |   Funcion   |  Costo Max  |  Costo Medio  |   |   Funcion   |  Costo Max  |  Costo Medio  |   |   Funcion   |  Costo Max  |  Costo Medio  |
    |-------------------------------------------|   |-------------------------------------------|   |-------------------------------------------|
    |  Evoc. Exit |    5.00     |     2.29      |   |  Evoc. Exit |     6.00    |      1.68     |   |  Evoc. Exit |     7.00    |     1.72      |
    |-------------------------------------------|   |-------------------------------------------|   |-------------------------------------------|
    |  Evoc. Frac |    4.00     |     1.38      |   |  Evoc. Frac |     9.00    |      3.87     |   |  Evoc. Frac |     11.00   |     3.56      |
    |-------------------------------------------|   |-------------------------------------------|   |-------------------------------------------|

    Para realizar el analisis de las tres estructuras, solo se compara el rendimiento en evocaciones, ignorando por completo el costo de altas y bajas. 
    Realizando un analisis del peor de los casos:

        - RS: en el rebalse separado, el peor caso se produce cuando tenemos una mala funcion de hashing. Esto significa, que la funcion siempre retorna
        el mismo valor de hash, y por lo tanto se produce una unica lista vinculada desordenada. Como resultado, tenemos un costo maximo tanto de exito
        como de fracaso, orden O(N).

        -RAL: en el rebalse abierto lineal, el peor caso se produce cuando todos los baldes tienen elementos, excepto el balde anterior al valor de hash
        para un elemento dado. Esto resulta en que se deben recorrer todos los baldes.

        -RAC: en el rebalse abierto cuadratico, el peor caso se produce cuando el elemento se encuentra en una posicion impar. Esto resulta en que se
        deben recorrer los baldes, hasta que se consulte esa posicion.

    Podemos ver en base a los resultados, que la estructura que tiene el mejor costo maximo exitoso, es el rebalse separado. Sin embargo, el costo medio
    exitoso mas bajo, lo presenta el rebalse abierto lineal. Ademas, el rebalse separado tiene el mejor comportamiento en las evocaciones que fracasan.
    Para poder elegir una de las tres, la preferencia la tendria el rebalse separado, ya que haciendo un balance es la de mejor rendimiento. Pero depende
    de la situacion, si se sabe que se realizaran evocaciones que fracasan con frecuencia, lo mejor seria un RS. En otro caso, si la preferencia la tiene
    el costo medio de evocacion exitosa, la mejor seria el RAL.
*/


//Funciones menu principal
void mostrarEstructuras(rebSep , rebAbLin , rebAbCua);   //Mostrar las 3 estructuras
int lecturaOperaciones(StructCost *, rebSep *, rebAbLin *, rebAbCua *);  //Funcion para leer desde el archivo
void mostrarCostos(StructCost );


//Funciones internas
void loadDeliveries(Deliveries *);   //Funcion auxiliar de carga de datos
void resetAll(rebSep *, rebAbLin *, rebAbCua *);   //Funcion para resetear todas las estructuras


int main(){
    
    int menu, lect;
    rebSep rs;
    rebAbLin ral;
    rebAbCua rac;
    Deliveries dev;
    StructCost c;

    initRS(&rs);
    initRAL(&ral);
    initRAC(&rac);
    initStructCost(&c);

    do{
        do{
            printf("\n|===========================================|");
            printf("\n|                BIENVENIDOS!               |");
            printf("\n|              ENVIOS EL REVOLEO            |");
            printf("\n|===========================================|");
            printf("\n|              MENU DE OPCIONES             |");
            printf("\n|===========================================|");
            printf("\n|         1. Comparar estructuras           |");
            printf("\n|         2. Mostrar estructuras            |");
            printf("\n|         3. Mostrar costos cargados        |");
            printf("\n|         4. Salir del sistema              |");
            printf("\n|===========================================|\n\n");
            scanf("%d", &menu);
        }while(menu < 1 || menu > 4);

        switch(menu){
            case 1: resetAll(&rs, &ral, &rac);
                    initStructCost(&c);
                    lect = lecturaOperaciones(&c, &rs, &ral, &rac);
                    if(lect == 1){
                        mostrarCostos(c);
                    }
                    else{
                        printf("\n|-------------------------------------------------------------------|");
                        printf("\n| No hay costos cargados. No se ha realizado la lectura por archivo |");
                        printf("\n|-------------------------------------------------------------------|");
                    }
                    break;

            case 2: mostrarEstructuras(rs, ral, rac);
                    break;

            case 3: mostrarCostos(c);
                    break;
        }

    }while(menu != 4);

    printf("\n|===========================================|");
    printf("\n|       GRACIAS POR USAR EL SISTEMA         |");
    printf("\n|             ENVIOS EL REVOLEO             |");
    printf("\n|===========================================|");
    printf("\n|      Sarubbi Franco - Orozco Mateo        |");
    printf("\n|===========================================|\n\n");
    return 0;
}



//Funciones internas
void loadDeliveries(Deliveries *dev){

    int i;
    long d;
    char c[CODE], n[NAME], date[DATE];

    printf("|---------------------------------------------|\n");
    printf("| Ingrese el codigo correspondiente al envio: |\n");
    scanf("%s", c);
    for(i = 0; c[i] != '\0'; i++){
        c[i] = toupper(c[i]);
    }
    strcpy(dev->code, c);

    do{
        printf("|------------------------------------|\n");
        printf("| Ingrese el documento del receptor: |\n");
        scanf("%ld", &d);
    }while(d < 0 || d > 99999999);  //rango de documento entre 4.000.000 hasta 50.000.000
    dev->doc = d;

    do{
        printf("|----------------------------------|\n");
        printf("| Ingrese el documento del emisor: |\n");
        scanf("%ld", &d);
    }while(d < 0 || d > 99999999);  //rango de documento entre 4.000.000 hasta 50.000.000
    dev->docSender = d;

    printf("|---------------------------------|\n");
    printf("| Ingrese el nombre del receptor: |\n");
    scanf(" %[^\n]", n);
    for(i = 0; n[i] != '\0'; i++){
        n[i] = toupper(n[i]);
    }
    strcpy(dev->name, n);

    printf("|-------------------------------|\n");
    printf("| Ingrese el nombre del emisor: |\n");
    scanf(" %[^\n]", n);
    for(i = 0; n[i] != '\0'; i++){
        n[i] = toupper(n[i]);
    }
    strcpy(dev->nameSender, n);

    printf("|-----------------------------------|\n");
    printf("|  Ingrese la direccion del envio:  |\n");
    scanf(" %[^\n]", n);
    for(i = 0; n[i] != '\0'; i++){
        n[i] = toupper(n[i]);
    }
    strcpy(dev->address, n);

    printf("|------------------------------|\n");
    printf("|  Ingrese la fecha de envio:  |\n");
    scanf(" %[^\n]", date);
    for(i = 0; date[i] != '\0'; i++){
        date[i] = toupper(date[i]);
    }
    strcpy(dev->dateSender, date);

    printf("|----------------------------------|\n");
    printf("|  Ingrese la fecha de recepcion:  |\n");
    scanf(" %[^\n]", date);
    for(i = 0; date[i] != '\0'; i++){
        date[i] = toupper(date[i]);
    }
    strcpy(dev->dateReceived, date);
}


void resetAll(rebSep *rs, rebAbLin *ral, rebAbCua *rac){
    resetRS(rs);
    initRAL(ral);
    initRAC(rac);
}


//Funciones del menu
void mostrarEstructuras(rebSep rs, rebAbLin ral, rebAbCua rac){
    
    int enter, opcion;
    do{
        do{
            printf("\n|------------------------------------------------------------|");
            printf("\n|                    Mostrando Estructuras                   |");
            printf("\n|------------------------------------------------------------|");
            printf("\n|  0. Salir de Mostrar Estructuras                           |");
            printf("\n|  1. Mostrar Estructuras                                    |");
            printf("\n|------------------------------------------------------------|\n");
            scanf("%d", &enter);
        }while(enter < 0 || enter > 1);

        if(enter == 1){
            
            do{
                printf("\n|-----------------------------------------------------|");
                printf("\n|  1. Mostrar Rebalse Separado                        |");
                printf("\n|  2. Mostrar Rebalse Abierto Lineal                  |");
                printf("\n|  3. Mostrar Rebalse Abierto Cuadratico              |");
                printf("\n|-----------------------------------------------------|\n");
                scanf("%d", &opcion);
            }while(opcion < 1 || opcion > 3);

            switch(opcion){
                case 1: if(rs.cant == 0){
                            printf("\n|----------------------------------------------------|");
                            printf("\n| No se pueden mostrar datos. Rebalse Separado Vacio |");
                            printf("\n|----------------------------------------------------|\n");
                        }
                        else{
                            printf("\n\n      Mostrando %d elementos\n", rs.cant);
                            mostrarRS(rs);
                        }
                        break;

                case 2: if(ral.cant == 0){
                            printf("\n|----------------------------------------------------------|");
                            printf("\n| No se pueden mostrar datos. Rebalse Abierto Lineal Vacio |");
                            printf("\n|---------------------------------------------------------|\n");
                        }
                        else{
                            printf("\n\n      Mostrando %d elementos\n", ral.cant);
                            mostrarRAL(ral);
                        }
                        break;

                case 3: if(rac.cant == 0){
                            printf("\n|--------------------------------------------------------------|");
                            printf("\n| No se pueden mostrar datos. Rebalse Abierto Cuadratico Vacio |");
                            printf("\n|--------------------------------------------------------------|\n");
                        }
                        else{
                            printf("\n\n      Mostrando %d elementos\n", rac.cant);
                            mostrarRAC(rac);
                        }
                        break;
            }
        }

    }while(enter != 0);
}


void mostrarCostos(StructCost c){

    printf("\n|------------------------------------------------------|");
    printf("\n|          Mostrando Costos de las Estructuras         |");
    printf("\n|------------------------------------------------------|\n");
    printf("\n|------------------------------------------------------|");
    printf("\n|                   Rebalse Separado                   |");
    printf("\n|------------------------------------------------------|");
    printf("\n|    Funcion    |    Costo Max     |     Costo Medio   |");
    printf("\n|------------------------------------------------------|");
    printf("\n| Evoc. Exitosa |   %.2f                %.2f              ", c.rs.maxCostSucEvo, c.rs.medCostSucEvo);
    printf("\n|------------------------------------------------------");
    printf("\n| Evoc. Fracaso |   %.2f                %.2f              ", c.rs.maxCostFailEvo, c.rs.medCostFailEvo);
    printf("\n|------------------------------------------------------|\n");

    printf("\n|------------------------------------------------------|");
    printf("\n|                Rebalse Abierto Lineal                |");
    printf("\n|------------------------------------------------------|");
    printf("\n|    Funcion    |    Costo Max     |     Costo Medio   |");
    printf("\n|------------------------------------------------------|");
    printf("\n| Evoc. Exitosa |   %.2f                %.2f              ", c.ral.maxCostSucEvo, c.ral.medCostSucEvo);
    printf("\n|------------------------------------------------------");
    printf("\n| Evoc. Fracaso |   %.2f                %.2f              ", c.ral.maxCostFailEvo, c.ral.medCostFailEvo);
    printf("\n|------------------------------------------------------|\n");

    printf("\n|----------------------------------------------------------|");
    printf("\n|                Rebalse Abierto Cuadratico                |");
    printf("\n|----------------------------------------------------------|");
    printf("\n|     Funcion     |     Costo Max      |     Costo Medio   |");
    printf("\n|----------------------------------------------------------|");
    printf("\n| Evoc. Exitosa   |   %.2f                 %.2f              ", c.rac.maxCostSucEvo, c.rac.medCostSucEvo);
    printf("\n|----------------------------------------------------------");
    printf("\n| Evoc. Fracaso   |   %.2f                %.2f              ", c.rac.maxCostFailEvo, c.rac.medCostFailEvo);
    printf("\n|----------------------------------------------------------|\n");
}


int lecturaOperaciones(StructCost *c, rebSep *rs, rebAbLin *ral, rebAbCua *rac){

    Deliveries dev;
    char code[CODE], name[NAME], nameSender[NAME], addres[NAME], dateS[DATE], dateR[DATE];
    long dni, dniS;
    int value, enter, i, codeOperator, baja = 1;
    float costo;

    FILE *preload;
    preload = fopen("Operaciones-Envios.txt", "r");

    if(preload == NULL){
        printf("|----------------------------------------------|\n");
        printf("|       No se pudo acceder al archivo          |\n");
        printf("|----------------------------------------------|\n\n");
        return 0;
    }
    else{ 
        while(!feof(preload)){

            fscanf(preload, "%d\n", &codeOperator);

            fscanf(preload, " %[^\n]\n", code);
            for(i = 0; code[i] != '\0'; i++){
                code[i] = toupper(code[i]);
            }
            strcpy(&dev.code, code);

            if(codeOperator == 1 || codeOperator == 2){
                
                fscanf(preload, "%ld\n", &dni);
                dev.doc = dni;
            
                fscanf(preload, " %[^\n]\n", nameSender);
                for(i = 0; nameSender[i] != '\0'; i++){
                    nameSender[i] = toupper(nameSender[i]);
                }
                strcpy(&dev.nameSender, nameSender);
            
                fscanf(preload, " %[^\n]\n", addres);
                for(i = 0; addres[i] != '\0'; i++){
                    addres[i] = toupper(addres[i]);
                }
                strcpy(&dev.address, addres);
            
                fscanf(preload, "%ld\n", &dniS);
                dev.docSender = dniS;

                fscanf(preload, " %[^\n]\n", name);
                for(i = 0; name[i] != '\0'; i++){
                    name[i] = toupper(name[i]);
                }
                strcpy(&dev.name, name);
            
                fscanf(preload, " %[^\n]\n", dateS);
                for(i = 0; dateS[i] != '\0'; i++){
                    dateS[i] = toupper(dateS[i]);
                }
                strcpy(&dev.dateSender, dateS);

                fscanf(preload, " %[^\n]\n", dateR);
                for(i = 0; dateR[i] != '\0'; i++){
                    dateR[i] = toupper(dateR[i]);
                }
                strcpy(&dev.dateReceived, dateR);

                switch(codeOperator){
                    case 1: value = altaRS(rs, dev);
                            value = altaRAL(ral, dev);
                            value = altaRAC(rac, dev);
                            break;


                    case 2: if(rs->cant != 0){
                                value = bajaRS(rs, dev);
                            }
                            if(ral->cant != 0){
                                value = bajaRAL(ral, dev);
                            }
                            if(rac->cant != 0){
                                value = bajaRAC(rac, dev);
                            }
                            break;
                }
            }
            else{
                if(codeOperator == 3){
                    costo = 0.0;
                    value = evocacionRS(*rs, &dev, &costo);
                    if(value == 1){
                        if(c->rs.maxCostSucEvo < costo){
                            c->rs.maxCostSucEvo = costo;
                        }
                        c->rs.cantSucEvo += 1;
                        c->rs.costAcumSucEvo += costo;
                    }
                    else{
                        if(c->rs.maxCostFailEvo < costo){
                            c->rs.maxCostFailEvo = costo;
                        }
                        c->rs.cantFailEvo += 1;
                        c->rs.costAcumFailEvo += costo;
                    }

                    costo = 0.0;
                    value = evocacionRAL(*ral, &dev, &costo);
                    if(value == 1){
                        if(c->ral.maxCostSucEvo < costo){
                            c->ral.maxCostSucEvo = costo;
                        }
                        c->ral.cantSucEvo += 1;
                        c->ral.costAcumSucEvo += costo;
                        
                    }
                    else{
                        if(c->ral.maxCostFailEvo < costo){
                            c->ral.maxCostFailEvo = costo;
                        }
                        c->ral.cantFailEvo += 1;
                        c->ral.costAcumFailEvo += costo;
                    }
                    
                    costo = 0.0;
                    value = evocacionRAC(*rac, &dev, &costo);
                    if(value == 1){
                        if(c->rac.maxCostSucEvo < costo){
                            c->rac.maxCostSucEvo = costo;
                        }
                        c->rac.cantSucEvo += 1;
                        c->rac.costAcumSucEvo += costo;
                    }
                    else{
                        if(c->rac.maxCostFailEvo < costo){
                            c->rac.maxCostFailEvo = costo;
                        }
                        c->rac.cantFailEvo += 1;
                        c->rac.costAcumFailEvo += costo;
                    }
                }
                else{
                    printf("\n|----------------------------------------|");
                    printf("\n| Error. Codigo de operacion no conocido |");
                    printf("\n|----------------------------------------|");
                }
            }
            codeOperator = 0;
        }
        fclose(preload);

        c->rs.medCostSucEvo= (c->rs.costAcumSucEvo)/(c->rs.cantSucEvo);
        c->rs.medCostFailEvo= (c->rs.costAcumFailEvo)/(c->rs.cantFailEvo);
        c->ral.medCostSucEvo= (c->ral.costAcumSucEvo)/(c->ral.cantSucEvo);
        c->ral.medCostFailEvo= (c->ral.costAcumFailEvo)/(c->ral.cantFailEvo);
        c->rac.medCostSucEvo= (c->rac.costAcumSucEvo)/(c->rac.cantSucEvo);
        c->rac.medCostFailEvo= (c->rac.costAcumFailEvo)/(c->rac.cantFailEvo);

        return 1;
    }
}
