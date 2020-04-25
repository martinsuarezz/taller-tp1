#include "dinamicvector.h"
#include <stdio.h>   
#include <stdlib.h>

void dinamicvector_create(dinamicvector_t* self, size_t size){
    self->length = 0;
    if (size > 0)
        self->capacity = size;
    else
        self->capacity = 32;
    
    self->array = (char*) malloc(sizeof(char) * self->capacity);
}

// Destruye el vector dinamico y libera la memoria utilizada
void dinamicvector_destroy(dinamicvector_t* self){
    free(self->array);
}

// Obtiene los datos del vector dinamico
char* dinamicvector_get_array(dinamicvector_t* self){
    return self->array;
}

size_t dinamicvector_get_length(dinamicvector_t* self){
    return self->length;
}

void dinamicvector_add(dinamicvector_t* self, const char* values, size_t size){
    while (size + self->length > self->capacity){
        self->capacity = self->capacity * 2;
        self->array = (char*) realloc((void*) self->array, sizeof(char) * self->capacity);
    }
    memmove(self->array + self->length, values, size);
    self->length = self->length + size;
}