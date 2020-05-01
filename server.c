#include "server.h"
#include "server_decoder.h"

void server_create(server_t* self){
    socket_t socket;
    socket_create(&socket);
    self->socket = socket;
}

void server_listen(server_t* self, const char* service){
    socket_bind_and_listen(&(self->socket), service);
}

void server_send_response(server_t* self, socket_t* socket){
    socket_send(socket, 3, "OK\n");
}

void server_run(server_t* self){
    decoder_t decoder;
    socket_t accepted_socket;
    socket_accept(&(self->socket), &accepted_socket);
    decoder_create(&decoder);
    decoder_add_socket(&decoder, &accepted_socket);
    while (decoder_decode(&decoder)){
        decoder_print_message(&decoder);
        server_send_response(self, &accepted_socket);
        decoder_reset_message(&decoder);
    }
    decoder_destroy(&decoder);
    socket_destroy(&accepted_socket);
}

void server_destroy(server_t* self){
    socket_destroy(&(self->socket));
}