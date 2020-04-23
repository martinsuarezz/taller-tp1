#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"

void socket_create(socket_t* self){
    //
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
            return 0;
        }
    }
    return 1;
}

int socket_send(socket_t* self, size_t bytes, const char* msg){
    send(self->socket, (const void*) msg, bytes, 0);
    return 0;
}