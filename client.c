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
    char buffer[32];
    while(filehandler_read(&(self->filehandler), buffer)){
        socket_send(&(self->socket), 32, (const char*) buffer);
    }
    return 0;
}