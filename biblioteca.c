#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "hashmap.h"

typedef struct Libro{
    char nombreDelLibro[50];
    char nombreDelAutor[50];
    char ISBN[20];
    char codigoLibro[10];
    int disponibilidad;
}Libro;

typedef struct Autor{
    char nombreDelAutor[50];
    List * listaDeLibros;
}Autor;

typedef struct Persona{
    char nombreDePersona[50];
    char numeroDeTelefono[10];
    Libro * libroSolicitado;
    char fecha[11];
}Persona;

