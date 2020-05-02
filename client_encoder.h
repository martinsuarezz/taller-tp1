#ifndef CLIENT_ENCODER_H
#define CLIENT_ENCODER_H

#include "client_filehandler.h"
#include "common_dinamicvector.h"

typedef struct{
    dinamicvector_t* line;
    dinamicvector_t* header;
    size_t message_length;
} encoder_t;

//Inicializa el encoder
void encoder_create(encoder_t* self);

//Encodea una línea de comando del archivo manejado por el filehandler
//siguiendo el protocolo D-BUS y devuelve dicho mensaje.
//PRE: el encoder fue inicializado.
char* encoder_encode_line(encoder_t* self, filehandler_t* fileh, size_t msg_n);

//Devuelve el largo del mensaje que encodeó.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_message_length(encoder_t* self);

//Se destruye el encoder liberando sus recursos.
//PRE: el enocder fue inicializado.
void encoder_destroy(encoder_t* self);

#endif
