#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "hashmap.h"
#include "biblioteca.h"


//Funcion para leer el contenido de un archivo csv
const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

void import (int * flag)
{
    FILE * archivoLibros;
    FILE * archivoMorosos;
    char nombreArchivoLibros[50];
    char nombreArchivoMorosos[50];
    printf(" Que operacion desea realizar:\n");
    printf(" 1. Importar archivo de libros y morosos \n");
    //printf(" 2. Importar archivo de morosos \n");
    //printf(" 3. Exportar archivo de libros\n");
    printf(" 2. Exportar archivo de libros y morosos\n");
    printf(" 0. Volver al menu\n");
    printf ("===============================================================\n");
    int op; //Variable de operacion del menu
    printf(" Ingrese un Numero: ");
    scanf(" %d",&op);
    while((op < 0 || op > 2)){
        printf ("===============================================================\n");
        printf (" Por favor ingrese uno de los numeros anteriores: ");
        while(getchar()!='\n');
        scanf (" %d", &op);
    }
    if(op == 1){    //Validacion de archivos importados. No se puede tener mas de 1 archivo importado por funcionalidad
        if(*flag == 1){
            printf("Ya se ha abierto un archivo, exporte los datos a otro archivo\n");
            return;
        }
        //Apertura archivo libros
        printf(" Ingrese el nombre del archivo de libros: ");
        getchar();
        gets(nombreArchivoLibros);    //Lectura del nombre del archivo a importar
        archivoLibros = fopen(nombreArchivoLibros,"r");
        if(archivoLibros == NULL){
            printf(" El archivo no existe\n");
            return;
        }
        //Apertura archivo morosos
        printf(" Ingrese el nombre del archivo de morosos: ");
        getchar();
        gets(nombreArchivoMorosos);    //Lectura del nombre del archivo a importar
        archivoMorosos = fopen(nombreArchivoMorosos,"r");
        if(archivoMorosos == NULL){
            printf(" El archivo no existe\n");
            return;
        }
        *flag = 1;

        char * linea = (char*)malloc(1024*sizeof(char));
        Libro * nuevoLibro;
        Autor * nuevoAutor;
        Persona * nuevaPersona;

        linea = fgets(linea, 1024, archivoLibros);
    //Lectura archivo libros
        while(fgets (linea, 1023, archivoLibros) != NULL){ //Recorrido del archivo para lectura y almacenado
            nuevoLibro = (Libro *)malloc(sizeof(Libro));
            nuevoLibro->ISBN = atoi(get_csv_field(linea,0));
            strcpy(nuevoLibro->nombreDelLibro,get_csv_field(linea,1));
         





}