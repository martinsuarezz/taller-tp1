#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include "client_encoder.h"

//Redondea el number al múltiplo más cercano de round_to.
static size_t round_up(size_t number, size_t round_to){
    return ((number + 7) & (-round_to));
}

// Convierte el numero del endianess local
// a little endian (protocolo DBUS)
static uint32_t host_to_dbus(uint32_t number){
    return htole32(number);
}

//Copia el numero recibido por parametro al array.
static void int32_to_array(char* array, uint32_t number){
    number = host_to_dbus(number);
    char* number_array = (char*) &number;
    for (int i = 0; i < 4; i++)
        array[i] = number_array[i];
}

//El encoder devuelve la cantidad de argumentos que tiene 
//el método de la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static size_t encoder_get_ammount_of_arguments(encoder_t* self){
    size_t ammount = 1;
    size_t index = 0;
    char* line = dinamicvector_get_array((self->line));

    while (line[index]!= '(')
        index++;

    if (line[index+1] == ')')
        return 0;

    while (line[index] != ')'){
        if (line[index] == ',')
            ammount++;
        index++;
    }
    return ammount;
}

//El encoder devuelve la cantidad de parametros que tiene la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static size_t encoder_get_ammount_of_parameters(encoder_t* self){
    if (encoder_get_ammount_of_arguments(self) > 0)
        return 5;
    return 4;
}

//El encoder devuelve el largo de cierto parametro en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static size_t encoder_get_parameter_length(encoder_t* self, size_t param_n){
    char* line = dinamicvector_get_array((self->line));
    if (param_n == 4)
        return 2 + encoder_get_ammount_of_arguments(self);
        
    size_t param = 0;
    size_t index1 = 0;
    size_t index2;
    while (param < param_n){
        if (line[index1] == ' ')
            param++;
        index1++;
    }
    index2 = index1;
    while (line[index1] != ' ' && line[index1] != '\0' && line[index1] != '(')
        index1++;
    return index1 - index2;
}

//Devuelve la posición donde comienza cierto parametro en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static size_t encoder_get_parameter_index(encoder_t* self, size_t parameter){
    size_t index = 0;
    size_t current_param = 0;
    char* line = dinamicvector_get_array((self->line));
    while (current_param < parameter){
        if (line[index] == ' ')
            current_param++;
        index++;
    }
    return index;
}

//El encoder devuelve el largo de cierto argumento del 
//método en la línea de comando.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static size_t encoder_get_argument_length(encoder_t* self, size_t argument_n){
    char* line = dinamicvector_get_array((self->line));
    size_t argument = 0;
    size_t index1 = 0;
    size_t index2;
    
    while (line[index1] != '(')
        index1++;
    index1++;

    while (argument < argument_n){
        if (line[index1] == ',')
            argument++;
        index1++;
    }
    index2 = index1;
    while (line[index1] != ',' && line[index1] != '\0' && line[index1] != ')')
        index1++;
    return index1 - index2;
}

//El encoder llena el mensaje con un padding de '\0' hasta un multiplo
//del número recibido por parámetro.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static void encoder_add_padding(encoder_t* self, size_t multiple_of){
    size_t length = dinamicvector_get_length(self->header);
    for (size_t i = 0; i < round_up(length, multiple_of) - length; i++)
        dinamicvector_add(self->header, "\0", 1);
}

