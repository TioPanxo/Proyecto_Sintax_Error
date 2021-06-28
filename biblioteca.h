

#ifndef funciones_h
#define funciones_h

typedef struct Libro Libro;
typedef struct Autor Autor;
typedef struct Persona Persona;

void import (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores, HashMap * mapaDeAutores, int * flag);
void librosOrdenados (HashMap * mapaLibrosBiblioteca);
void mapaLibro_Lista (HashMap * map, List * list);

#endif /* funciones_h */
