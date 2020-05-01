#include "common_socket.h"

typedef struct{
    socket_t socket;
}server_t;

void server_create(server_t* self);

void server_listen(server_t* self, const char* service);

void server_run(server_t* self);

int server_read(server_t* self, char* buffer, size_t readsize);

void server_destroy(server_t* self);