#ifndef SOCKET_H
#define SOCKET_H

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

// Se crea un socket
void socket_create(socket_t* self);

// Se destruye el socket, cerrandolo.
void socket_destroy(socket_t* self);

// Conecta el socket al host y servicio recibidos
int socket_connect(socket_t* self, const char* host, const char* service);

// El socket escucha conexiones al puerto recibido.
int socket_bind_and_listen(socket_t* self, const char* service);

// El socket acepta una conexión entrante.
int socket_accept(socket_t* self, socket_t* accepted_socket);

// El socket envía del mensaje la cantidad de bytes indicados.
// Devuelve la cantidad de bytes que envío satisfactoriamente.
int socket_send(socket_t* self, size_t bytes, const char* msg);

//Recibe la cantidad de bytes indicada por parametro y los almacena
//en el buffer. Devuelve la cantidad de bytes leidos o -1 si hubo un error.
//Si se cerró la conexión de parte del cliente devuelve un 0.
int socket_receive(socket_t* self, size_t bytes, char* buffer);

#endif