#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common_socket.h"

void socket_create(socket_t* self){
    self->socket = -1;
}

void socket_destroy(socket_t* self){
    shutdown(self->socket, SHUT_RDWR);
    close(self->socket);
}

int socket_connect(socket_t* self, const char* host, const char* service){
    struct addrinfo hints;
    int error, s;
    struct addrinfo *results, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    if ((error = getaddrinfo(host, service, &hints, &results)) != 0)
        return error;
    
    for (rp = results; rp != NULL; rp = rp->ai_next) {
        s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (s == -1)
            continue;

        if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1){
            self->socket = s;
            freeaddrinfo(results);
            return 0;
        }
    }
    return 1;
}

int socket_send(socket_t* self, size_t bytes, const char* msg){
    ssize_t status;
    ssize_t sent = 0;
    while (sent < (ssize_t) bytes){
        status = send(self->socket, (const void*) msg, bytes, 0);
        if (status <= 0)
            return -1;
        sent += status;
    }
        
    return (int)sent;
}
     

int socket_bind_and_listen(socket_t* self, const char* service){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0) 
        return -1;
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        int val = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0 && listen(sfd, 1) == 0)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL)
        return -1;

    freeaddrinfo(result);
    self->socket = sfd;
    return 0;
}

int socket_accept(socket_t* self, socket_t* accepted_socket){
    if ((accepted_socket->socket = accept(self->socket, NULL, NULL)) == -1)
        return -1;
    return 0;
}

int socket_receive(socket_t* self, size_t bytes, char* buffer){
    ssize_t status;
    ssize_t received = 0;
    while (received < (ssize_t) bytes){
        status = recv(self->socket, buffer + received, bytes - received, 0);
        if (status <= 0)
            return 0;
        received += status;
    }
        
    return (int)received;
}
