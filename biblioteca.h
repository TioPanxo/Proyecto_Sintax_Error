

#ifndef biblioteca_h
#define biblioteca_h

typedef struct Libro Libro;

typedef struct Autor Autor;

typedef struct Persona Persona;

void import (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores, HashMap * mapaDeAutores, int * flag);

void librosOrdenados (HashMap * mapaLibrosBiblioteca);

void mostrarTodosLosLibrosAutor(HashMap * mapaDeAutores);

void mostrarUnLibro(HashMap * mapaLibrosBiblioteca);

void solicitarLibro(HashMap * mapaLibrosBiblioteca,HashMap * mapaDeAutores,HashMap * mapaDeDeudores);

void devolverLibro(HashMap * mapaLibrosBiblioteca,HashMap * mapaDeAutores,HashMap * mapaDeDeudores);

void mostrarMorosos(HashMap * mapaDeDeudores);

void cerrarPrograma (HashMap * mapaLibrosBiblioteca, HashMap * mapaDeDeudores, int * flag);

#endif /* biblioteca_h */
