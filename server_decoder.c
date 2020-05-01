#include "server_decoder.h"


static size_t round_up(size_t number, size_t round_to){
    return ((number + 7) & (-round_to));
}

static size_t get_paramter_length(char* number){
    uint32_t num;
    memcpy(&num, number, 4);
    return round_up((size_t) num + 1, 8);
}

static size_t get_body_length(char* number){
    uint32_t num;
    memcpy(&num, number, 4);
    return num;
}

void decoder_create(decoder_t* self){
    message_t message;
    message_create(&message);
    self->message = message;
}

void decoder_add_socket(decoder_t* self, socket_t* accepted_socket){
    self->socket = accepted_socket;
}

int decoder_decode_top_bytes(decoder_t* self){
    socket_t* socket = self->socket;
    message_t* message = &(self->message);
    char buffer[16];
    if(socket_receive(socket, 16, buffer) <= 0)
        return -1;
    message_add_parameter(message, ID, buffer + 8, 4);
    message_set_body_length(message, get_body_length(buffer + 4));
    return 0;
}

static int get_parameter_id(char number){
    switch (number){
        case 1:
            return PATH;
        case 6:
            return DESTINATION;
        case 2:
            return INTERFACE;
        case 3:
            return METHOD;
        case 9:
            return FIRM;
    }
    return -1;
}

static void decoder_decode_parameter(decoder_t* self){
    char buffer[8];
    socket_receive(self->socket, 8, buffer);
    dinamicvector_t vector;
    dinamicvector_create(&vector, 16);
    size_t param_length = get_paramter_length(buffer + 4);
    char character[1];
    for (size_t i = 0; i < param_length; i++){
        socket_receive(self->socket, 1, character);
        dinamicvector_add(&vector, (const char*) character, 1);
    }
    message_add_parameter(&(self->message), get_parameter_id(buffer[0]), dinamicvector_get_array(&vector), dinamicvector_get_length(&vector));
    dinamicvector_destroy(&vector);
}

static void decoder_decode_body(decoder_t* self){
    size_t received_bytes = 0;
    char buffer[4];
    char character[1];
    dinamicvector_t vector;
    dinamicvector_create(&vector, 16);
    while (received_bytes < message_get_body_length(&(self->message))){
        received_bytes += socket_receive(self->socket, 4, buffer);
        character[0] = 1;
        while (character[0] != '\0'){
            socket_receive(self->socket, 1, character);
            dinamicvector_add(&vector, (const char*) character, 1);
            received_bytes++;
        }
    }
    message_add_parameter(&(self->message), FIRM, dinamicvector_get_array(&vector), dinamicvector_get_length(&vector));
    dinamicvector_destroy(&vector);
}

size_t decoder_get_ammount_of_parameters(decoder_t* self){
    if (message_has_body(&(self->message)))
        return 4;
    return 3;
}

static void decoder_decode_firm(decoder_t* self){
    char buffer[5];
    socket_receive(self->socket, 5, buffer);
    size_t length = round_up(6 + buffer[4], 8);
    for (size_t i = 5; i < length; i++){
        socket_receive(self->socket, 1, buffer);
    }
}

int decoder_decode(decoder_t* self){
    if (decoder_decode_top_bytes(self) == -1)
        return 0;
    size_t ammount_param = decoder_get_ammount_of_parameters(self);
    for (size_t i = 0; i < ammount_param; i++){
        decoder_decode_parameter(self);
    }
    if (message_has_body(&(self->message))){
        decoder_decode_firm(self);
        decoder_decode_body(self);
    }
        
    return 1;
}

void decoder_print_message(decoder_t* self){
    message_print(&(self->message));
}


void decoder_destroy(decoder_t* self){
    
}

void decoder_reset_message(decoder_t* self){
    message_destroy(&(self->message));
    message_t message;
    message_create(&message);
    self->message = message;
}