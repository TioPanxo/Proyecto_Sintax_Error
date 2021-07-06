#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
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
    char libroSolicitado[120];
    char fecha[11]; // dd,mm,aaaa
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

    if(*flag == 1){
        printf(" Ya se han abierto los archivos\n");
        return;
    }

    //Lectura del nombre del archivo a importar*/
    archivoLibros = fopen("librosBiblioteca.csv","r");
    if(archivoLibros == NULL){
        printf(" El archivo no existe\n");
        return;
    }
    
    
    *flag = 1; 

    //printf ("LLegue aca\n");
    
    char * linea = (char*)malloc(1024*sizeof(char));
    Libro * nuevoLibro;
    Autor * nuevoAutor;
    Persona * nuevaPersona;
    Autor * auxiliar;

    linea = fgets(linea, 1024, archivoLibros);

    //Lectura archivo libros
    while(fgets (linea, 1024, archivoLibros) != NULL){ //Recorrido del archivo para lectura y almacenado
        nuevoLibro = (Libro *)malloc(sizeof(Libro));

        strcpy(nuevoLibro->ISBN,get_csv_field(linea,0));
        strcpy(nuevoLibro->nombreDelLibro,get_csv_field(linea,1));
        strcpy(nuevoLibro->nombreDelAutor,get_csv_field(linea,2));
        char auxNombreAutor[50];
        strcpy(auxNombreAutor, get_csv_field(linea,2));
        strcpy(nuevoLibro->codigoLibro, get_csv_field(linea,3));
        nuevoLibro->disponibilidad = atoi(get_csv_field(linea,4));
        insertMap(mapaLibrosBiblioteca, strdup(get_csv_field(linea,1)),nuevoLibro);   //Almacenado en HashMap de Libros
        auxiliar = searchMap(mapaDeAutores, nuevoLibro->nombreDelAutor);

        //printf ("isbn: %s - libro: %s - autor: %s - ubi: %s - dispo: %d\n", nuevoLibro->ISBN,nuevoLibro->nombreDelLibro, nuevoLibro->nombreDelAutor, nuevoLibro->codigoLibro, nuevoLibro->disponibilidad);
            
        //MAPA AUTORES
        if (auxiliar != NULL) { //Caso en que se encuentra un libro con el mismo nombre de autor
            pushBack(auxiliar->listaDeLibros, nuevoLibro); //No se si esta bien, pero al ya existir el nombre en el struct solo se llena la lista¿?
        } else {    //En caso de que no exista, se "inciaria" la lista
            nuevoAutor = (Autor *) malloc (sizeof(Autor));
            strcpy(nuevoAutor->nombreDelAutor,nuevoLibro->nombreDelAutor);
            nuevoAutor->listaDeLibros = createList();
            pushBack(nuevoAutor->listaDeLibros, nuevoLibro);
            insertMap(mapaDeAutores,strdup(auxNombreAutor), nuevoAutor);    //AL NO EXISTIR SE AGREGA AL MAPA d autores
        }

    }

    printf ("Archivo librosBiblioteca.csv leido correctamente\n");   
    fclose(archivoLibros);

    //LECTURA ARCHIVO PRESTAMOS
    archivoMorosos = fopen("deudores.csv","r");
    if(archivoMorosos == NULL){
        printf(" El archivo '%s' no existe\n", nombreArchivoMorosos);
        return;
    }
    Libro * auxLibroPrestado;
    linea = fgets(linea, 1024, archivoMorosos);
    while(fgets (linea, 1023, archivoMorosos) != NULL){

        nuevaPersona = (Persona *)malloc(sizeof(Persona));

        strcpy(nuevaPersona->nombreDePersona,get_csv_field(linea,0));
        char nombrePersonaAux[50];
        strcpy(nombrePersonaAux,get_csv_field(linea,0));
        strcpy(nuevaPersona->numeroDeTelefono,get_csv_field(linea,1));
        strcpy(nuevaPersona->libroSolicitado,get_csv_field(linea,2));
        strcpy(nuevaPersona->fecha, get_csv_field(linea,3));

        List * listaDePersona = searchMap(mapaDeDeudores, nombrePersonaAux);

        if (listaDePersona != NULL) { //Caso en que se encuentra persona con deuda
            pushBack(listaDePersona, nuevaPersona); 
        }
        else{    //En caso de que no exista, se "inciaria" la lista
            listaDePersona = createList();
            pushBack(listaDePersona,nuevaPersona);
            insertMap(mapaDeDeudores, strdup(nuevaPersona->nombreDePersona),listaDePersona);    //AL NO EXISTIR SE AGREGA AL MAPA d deudores
        }
        printf("%s\n",nuevaPersona->nombreDePersona);
    }
    printf ("Archivo Deudores.csv leido correctamente\n");
    fclose(archivoMorosos);
       

}

