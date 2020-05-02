#include <stdio.h>
#include <string.h>
#include "client_filehandler.h"

#define FILENAME_BUFFER_SIZE 40

int filehandler_create(filehandler_t* self, const char* filename, size_t bytes){
    FILE* input_file;
    char buffer[FILENAME_BUFFER_SIZE];
    if (!filename){
        input_file = stdin;
    } else{
        strncpy(buffer, filename, FILENAME_BUFFER_SIZE - 1);
        input_file = fopen(buffer, "r");
        if (!input_file)
            return 1;
    }
    self->file = input_file;
    self->readbytes = bytes;
    self->index = bytes;
    return 0;
}

size_t filehandler_is_end_of_file(filehandler_t* self){
    return feof(self->file);
}


char* filehandler_read(filehandler_t* self){
    return fgets(self->readbuffer, self->readbytes + 1, self->file);
}

void clean_bytes(char* buffer, int from, int to){
    for (int i = from; i < to; i++)
        buffer[i] = '\0';
}

char* filehandler_readline(filehandler_t* self, dinamicvector_t* vector){
    size_t index;
    size_t continue_reading = 1;
    while (continue_reading){
        if (!filehandler_read(self))
            return NULL;
        for (index = 0; index < self->readbytes; index++){
            if (self->readbuffer[index] == '\n'){
                continue_reading = 0;
                break;
            }
        }
        dinamicvector_add(vector, self->readbuffer, index);
    }  
    return dinamicvector_get_array(vector);
}

void filehandler_destroy(filehandler_t* self){
    if (self->file != stdin)
        fclose(self->file);
}
