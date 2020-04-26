#ifndef ENCODER_H
#define ENCODER_H

#include "filehandler.h"
#include "dinamicvector.h"

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
char* encoder_encode_line(encoder_t* self, filehandler_t* filehandler, size_t msg_number);

//Devuelve el largo del mensaje que encodeó.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_message_length(encoder_t* self);

//Se destruye el encoder liberando sus recursos.
//PRE: el enocder fue inicializado.
void encoder_destroy(encoder_t* self);

//El encoder devuelve la cantidad de argumentos que tiene el método de la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_get_ammount_of_arguments(encoder_t* self);

//El encoder devuelve la cantidad de parametros que tiene la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_get_ammount_of_parameters(encoder_t* self);

//El encoder devuelve el largo de cierto parametro en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_get_parameter_length(encoder_t* self, size_t param_number);

//Devuelve la posición donde comienza cierto parametro en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_get_parameter_index(encoder_t* self, size_t parameter);

//El encoder devuelve el largo de cierto argumento del método en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
size_t encoder_get_argument_length(encoder_t* self, size_t argument_number);

//El encoder encodea un parametro.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
void encoder_encode_parameter(encoder_t* self, size_t parameter);

//El encoder llena el mensaje con un padding de '\0' hasta un multiplo
//del número recibido por parámetro.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
void encoder_add_padding(encoder_t* self, size_t multiple_of);

#endif