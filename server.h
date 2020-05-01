#include "common_socket.h"

typedef struct{
    socket_t socket;
}server_t;

// Inicializa el servidor
void server_create(server_t* self);

// El servidor comienza a escuchar solicitudes del puerto indicado.
void server_listen(server_t* self, const char* service);

// El servidor recibe los mensajes y los decodifica.
void server_run(server_t* self);

// El servidor se destruye liberando el socket utilizado.
void server_destroy(server_t* self);