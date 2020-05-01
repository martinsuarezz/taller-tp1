#include "common_socket.h"
#include "server_message.h"

typedef struct{
    socket_t* socket;
    message_t message;
}decoder_t; 

// Inicializa el decoder
void decoder_create(decoder_t* self);

// Se le agrega un socket donde recibir mensajes.
// PRE: el decoder debe estar inicializado, el socket
// debe ser un socket aceptado para comunicación.
void decoder_add_socket(decoder_t* self, socket_t* accepted_socket);

// Lee un mensaje proveniente del socket de comunicación.
// Devuelve 1 en caso de que se haya leido correctamente un mensaje.
// Devuelve 0 si ocurrió un error o se cerró el canal de comunicación.
// PRE: el decodificador fue inicializado y se le agregó un
// socket válido para recibir mensajes.
int decoder_decode(decoder_t* self);

// Imprime por salida estándar el último mensaje decodificado.
void decoder_print_message(decoder_t* self);

// Destruye el decoder.
void decoder_destroy(decoder_t* self);

void decoder_reset_message(decoder_t* self);