#include "common_socket.h"

typedef struct{
    socket_t socket;
}server_t;

// Inicializa el servidor
void server_create(server_t* self);

// El servidor comienza a escuchar solicitudes del puerto indicado.
// Devuelve 0 si ejecuta correctamente, -1 en caso de error.
int server_listen(server_t* self, const char* service);

// El servidor recibe los mensajes, los decodifica  y envía una
// confirmación al cliente.
// Devuelve 0 si ejecuta correctamente, -1 en caso de error.
int server_run(server_t* self);

// Envía OK al cliente.
// Devuelve 0 si ejecuta correctamente, -1 en caso de error.
int server_send_response(server_t* self, socket_t* socket);

// El servidor se destruye liberando el socket utilizado.
void server_destroy(server_t* self);
