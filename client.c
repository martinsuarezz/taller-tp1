#include <stdio.h>
#include <string.h>
#include "client.h"

void client_create(client_t* self){
    socket_t socket;
    socket_create(&socket);
    self->socket = socket;
}

void client_destroy(client_t* self){

}

void int32_to_array(char* array, uint32_t number){
    char* number_array = (char*) &number;
    for (int i = 0; i < 4; i++)
        array[i] = number_array[i];
}

size_t length_of_body(char* array){
    size_t index1, index2, length;
    for (index1 = 0; array[index1] != '('; index1++)
        continue;
    for (index2 = index1; array[index2] != ')'; index2++)
        continue;

    length = index2 - index1;
    if (length == 1)
        return 0;
    return length + 4;
}

int client_encode_and_send(client_t* self, int msg_number){
    dinamicvector_t vector;
    dinamicvector_create(&vector);
    filehandler_readline(&(self->filehandler), &vector);
    char header[12] = {0x6c, 0x01, 0x00, 0x01};
    int32_to_array(header+4, (uint32_t) length_of_body((dinamicvector_get_array(&vector))));
    int32_to_array(header+8, (uint32_t) msg_number);
    socket_send(&(self->socket), 12, (const char*) header);
    return 0;
}

int client_connect(client_t* self, const char*  host, const char* service){
    return socket_connect(&(self->socket), host, service);
}

int client_run(client_t* self, const char* filename, int from_stdin){
    filehandler_t filehandler;
    if (from_stdin)
        filehandler_create(&filehandler, NULL, 32);
    else
        filehandler_create(&filehandler, filename, 32);
    
    self->filehandler = filehandler;
    /*char buffer[32];
    while(filehandler_read(&(self->filehandler), buffer)){
        socket_send(&(self->socket), 32, (const char*) buffer);
    }*/
    client_encode_and_send(self, 1);
    return 0;
}