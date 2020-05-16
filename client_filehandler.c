#include <stdio.h>
#include <string.h>
#include "client_filehandler.h"

void filehandler_create(filehandler_t* self){
    self->file = NULL;
}

int filehandler_add_file(filehandler_t* self, const char* file, size_t bytes){
    FILE* input_file;
    if (!file){
        input_file = stdin;
    } else{
        input_file = fopen(file, "r");
        if (!input_file)
            return -1;
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
    return fgets(self->readbuffer, self->readbytes, self->file);
}

void clean_bytes(char* buffer, int from, int to){
    memset(buffer + from, '\0', to - from);
}

char* filehandler_readline(filehandler_t* self, dinamicvector_t* vector){
    size_t index;
    size_t continue_reading = 1;
    while (continue_reading){
        if (!filehandler_read(self))
            return NULL;
        for (index = 0; index < self->readbytes - 1; index++){
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
    if (self->file != stdin && self->file != NULL)
        fclose(self->file);
}
