#include <stdio.h>
#include <string.h>
#include "server.h"

int main(int argc, char* argv[]) {
    server_t server;
    server_create(&server);
    if (server_listen(&server, argv[1]) == -1){
        fprintf(stderr, "Error al establecer conexión\n");
        return 1;
    }
    if (server_run(&server) == -1){
        fprintf(stderr, "Error en la comunicación con el cliente");
        return 2;
    }
    return 0;
}
