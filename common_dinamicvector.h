#ifndef COMMON_DINAMICVECTOR_H
#define COMMON_DINAMICVECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char* array;
    size_t length;
    size_t capacity;
}dinamicvector_t;

// Crea un vector dinamico
void dinamicvector_create(dinamicvector_t* self, size_t size);

// Destruye el vector dinamico y libera la memoria utilizada
void dinamicvector_destroy(dinamicvector_t* self);

// Obtiene los datos del vector dinamico
char* dinamicvector_get_array(dinamicvector_t* self);

size_t dinamicvector_get_length(dinamicvector_t* self);

void dinamicvector_add(dinamicvector_t* self, const char* values, size_t size);

#endif