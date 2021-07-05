

#ifndef biblioteca_h
#define biblioteca_h

typedef struct Libro Libro;
typedef struct Autor Autor;
typedef struct Persona Persona;

void import (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores, HashMap * mapaDeAutores, int * flag);
void librosOrdenados (HashMap * mapaLibrosBiblioteca);
void mapaLibro_Lista (HashMap * map, List * list);
void mostrarTodosLosLibrosAutor(HashMap * mapaDeAutores);
void mostrarUnLibro(HashMap * mapaLibrosBiblioteca);

#endif /* biblioteca_h */
