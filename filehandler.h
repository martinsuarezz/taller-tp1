#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "dinamicvector.h"

typedef struct{
    FILE* file;
    size_t readbytes;
    char readbuffer[32];
    size_t index;
} filehandler_t;

//Inicializa el handler de archivos, recibe un nombre de archivo a abrir, si es NULL lee de stdin.
//Recibe también la cantidad de bytes a leer en cada llamada a filehandler_read
//PRE: se recibe un puntero a un filehandler_t
//POST: si encuentra un error devuelve 1, sino devuelve 0
int filehandler_create(filehandler_t* self, const char* filename, size_t readbytes);

//Recibe un handler de archivos y copia al buffer la cantidad designada de bytes 
//leyendolos del archivo de lectura.
//PRE: self recibe un puntero a un filehandler_t ya inicializado.
//POST: el lector del archivo apunta al siguiente caracter del último copiado.
char* filehandler_read(filehandler_t* self, char* buffer);

//Copia al buffer una línea del archivo de entrada.
//Si la linea es mayor a la cantidad de bytes permitidas se copia una parte
//y devuelve 1. Si se pudo llegar hasta un salto de línea devuelve 0.
//PRE: recibe un puntero a un filehandler_t ya inicializado.
//POST: el lector del archivo apunta al siguiente caracter del último copiado.
void filehandler_readline(filehandler_t* self, dinamicvector_t* vector);

//Cierra el archivo utilizado.
//PRE: se recibe un puntero a un filehandler_t ya inicializado.
void filehandler_destroy(filehandler_t* self);

//Devuelve el tamaño en bytes de lo que ocuparán los argumentos en
//el body del protocolo D-BUS.
//PRE: se recibe un puntero a un filehandler_t ya inicializado
size_t filehandler_length_of_arguments(filehandler_t* self);

//Devuelve la cantidad de argumentos que posee el método.
//PRE: se recibe un puntero a un filehandler_t ya inicializado
size_t filehandler_ammount_of_arguments(filehandler_t* self);

#endif