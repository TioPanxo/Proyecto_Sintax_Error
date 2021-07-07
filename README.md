# BibliApp - Proyecto Sintax_Error


## Instalacion

Descargar los archivos, ingresar a la carpeta mediante la consola de Windos o Visual Studio Code, y compilar el programa. Debe tener un compilador de C, en nuestro caso, mingw y la extension de Microsoft para C en Visual Studio Code.

```
compilamos:
gcc hashmap.c list.c biblioteca.c main.c -o main
y ejecutamos:
./main
```

## Consideraciones

```
Es necesario utilizar los archivos con el nombre correcto. Para las pruebas, se utiliza 
librosBiblioteca.csv como el archivo de libros y 
deudores.csv como el archivo de personas y libros prestados. En caso de cambiar el 
nombre de los archivos, se debera modificar el codigo del programa.
Se debe tener en consideracion el formato de los archivos anteriormente mencionados,
 ya que intentar utilizar un archivo con un formato diferente al orden que tienen los 
 datos de los archivos tambien provocara problemas en la aplicacion.
```

## BibliApp
Aplicacion para bibliotecas en la cual se pueden controlar las existencias de libros y los prestamos de estos. Contiene las siguientes funcionalidades: 
1. Importar el archivo .csv de los libros existentes
2. Mostrar todos los libros contenidos en el archivo importado ordenados por su nombre de autor
3. Mostrar todos los libros existentes de un autor
4. Mostrar un libro especifico, sus caracteristicas y disponibilidad
5. Ingresar la solicitud de un libro
6. Ingresar la devolucion de un libro
7. Mostrar la lista de deudores de libros segun la fecha
8. Actualizar los archivos segun los cambios realizados durante el uso del programa. Esto ocurre al cerrar el programa


       