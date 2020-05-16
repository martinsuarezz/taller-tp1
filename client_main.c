#include <stdio.h>
#include <string.h>
#include "client.h"

int main(int argc, char* argv[]) {
    if (!(argc == 3 || argc == 4))
        return 1;
    client_t client;
    client_create(&client);
    if (client_connect(&client, argv[1], argv[2]) == -1){
        fprintf(stderr, "Error al conectar con el servidor\n");
        client_destroy(&client);
        return 1;
    }
    if (client_run(&client, argv[3], 4-argc) == -1){
        fprintf(stderr, "Error al enviar información al servidor\n");
        client_destroy(&client);
        return 2;
    }
    client_destroy(&client);
    return 0;
}
