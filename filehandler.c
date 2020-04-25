#include <stdio.h>
#include <string.h>
#include "filehandler.h"

#define FILENAME_BUFFER_SIZE 40

int filehandler_create(filehandler_t* self, const char* filename, size_t readbytes){
    FILE* input_file;
    char buffer[FILENAME_BUFFER_SIZE];
    if (!filename){
        input_file = stdin;
    }
    else{
        strncpy(buffer, filename, FILENAME_BUFFER_SIZE - 1);
        input_file = fopen(buffer, "r");
        if (!input_file)
            return 1;
    }
    self->file = input_file;
    self->readbytes = readbytes;
    self->index = readbytes;
    return 0;
}


char* filehandler_read(filehandler_t* self, char* buffer){
    return fgets(buffer, self->readbytes, self->file);
}

void clean_bytes(char* buffer, int from, int to){
    for (int i = from; i < to; i++)
        buffer[i] = '\0';
}

void filehandler_readline(filehandler_t* self, dinamicvector_t* vector){
    while (1){
        for (size_t i = self->index; i < self->readbytes; i++){
            if (self->readbuffer[i] == '\n'){
                dinamicvector_add(vector, self->readbuffer + self->index, i - self->index);
                self->index = i;
                return;
            }
        }
        dinamicvector_add(vector, self->readbuffer + self->index, self->readbytes - self->index);
        filehandler_read(self, self->readbuffer);
        self->index = 0;
    }
    return;
}



size_t filehandler_ammount_of_arguments(filehandler_t* self){
    int character;
    size_t ammount = 1;
    /*
    if (filehandler_length_of_arguments(self) == 0)
        return 0;*/

    while (fgetc(self->file) != '(')
        continue;
     
    while ((character = fgetc(self->file)) != ')'){
        if (character == ',')
            ammount++;
    }
    return ammount;
}

void filehandler_destroy(filehandler_t* self){
    if(self->file != stdin)
        fclose(self->file);
}