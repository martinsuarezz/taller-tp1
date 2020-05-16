#include <stdio.h>
#include <string.h>
#include "client.h"
#include "client_encoder.h"

void client_create(client_t* self){
    socket_t socket;
    filehandler_t filehandler;
    socket_create(&socket);
    filehandler_create(&filehandler);
    self->socket = socket;
    self->filehandler = filehandler;
    self->msg_number = 1;
}

void client_destroy(client_t* self){
    socket_destroy(&(self->socket));
    filehandler_destroy(&(self->filehandler));
}

int client_encode_and_send(client_t* self, size_t msg_n){
    encoder_t encoder;
    encoder_create(&encoder);
    char* message = encoder_encode_line(&encoder, &(self->filehandler), msg_n);
    if (!message){
        encoder_destroy(&encoder);
        return 0;
    }
    size_t message_len = encoder_message_length(&encoder);
    if (socket_send(&(self->socket), message_len, (const char*) message) == -1)
        return -1;
    free(message);
    encoder_destroy(&encoder);
    return 1;
}

int client_connect(client_t* self, const char*  host, const char* service){
    return socket_connect(&(self->socket), host, service);
}

void client_wait_response(client_t* self){
    char buffer[3];
    socket_receive(&(self->socket), 3, buffer);
    printf("%#010x: ", self->msg_number);
    fputs((const char*) buffer, stdout);
}

int client_run(client_t* self, const char* filename, int from_stdin){
    int err;
    if (from_stdin)
        err = filehandler_add_file(&(self->filehandler), NULL, 32);
    else
        err = filehandler_add_file(&(self->filehandler), filename, 32);

    if (err == 1)
        return -1;
    
    err = 0;
    while ((err=client_encode_and_send(self, (size_t) self->msg_number)) > 0){
        client_wait_response(self);
        self->msg_number++;
    }

    if (err == -1)
        return -1;
    
    return 0;
}