char * aMinusculas(char * nombreAutor){
    char * minusculas = (char *)malloc(sizeof(char) * (strlen(nombreAutor)));
    for(int i = 0;i < strlen(nombreAutor);i++){
        minusculas[i] = tolower(nombreAutor[i]);
    }
    return minusculas;
}

void bubbleSortLibrosPorAutor(Libro ** Array,long largo){
    Libro * aux;

    for(int i = 0;i < largo - 1;i++){
        for(int j = 0;j < largo - i - 1;j++){
            if(strcmp(aMinusculas(Array[j]->nombreDelAutor),aMinusculas(Array[j+1]->nombreDelAutor)) > 0){
                aux = Array[j+1];
                Array[j+1] = Array[j];
                Array[j] = aux;
            }
        }
    }
}

void librosOrdenados (HashMap * mapaLibrosBiblioteca)
{
    Libro * auxLibro = firstMap(mapaLibrosBiblioteca);

    long cantidadLibros = getSizeMap(mapaLibrosBiblioteca);

    Libro ** listaLibros = (Libro **)malloc(sizeof(Libro *) * cantidadLibros);
    for(long i = 0; i< cantidadLibros;i++){
        listaLibros[i] = auxLibro;
        auxLibro = nextMap(mapaLibrosBiblioteca);
    }

    bubbleSortLibrosPorAutor(listaLibros,cantidadLibros);

    printf(" LIBROS ORDENADOS SEGUN NOMBRE(S) DE AUTOR(ES)\n");
    printf("===============================================================\n");

    for(long i = 0;i<cantidadLibros;i++){
        printf(" Nombre(s) Autor(es): %s\n Nombre Libro: %s - ISBN: %s\n",listaLibros[i]->nombreDelAutor,listaLibros[i]->nombreDelLibro,listaLibros[i]->ISBN);
        printf(" Codigo Biblioteca: %s - Disponibilidad: %d\n",listaLibros[i]->codigoLibro,listaLibros[i]->disponibilidad);
        printf("===============================================================\n");
    }
    printf (" Total de libros existentes: %ld\n", cantidadLibros);
}

void ordenarLibrosPorDisponibilidad(Libro ** Array,int largo){
    Libro * aux;

    for(int i = 0;i < largo - 1;i++){
        for(int j = 0;j < largo - i - 1;j++){
            if(Array[j]->disponibilidad > Array[j+1]->disponibilidad){
                aux = Array[j+1];
                Array[j+1] = Array[j];
                Array[j] = aux;
            }
        }
    }
}

char * aMayusculas(char * nombreAutor){
    char * mayusculas = (char *)malloc(sizeof(char) * (strlen(nombreAutor)));
    for(int i = 0;i < strlen(nombreAutor);i++){
        mayusculas[i] = toupper(nombreAutor[i]);
    }
    return mayusculas;
}


//Apellido Nombre
void mostrarTodosLosLibrosAutor(HashMap * mapaDeAutores){
    char nombreAux[50];
    printf(" Ingrese el nombre del Autor *apellido nombre*\n ");
    gets(nombreAux);
    Autor * autorAux = searchMap(mapaDeAutores,nombreAux);
    if(autorAux == NULL){
        printf(" NO SE ENCUENTRA EL AUTOR DE NOMBRE %s",nombreAux);
        return;
    }

    int largo = get_size(autorAux->listaDeLibros);
    Libro * libroActual = first(autorAux->listaDeLibros);
    Libro ** listaLibros = (Libro **)malloc(sizeof(Libro *) * largo);

    for(int i = 0;i<largo;i++){
        listaLibros[i] = libroActual;
        libroActual = next(autorAux->listaDeLibros);
    }

    ordenarLibrosPorDisponibilidad(listaLibros,largo);

    
    printf(" LIBROS DE %s ORDENADOS SEGUN DISPONIBILIDAD)\n",aMayusculas(autorAux->nombreDelAutor));
    
    for(long i = 0;i<largo;i++){
        printf("===============================================================\n");
        printf(" Nombre(s) Autor(es): %s\n Nombre Libro: %s - ISBN: %s\n",listaLibros[i]->nombreDelAutor,listaLibros[i]->nombreDelLibro,listaLibros[i]->ISBN);
        printf(" Codigo Biblioteca: %s - Disponibilidad: %d\n",listaLibros[i]->codigoLibro,listaLibros[i]->disponibilidad);
    }
}

