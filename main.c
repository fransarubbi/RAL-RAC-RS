#include <stdio.h>
#include "rs.h"
#include "ral.h"
#include "rac.h"


//Funciones menu principal
void mostrarEstructuras(rebSep );   //Mostrar las 3 estructuras
int lecturaOperaciones(StructCost *, rebSep *);  //Funcion para leer desde el archivo


//Funciones internas
void loadDeliveries(Deliveries *);   //Funcion auxiliar de carga de datos
void resetAll(rebSep *);   //Funcion para resetear todas las estructuras


int preload(rebSep *);


int main(){
    
    int menu, lect;
    rebSep rs;
    rebAbLin ral;
    Deliveries dev;
    StructCost c;

    initRebSep(&rs);
    initRebAbLin(&ral);
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
            case 1: resetAll(&rs);
                    initStructCost(&c);
                    lect = lecturaOperaciones(&c, &rs);
                    if(lect == 1){
                        mostrarCostos(c);
                    }
                    else{
                        printf("\n|-------------------------------------------------------------------|");
                        printf("\n| No hay costos cargados. No se ha realizado la lectura por archivo |");
                        printf("\n|-------------------------------------------------------------------|");
                    }
                    break;

            case 2: mostrarEstructuras(rs);
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


void resetAll(rebSep *rs){
    resetRebSep(rs);
}


void mostrarEstructuras(rebSep rs){
    
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
                            mostrarRs(rs);
                        }
                        break;

                case 2: break;

                case 3: break;
            }

        }
    }while(enter != 0);
}


int preload(rebSep *rs){

    Deliveries dev;
    char code[CODE], name[NAME], nameSender[NAME], addres[NAME], dateS[DATE], dateR[DATE];
    long dni, dniS;
    int value, enter, i;

    FILE *preload;
    preload = fopen("Envios.txt", "r");

    if(preload == NULL){
        printf("|----------------------------------------------|\n");
        printf("|       No se pudo acceder al archivo          |\n");
        printf("|----------------------------------------------|\n\n");
        return 0;
    }
    else{
        while(!feof(preload)){

            fscanf(preload, " %[^\n]\n", code);
            for(i = 0; code[i] != '\0'; i++){
                code[i] = toupper(code[i]);
            }
            strcpy(&dev.code, code);
            
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

            value = altaRebSep(rs, dev);

            switch(value){
                case 0: printf("|----------------------------------------------|\n");
                        printf("| Error al cargar elemento. No hay mas espacio |\n");
                        printf("|----------------------------------------------|\n\n");
                        break;
            
                case 1: printf("|-------------------------------------------------|\n");
                        printf("| Error al cargar elemento. El elemento ya existe |\n");
                        printf("|-------------------------------------------------|\n\n");
                        break;

                case 2: printf("|----------------------------------------------|\n");
                        printf("|            Carga exitosa de datos            |\n");
                        printf("|----------------------------------------------|\n\n");
                        break;
            }
        }
        printf("           Elementos cargados: %d\n", rs->cant);
        fclose(preload);
        return 1;
    }
}


void mostrarCostos(StructCost c){

    printf("\n|------------------------------------------------------|");
    printf("\n|          Mostrando Costos de las Estructuras         |");
    printf("\n|------------------------------------------------------|\n\n");
    printf("\n|------------------------------------------------------|");
    printf("\n|                   Rebalse Separado                   |");
    printf("\n|------------------------------------------------------|");
    printf("\n|    Funcion    |    Costo Max     |     Costo Medio   |");
    printf("\n|------------------------------------------------------|");
    printf("\n| Evoc. Exitosa |   %.2f                %.2f              ", c.rs.maxCostSucEvo, c.rs.medCostSucEvo);
    printf("\n|------------------------------------------------------");
    printf("\n| Evoc. Fracaso |   %.2f                %.2f              ", c.rs.maxCostFailEvo, c.rs.medCostFailEvo);
    printf("\n|------------------------------------------------------|\n");
}


int lecturaOperaciones(StructCost *c, rebSep *rs){

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
                    case 1: value = altaRebSep(rs, dev);
                            if(value == 2){

                            }
                            break;


                    case 2: if(rs->cant != 0){
                                value = bajaRebSep(rs, dev);
                            }
                            break;
                }
            }
            else{
                if(codeOperator == 3){
                    if(rs->cant != 0){
                        costo = 0.0;
                        value = evocacionRebSep(*rs, &dev, &costo);
                        if(value == 1){
                            if(c->rs.maxCostSucEvo < costo){
                                c->rs.maxCostSucEvo = costo;
                            }
                            c->rs.cantSucEvo += 1;
                            c->rs.costAcumSucEvo += costo;
                            c->rs.medCostSucEvo= (c->rs.costAcumSucEvo)/(c->rs.cantSucEvo);
                        }
                        else{
                            if(c->rs.maxCostFailEvo < costo){
                                c->rs.maxCostFailEvo = costo;
                            }
                            c->rs.cantFailEvo += 1;
                            c->rs.costAcumFailEvo += costo;
                            c->rs.medCostFailEvo= (c->rs.costAcumFailEvo)/(c->rs.cantFailEvo);
                        }
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
        return 1;
    }
}
