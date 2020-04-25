#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket.h"
#include "filehandler.h"

typedef struct{
    socket_t socket;
    filehandler_t filehandler;
    uint32_t msg_number;
}client_t;

//
void client_create(client_t* client);

void client_destroy(client_t* client);

// El cliente se conecta al host y servicio indicados
// PRE: el cliente fue creado
// POST: devuelve 0 si se pudo conectar correctamente, 1 en caso de error
int client_connect(client_t* client, const char*  host, const char* service);

// El cliente comienza a operar, enviando las instrucciones del
// archivo de entrada, esperando el OK del servidor e imprimiendo por pantalla el status
// PRE: el cliente fue creado y se lo conectó a un servidor. Si from_stdin es 1 lee de stdin.
// POST: devuelve 1 en caso de error.
int client_run(client_t* client, const char* filename, int from_stdin);

//Envía al servidor conectado el mensaje recibido por parámetro.
int client_encode_and_send(client_t* self, size_t msg_number);