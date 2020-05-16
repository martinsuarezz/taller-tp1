#include "server.h"
#include "server_decoder.h"

void server_create(server_t* self){
    socket_create(&self->socket);
}

int server_listen(server_t* self, const char* service){
    return socket_bind_and_listen(&(self->socket), service);
}

int server_send_response(server_t* self, socket_t* socket){
    return socket_send(socket, 3, "OK\n");
}

int server_run(server_t* self){
    decoder_t decoder;
    socket_t accepted_socket;
    if (socket_accept(&(self->socket), &accepted_socket) == -1)
        return -1;
    decoder_create(&decoder);
    decoder_add_socket(&decoder, &accepted_socket);
    while (decoder_decode(&decoder)){
        decoder_print_message(&decoder);
        if (server_send_response(self, &accepted_socket) == -1)
            return -1;
        decoder_reset_message(&decoder);
    }
    decoder_destroy(&decoder);
    socket_destroy(&accepted_socket);
    return 0;
}

void server_destroy(server_t* self){
    socket_destroy(&(self->socket));
}
