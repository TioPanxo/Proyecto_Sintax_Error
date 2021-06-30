#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "hashmap.h"

typedef struct Libro{
    char nombreDelLibro[120];
    char nombreDelAutor[50];
    char ISBN[20];
    char codigoLibro[20];
    int disponibilidad;
}Libro;

typedef struct Autor{
    char nombreDelAutor[50];
    List * listaDeLibros;
}Autor;

typedef struct Persona{
    char nombreDePersona[50];
    char numeroDeTelefono[10];
    List * librosSolicitado;     //ES TIPO LIBRO O ES LISTA DE LIBROS
    //int cantidadLirbosSolicitados?
    char fecha[11];
}Persona;



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

void import (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores, HashMap * mapaDeAutores, int * flag)
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
        //getchar();
        gets(nombreArchivoMorosos);    //Lectura del nombre del archivo a importar
        archivoMorosos = fopen(nombreArchivoMorosos,"r");
        if(archivoMorosos == NULL){
            printf(" El archivo '%s' no existe\n", nombreArchivoMorosos);
            return;
        }
        *flag = 1; 
        //printf ("LLegue aca\n");
        char * linea = (char*)malloc(1024*sizeof(char));
        Libro * nuevoLibro;
        Autor * nuevoAutor;
        Persona * nuevaPersona;

        linea = fgets(linea, 1024, archivoLibros);
    //Lectura archivo libros
        printf ("primer while \n");
        while(fgets (linea, 1024, archivoLibros) != NULL){ //Recorrido del archivo para lectura y almacenado
            nuevoLibro = (Libro *)malloc(sizeof(Libro));

            strcpy(nuevoLibro->ISBN,get_csv_field(linea,0));
            strcpy(nuevoLibro->nombreDelLibro,get_csv_field(linea,1));
            char auxNombreLibro[50];
            strcpy(auxNombreLibro, get_csv_field(linea,1));
            strcpy(nuevoLibro->nombreDelAutor,get_csv_field(linea,2));
            char auxNombreAutor[50];
            strcpy(auxNombreAutor, get_csv_field(linea,2));
            strcpy(nuevoLibro->codigoLibro, get_csv_field(linea,3));
            nuevoLibro->disponibilidad = atoi(get_csv_field(linea,4));
            insertMap(mapaLibrosBiblioteca, strdup(auxNombreLibro),nuevoLibro);   //Almacenado en HashMap de Libros
            //*totalLibros+=1;
            Libro * auxiliar = searchMap(mapaLibrosBiblioteca, nuevoLibro->nombreDelAutor);

            //printf ("isbn: %s - libro: %s - autor: %s - ubi: %s - dispo: %d\n", nuevoLibro->ISBN,nuevoLibro->nombreDelLibro, nuevoLibro->nombreDelAutor, nuevoLibro->codigoLibro, nuevoLibro->disponibilidad);
            
            //MAPA AUTORES
            if (auxiliar != NULL) { //Caso en que se encuentra un libro con el mismo nombre de autor
                pushBack(nuevoAutor->listaDeLibros, auxiliar); //No se si esta bien, pero al ya existir el nombre en el struct solo se llena la lista¿?
            } else {    //En caso de que no exista, se "inciaria" la lista
                nuevoAutor = (Autor *) malloc (sizeof(Autor));
                strcpy(nuevoAutor->nombreDelAutor,nuevoLibro->nombreDelAutor);
                nuevoAutor->listaDeLibros = createList();
                pushFront(nuevoAutor->listaDeLibros, auxiliar);
                insertMap(mapaDeAutores,strdup(auxNombreAutor), nuevoAutor);    //AL NO EXISTIR SE AGREGA AL MAPA d autores
            }

        }
        printf ("Archivo %s leido correctamente\n", nombreArchivoLibros);   

        
        /**    * IMPRESION MAPA LIBROS *
         
        Libro * auxiliarPrint = firstMap(mapaLibrosBiblioteca);
        int cont = 0;
        while (auxiliarPrint != NULL){
            
            printf ("autor: %s - libro: %s - ubi: %s -  disponibilidad: %d - ISBN: %s \n",auxiliarPrint->nombreDelAutor, auxiliarPrint->nombreDelLibro,auxiliarPrint->codigoLibro, auxiliarPrint->disponibilidad, auxiliarPrint->ISBN);
            cont++;
            auxiliarPrint = nextMap(mapaLibrosBiblioteca);
        }
        printf ("CONTADOR = %d\n", cont);
        **/ 
       
        //LECTURA ARCHIVO PRESTAMOS
        linea = fgets(linea, 1024, archivoMorosos);
        while(fgets (linea, 1023, archivoMorosos) != NULL){

            nuevaPersona = (Persona *)malloc(sizeof(Persona));
            Libro * auxLibroPrestado = (Libro *)malloc(sizeof(Libro));

            strcpy(nuevaPersona->nombreDePersona,get_csv_field(linea,0));
            char auxNombrePersona[50];
            strcpy(auxNombrePersona,get_csv_field(linea,0));
            strcpy(nuevaPersona->numeroDeTelefono,get_csv_field(linea,1));
            char nombreLibroSolicitado[50];
            strcpy(nombreLibroSolicitado, get_csv_field(linea,2));
            //pushFront(listaDeLibrosSolicitados, )
            strcpy(nuevaPersona->fecha, get_csv_field(linea,3));

            auxLibroPrestado = searchMap(mapaLibrosBiblioteca, strdup(nombreLibroSolicitado));

            Persona * auxPersona = searchMap(mapaDeDeudores, nuevaPersona->nombreDePersona);

            if (auxPersona != NULL) { //Caso en que se encuentra persona con deuda
                pushFront(nuevaPersona->librosSolicitado, auxLibroPrestado); 
                // FALTA CONTADOR DE MAXIMO 3 LIBROS, AGREGAR A STRUCT
            } else {    //En caso de que no exista, se "inciaria" la lista
                nuevaPersona = (Persona *) malloc (sizeof(Persona));
                strcpy(nuevaPersona->nombreDePersona,auxNombrePersona);
                nuevaPersona->librosSolicitado = createList();
                //CONTADOR MAXIMO 3 LIBROS
                pushFront(nuevaPersona->librosSolicitado, auxLibroPrestado);   //no entiendo la implementacion de esto
                insertMap(mapaDeDeudores, strdup(auxNombrePersona), nuevaPersona);    //AL NO EXISTIR SE AGREGA AL MAPA d deudores
            }
        }
        printf ("Archivo %s leido correctamente\n", nombreArchivoMorosos);
        /** IMPRIMIR MAPA DEUDORES Y LISTA DE LIBROS DE SE DEBEN **
        Persona * auxPersona = (Persona *) malloc (sizeof(Persona));
        auxPersona = firstMap(mapaDeDeudores);
        while (auxPersona != NULL)
        {
            Libro * auxLibro = (Libro *)malloc(sizeof(Libro));
            auxLibro = first(auxPersona->librosSolicitado);
            printf("Deudor: %s - Libro(s) solicitado(s): %s",auxPersona->nombreDePersona,auxLibro);
            while (auxLibro != NULL) {
                auxLibro = next(auxPersona->librosSolicitado);
                if (auxLibro == NULL) break;
                printf ("- %s ", auxLibro);
            }
            auxPersona = nextMap(mapaDeDeudores);
            printf ("\n");
        }
        **/

    }

    if (op == 2 ) {
        printf ("BLABLA");
    }
    return;
}

void mapaLibro_Lista (HashMap * map, List * list) {

    //List * auxList = createList();
    Libro * auxLibro = firstMap(map);
    
    while (auxLibro != NULL)
    {
        pushFront(list, auxLibro);
        auxLibro = nextMap(map);
    }

}
void ordenarListaAutores (List * listaLibros)
{
    
}
void librosOrdenados (HashMap * mapaLibrosBiblioteca)
{
    Libro * auxLibro = firstMap(mapaLibrosBiblioteca);
    List * listaLibros = createList();

    mapaLibro_Lista(mapaLibrosBiblioteca, listaLibros);

    int cantidadLibros = get_size(listaLibros);
    auxLibro = first(listaLibros);

    while (auxLibro != NULL)
    {
        printf("Libro: %s - Autor: %s - ISBN: %s \n", auxLibro->nombreDelLibro, auxLibro->nombreDelAutor, auxLibro->ISBN);
        auxLibro = next(listaLibros);
    }
    printf ("Total de libros existentes: %d\n", cantidadLibros);
    
    
}