void mostrarUnLibro(HashMap * mapaLibrosBiblioteca){
    char nombreAux[120];
    printf(" Ingrese el nombre del Libro que desea buscar\n ");
    gets(nombreAux);
    Libro * libroAux = searchMap(mapaLibrosBiblioteca,nombreAux);
    if(libroAux == NULL){
        printf(" NO SE ENCUENTRA EL LIBRO DE NOMBRE %s\n",nombreAux);
        return;
    }
    else{
        printf("===============================================================\n");
        printf(" Nombre(s) Autor(es): %s\n Nombre Libro: %s - ISBN: %s\n",libroAux->nombreDelAutor,libroAux->nombreDelLibro,libroAux->ISBN);
        printf(" Codigo Biblioteca: %s - Disponibilidad: %d\n",libroAux->codigoLibro,libroAux->disponibilidad);
    }
}

Libro * buscarLibroLista(Autor * autorAux,char * nombreLibro){
    Libro * libroAux = first(autorAux->listaDeLibros);
    while(libroAux != NULL){
        if(strcmp(libroAux->nombreDelLibro,nombreLibro) == 0){
            return libroAux;
        }
        libroAux = next(autorAux->listaDeLibros);
    }
    return(NULL);
}

char * obtenerFechaActual(){
    time_t ahora;
    struct tm * fecha;
    time(&ahora);
    fecha = localtime(&ahora);

    char * fechaAux  = (char * )malloc(sizeof(char) * 11);
    char diaAux[3];
    char mesAux[3];
    char anioAux[5];

    sprintf(diaAux,"%d",fecha->tm_mday);
    sprintf(mesAux,"%d",fecha->tm_mon+1);
    sprintf(anioAux,"%d",fecha->tm_year+1900);

    if(fecha->tm_mday < 10){
        strcpy(fechaAux,"0");
    }
    strcat(fechaAux,diaAux);
    strcat(fechaAux,",");

    if(fecha->tm_mon + 1 <10){
        strcat(fechaAux,"0");
    }
    strcat(fechaAux,mesAux);
    strcat(fechaAux,",");

    strcat(fechaAux,anioAux);

    return fechaAux;
}

