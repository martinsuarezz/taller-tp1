#include <stdio.h>
#include <string.h>
#include "client.h"
#include "encoder.h"

void client_create(client_t* self){
    socket_t socket;
    socket_create(&socket);
    self->socket = socket;
}

void client_destroy(client_t* self){

}

size_t prueba2(dinamicvector_t* vector){
    return dinamicvector_get_length(vector);
}

char* prueba(dinamicvector_t* vector){
    dinamicvector_create(vector, 32);
    const char values[100] = "\x00\x01\x00\x02\x67x00\x5f\x53\x02\x67";
    dinamicvector_add(vector, values, 100);
    return dinamicvector_get_array(vector);
}

int client_encode_and_send(client_t* self, size_t msg_number){
    encoder_t encoder;
    encoder_create(&encoder);
    
    char* message = encoder_encode_line(&encoder, &(self->filehandler), msg_number);
    //size_t message_len = encoder_message_length(&encoder);
    size_t message_len = encoder_message_length(&encoder);
    /*
    dinamicvector_t vector;
    char* message = prueba(&vector);
    size_t message_len = prueba2(&vector);
    */
    
    socket_send(&(self->socket), message_len, (const char*) message);
    free(message);
    encoder_destroy(&encoder);
    return 0;
}

int client_connect(client_t* self, const char*  host, const char* service){
    return socket_connect(&(self->socket), host, service);
}

int client_run(client_t* self, const char* filename, int from_stdin){
    filehandler_t filehandler;
    size_t msg_number = 1;
    if (from_stdin)
        filehandler_create(&filehandler, NULL, 32);
    else
        filehandler_create(&filehandler, filename, 32);
    
    self->filehandler = filehandler;
    while(client_encode_and_send(self, msg_number))
        msg_number++;
    filehandler_destroy(&filehandler);
    return 0;
}