//El encoder encodea un parametro.
//PRE: el encoder fue inicializado y se llamó a encoder_encode_line().
static void encoder_encode_parameter(encoder_t* self, size_t parameter){
    dinamicvector_t* header = (self->header);
    dinamicvector_t* line = (self->line);
    char param_list[5][4] = {"\x1\x1\x6F\0", "\x6\x1\x73\0", "\x2\x1\x73\0", 
                            "\x3\x1\x73\0", "\x9\x01\x67\0"};
    dinamicvector_add(header, param_list[parameter], 4);
    if (parameter != 4){
        char buffer[4];
        size_t parameter_length = encoder_get_parameter_length(self, parameter);
        int32_to_array(buffer, (uint32_t) parameter_length);
        dinamicvector_add(header, (const char*) buffer, 4);
        const char* array = (const char*) dinamicvector_get_array(line);
        size_t index = encoder_get_parameter_index(self, parameter);
        dinamicvector_add(header, array + index, parameter_length); 
    } else{
        char ammount_arg_c[2];
        char ammount_arg = (char) encoder_get_ammount_of_arguments(self);
        snprintf(ammount_arg_c, sizeof(ammount_arg_c), "%c", ammount_arg);
        dinamicvector_add(header, (const char*) ammount_arg_c, 1);
        for (size_t i = 0; i < ammount_arg; i++)
            dinamicvector_add(header, "s", 1);
    }
    dinamicvector_add(header, "\0", 1);
    encoder_add_padding(self, 8);
}

static size_t encoder_get_length_of_body(encoder_t* self){
    size_t arguments = encoder_get_ammount_of_arguments(self);
    size_t length = 0;
    for (size_t i = 0; i < arguments; i++)
        length = length + encoder_get_argument_length(self, i) + 5;
    return length;
}

static size_t encoder_get_array_size(encoder_t* self){
    size_t parameters = encoder_get_ammount_of_parameters(self);
    size_t size = 8 * parameters;
    for (size_t i = 0; i < parameters - 1; i++)
        size = size + round_up(encoder_get_parameter_length(self, i), 8);
    
    return size + encoder_get_parameter_length(self, parameters - 1);
}

static size_t encoder_get_argument_index(encoder_t* self, size_t argument){
    size_t index = 0;
    size_t current_argument = 0;
    char* line = dinamicvector_get_array((self->line));

    while (line[index] != '(')
        index++;

    index++;

    while (current_argument < argument){
        if (line[index] == ',')
            current_argument++;
        index++;
    }
    return index;
}

static void encoder_encode_argument(encoder_t* self, size_t argument){
    char buffer[4];
    size_t argument_length = encoder_get_argument_length(self, argument);
    int32_to_array(buffer, (uint32_t) argument_length);
    dinamicvector_add(self->header, (const char*) buffer, 4);
    const char* array = (const char*) dinamicvector_get_array(self->line);
    size_t index = encoder_get_argument_index(self, argument);
    dinamicvector_add(self->header, array + index, argument_length);
    dinamicvector_add(self->header, "\0", 1);
}

static void encoder_encode_body(encoder_t* self){
    if (encoder_get_ammount_of_parameters(self) == 4)
        return;
    for (size_t i = 0; i < encoder_get_ammount_of_arguments(self); i++)
        encoder_encode_argument(self, i);
}

void encoder_create(encoder_t* self){
    self->message_length = 0;
}

char* encoder_encode_line(encoder_t* self, filehandler_t* fileh, size_t msg_n){
    dinamicvector_t readline, header;
    dinamicvector_create(&readline, 32);
    self->line = &readline;
    if (!filehandler_readline(fileh, &readline)){
        dinamicvector_destroy(self->line);
        return NULL;
    }
    self->header = &header;
    size_t array_size = encoder_get_array_size(self);
    dinamicvector_create(&header, array_size);
    char buffer[16] = {0x6c, 0x01, 0x00, 0x01};
    int32_to_array(buffer + 4, (uint32_t) encoder_get_length_of_body(self));
    int32_to_array(buffer + 8, (uint32_t) msg_n);
    int32_to_array(buffer + 12, (uint32_t) array_size);
    dinamicvector_add(&header, (const char*) buffer, 16);
    for (size_t i = 0; i < encoder_get_ammount_of_parameters(self); i++)
        encoder_encode_parameter(self, i);
    encoder_encode_body(self);
    self->message_length = dinamicvector_get_length(self->header);
    dinamicvector_destroy(self->line);
    return dinamicvector_get_array(self->header);
}

size_t encoder_message_length(encoder_t* self){
    return self->message_length;
}

void encoder_destroy(encoder_t* self){
}
