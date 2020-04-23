#include <stdio.h>
#include <string.h>
#include "fileHandler.h"

#define BUFFER_SIZE 40

int filehandler_create(filehandler_t* self, const char* filename, size_t readbytes){
    FILE* input_file;
    char buffer[BUFFER_SIZE];
    if (!filename){
        input_file = stdin;
    }
    else{
        strncpy(buffer, filename, BUFFER_SIZE-1);
        input_file = fopen(buffer, "r");
        if (!input_file)
            return 1;
    }
    self->file = input_file;
    self->readbytes = readbytes;
    return 0;
}


char* filehandler_read(filehandler_t* self, char* buffer){
    return fgets(buffer, self->readbytes, self->file);
}


void filehandler_destroy(filehandler_t* self){
    if(self->file != stdin)
        fclose(self->file);
}