#include "server_message.h"
#include <stdint.h>

void message_create(message_t* self){
    dinamicvector_t id, path, destination, interface, method, arguments;
    dinamicvector_create(&id, 4);
    dinamicvector_create(&path, 16);
    dinamicvector_create(&destination, 16);
    dinamicvector_create(&interface, 16);
    dinamicvector_create(&method, 16);
    dinamicvector_create(&arguments, 16);
    self->parameters[0] = id;
    self->parameters[1] = path;
    self->parameters[2] = destination;
    self->parameters[3] = interface;
    self->parameters[4] = method;
    self->parameters[5] = arguments;
    self->body_length = 0;
}

void message_destroy(message_t* self){
    for (int i = 0; i < 6; i++)
        dinamicvector_destroy(self->parameters + i);
}

static void print_body(message_t* self){
    printf("* Parametros:\n    * ");
    size_t length = dinamicvector_get_length(self->parameters + 5);
    char* body = dinamicvector_get_array(self->parameters + 5);
    for (size_t i = 0; i < length - 1; i++){
        if (body[i] == '\0'){
            printf("\n    * ");
            continue;
        }
        printf("%c", body[i]);
    }
}

int message_has_body(message_t* self){
    if (self->body_length > 0)
        return 1;
    return 0;
}

static uint32_t get_id_number(char* number){
    uint32_t num;
    memcpy(&num, number, 4);
    return num;
}

void print_id(message_t* self){
    char* array = dinamicvector_get_array(&(self->parameters[0]));
    uint32_t id = get_id_number(array);
    printf("* ");
    printf("Id: ");
    printf("%#010x\n", id);
}

void message_print(message_t* self){
    const char* text[] = {"Destino: ", "Ruta: ", "Interfaz: ", "Metodo: "};
    print_id(self);
    for (size_t i = 0; i < 4; i++){
        printf("* ");
        printf(text[i]);
        printf(dinamicvector_get_array(&(self->parameters[i+1])));
        printf("\n");
    }
    if (message_has_body(self)){
        print_body(self);
        printf("\n");
    }
    printf("\n");
}

void message_add_parameter(message_t* self, int id, char* param, size_t len){
    dinamicvector_add(self->parameters + id, (const char*) param, len);
}

size_t message_get_body_length(message_t* self){
    return self->body_length;
}

void message_set_body_length(message_t* self, size_t length){
    self->body_length = length;
}