void solicitarLibro(HashMap * mapaLibrosBiblioteca,HashMap * mapaDeAutores,HashMap * mapaDeDeudores){
    char fechaPrestamo[11];
    strcpy(fechaPrestamo,obtenerFechaActual());
    char nombreAux[120];
    printf(" Ingrese el nombre del Libro que desea Solicitar\n ");
    gets(nombreAux);
    Libro * libroAux = searchMap(mapaLibrosBiblioteca,nombreAux);
    if(libroAux == NULL){
        printf(" NO SE ENCUENTRA EL LIBRO DE NOMBRE %s\n",nombreAux);
        return;
    }
    if(libroAux->disponibilidad == 0){
        printf(" NO HAY DISPONIBILIDAD PARA ESTE LIBRO\n");
        return;
    }
    libroAux->disponibilidad -= 1;
    Autor * autorAux = searchMap(mapaDeAutores,libroAux->nombreDelAutor);
    libroAux = buscarLibroLista(autorAux,libroAux->nombreDelLibro);
    Persona * nuevaPersona = (Persona * )malloc(sizeof(Persona));
    printf(" Ingrese su Nombre\n ");
    gets(nuevaPersona->nombreDePersona);
    List * listaDePersona = searchMap(mapaDeDeudores,nuevaPersona->nombreDePersona);
    if(listaDePersona == NULL){
        listaDePersona = createList();
        printf(" Ingrese su numero telefonico (ej: 912345678)\n ");
        gets(nuevaPersona->numeroDeTelefono);
        strcpy(nuevaPersona->libroSolicitado,libroAux->nombreDelLibro);
        strcpy(nuevaPersona->fecha,fechaPrestamo);
        pushBack(listaDePersona,nuevaPersona);
        insertMap(mapaDeDeudores,strdup(nuevaPersona->nombreDePersona),listaDePersona);
    }
    else{
        if(get_size(listaDePersona) == 3){
            printf(" NO PUEDES SOLICITAR MAS LIBROS\n");
            return;
        }
        else{
            printf(" Ingrese su numero telefonico (ej: 912345678)\n ");
            gets(nuevaPersona->numeroDeTelefono);
            strcpy(nuevaPersona->libroSolicitado,libroAux->nombreDelLibro);
            strcpy(nuevaPersona->fecha,fechaPrestamo);
            pushBack(listaDePersona,nuevaPersona);
        }
    }

    printf(" LIBRO SOLICITADO CON EXITO\n");
    printf("===============================================================\n");
    printf(" Nombre del solicitante: %s - Numero de telefono: %s\n",nuevaPersona->nombreDePersona,nuevaPersona->numeroDeTelefono);
    printf(" Nombre del libro: %s - Fecha de prestamo: %s\n",nuevaPersona->libroSolicitado,nuevaPersona->fecha);
    printf(" Recuerda que tienes 7 dias para devolver el libro\n");
}

Persona * buscarPersonaLista(List * listaDePersona,char * nombreLibroAux){
    Persona * auxPersona = first(listaDePersona);
    while(auxPersona != NULL){
        if(strcmp(auxPersona->libroSolicitado,nombreLibroAux) == 0){
            return auxPersona;
        }
    }
    return NULL;
}

void devolverLibro(HashMap * mapaLibrosBiblioteca,HashMap * mapaDeAutores,HashMap * mapaDeDeudores){
    char nombreAux[50];
    printf(" Ingrese su Nombre\n ");
    gets(nombreAux);
    List * listaDePersona = searchMap(mapaDeDeudores,nombreAux); 
    if(listaDePersona == NULL){
        printf(" NO SE ENCUENTRA A LA PERSONA DE NOMBRE %s\n",nombreAux);
        return;
    }
    printf(" La persona de nombre %s debe el o los siguientes libros\n",nombreAux);
    Persona * auxPersona = first(listaDePersona);
    while(auxPersona != NULL){
        printf("===============================================================\n");
        printf(" Nombre del solicitante: %s - Numero de telefono: %s\n",auxPersona->nombreDePersona,auxPersona->numeroDeTelefono);
        printf(" Nombre del libro: %s - Fecha de prestamo: %s\n",auxPersona->libroSolicitado,auxPersona->fecha);
        auxPersona = next(listaDePersona);
    }
    char nombreLibroAux[120];
    printf("===============================================================\n");
    printf(" Ingrese el nombre del libro que desea devolver\n ");
    gets(nombreLibroAux);
    Persona * personaAux = buscarPersonaLista(listaDePersona,nombreLibroAux);
    while(personaAux == NULL){
        printf(" No se encuentra el libro de nombre %s\n",nombreLibroAux);
        printf(" Ingrese el nombre del libro que desea devolver\n ");
        gets(nombreLibroAux);
        Persona * personaAux = buscarPersonaLista(listaDePersona,nombreLibroAux);
    }
    Libro * auxLibro = searchMap(mapaLibrosBiblioteca,personaAux->libroSolicitado);
    auxLibro->disponibilidad += 1;
    popCurrent(listaDePersona);
    printf(" El libro %s se ha devuelto con exito\n",auxLibro->nombreDelLibro);
}

