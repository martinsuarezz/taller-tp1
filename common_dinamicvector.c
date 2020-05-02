#include "common_dinamicvector.h"
#include <stdio.h>   
#include <stdlib.h>

void dinamicvector_create(dinamicvector_t* self, size_t size){
    self->length = 0;
    if (size > 0)
        self->capacity = size;
    else
        self->capacity = 32;
    
    self->array = (char*) malloc(sizeof(char) * self->capacity);
    memset(self->array, '\0', size);
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
        size_t new_capacity  = self->capacity * 2;
        self->capacity = new_capacity;
        self->array = (char*)realloc(self->array, sizeof(char) * new_capacity);
        memset(self->array + self->length, '\0', self->capacity - self->length);
    }
    memmove(self->array + self->length, values, size);
    self->length = self->length + size;
}
