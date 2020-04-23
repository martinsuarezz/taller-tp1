#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    int socket;
}socket_t;

//
void socket_create(socket_t* self);

//
void socket_destroy(socket_t* self);

//Conecta el socket al host y servicio recibidos
int socket_connect(socket_t* self, const char* host, const char* service);

int socket_bind_and_listen(socket_t* self, const char* service);

//
int socket_accept(socket_t* self, socket_t accepted_socket);

//
int socket_send(socket_t* self, size_t bytes, const char* msg);

//
int socket_receive(socket_t* self, size_t bytes, char* buffer);