void mostrarMorosos(HashMap * mapaDeDeudores){
    printf(" MOSTRANDO UNA LISTA DE LAS PERSONAS QUE DEBEN LIBROS\n");

    //Tiempo actual en tipo time_t
    time_t ahora,horaAux;
    time(&ahora);

    struct tm fechaAux;
    struct tm * fechaActualAux;

    fechaActualAux = localtime(&ahora);
    int contador = 0;

    List * listaDePersona = firstMap(mapaDeDeudores);
    while(listaDePersona != NULL){
        Persona * personaAux = first(listaDePersona);
        while(personaAux != NULL){
            fechaAux.tm_year = fechaActualAux->tm_year;
            fechaAux.tm_mon = atoi(get_csv_field(personaAux->fecha,1));
            fechaAux.tm_mon -= 1;
            fechaAux.tm_mday = atoi(get_csv_field(personaAux->fecha,0));
            fechaAux.tm_hour = 0;
            fechaAux.tm_min = 0;
            fechaAux.tm_sec = 1;
            fechaAux.tm_isdst = -1;

            horaAux = mktime(&fechaAux);

            double diferenciaTiempo = difftime(ahora,horaAux);
            if((diferenciaTiempo / (60*60*24)) >= 8.0){
                printf("===============================================================\n");
                printf(" La persona de nombre: %s - Numero Telefonico: %s\n",personaAux->nombreDePersona,personaAux->numeroDeTelefono);
                printf(" Debe el libro: %s - Desde hace %.0lf Dias\n",personaAux->libroSolicitado,(diferenciaTiempo / (60*60*24)) - 8);
                contador++;
            }
            personaAux = next(listaDePersona);
        }
        listaDePersona = nextMap(mapaDeDeudores);
    }
}

/*void cerrarPrograma (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores){
    FILE * archivoLibros;
    FILE * archivoMorosos;

    if (*flag == 0) {
        printf ("Recuerde importar archivos antes de exportarlos");
        return;
    }
    //Actualizacion csv
    archivoLibros = fopen("librosBiblioteca.csv","r+"); //Es mejor borrar todo y agregar o que se sobreescriba y usar fseek
    archivoMorosos = fopen("deudores.csv", "r+");

    fseek(archivoLibros,-1,SEEK_SET);
    fseek(archivoMorosos,-1,SEEK_SET);

    Libro * nuevoLibro;
    Persona * nuevaPersona;


    nuevoLibro = (Libro *)malloc(sizeof(Libro));
    nuevoLibro = firstMap(mapaLibrosBiblioteca);

    archivoLibros = fopen("librosBiblioteca.csv","r+"); //Es mejor borrar todo y agregar o que se sobreescriba y usar fseek
    archivoMorosos = fopen("deudores.csv", "r+");

    fseek(archivoLibros,-1,SEEK_SET);
    fseek(archivoMorosos,-1,SEEK_SET);

    fprintf (archivoLibros,"ISBN,Titulo,Autor,Ubicacion,Disponibilidad\n");

    while (nuevoLibro != NULL) {    //Recorrido de libros para escribir en csv
        fprintf(archivoLibros, "%s,", nuevoLibro->ISBN);
        fprintf(archivoLibros, "%s,", nuevoLibro->nombreDelLibro);
        fprintf(archivoLibros, "%s,", nuevoLibro->nombreDelAutor);
        fprintf(archivoLibros, "%s,", nuevoLibro->codigoLibro);
        fprintf(archivoLibros, "%d\n", nuevoLibro->disponibilidad);
        printf ("Libro almacenado: %s \n", nuevoLibro->nombreDelLibro);
        nuevoLibro = nextMap(mapaLibrosBiblioteca);
    }

    printf ("Libros exportados correctamente a librosBiblioteca.csv");
    fclose(archivoLibros);

    nuevaPersona = (Persona *)malloc(sizeof(Persona));
    nuevaPersona = firstMap(mapaDeDeudores);
    fprintf (archivoMorosos, "Nombre de persona,Numero telefonico,Nombre del libro,Fecha de prestamo\n");
    while (nuevaPersona != NULL)
    {
        fprintf (archivoMorosos, "%s,", nuevaPersona->nombreDePersona);
        fprintf (archivoMorosos, "%s,", nuevaPersona->numeroDeTelefono);
        fprintf (archivoMorosos, "%s,", nuevaPersona->libroSolicitado);
        fprintf (archivoMorosos, "%s\n", nuevaPersona->fecha);
        printf ("Moroso almacenado: %s \n", nuevaPersona->nombreDePersona);
        nuevaPersona = nextMap(mapaDeDeudores);
    }
    
    printf ("Morosos exportados correctamente a deudores.csv");
    fclose(archivoMorosos);
}*/
