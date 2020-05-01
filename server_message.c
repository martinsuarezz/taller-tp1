#include "server_message.h"

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
    printf("* Parámetros:\n    * ");
    size_t length = dinamicvector_get_length(self->parameters + 5);
    char* body = dinamicvector_get_array(self->parameters + 5);
    for (size_t i = 0; i < length; i++){
        if (body[i] == '\0'){
            printf("\n    * ");
            continue;
        }
        printf(body + i);
    }
}

int message_has_body(message_t* self){
    if (self->body_length > 0)
        return 1;
    return 0;
}

void message_print(message_t* self){
    const char* text[] = {"Id: ", "Destino: ", "Path: ", "Interfaz: ", "Método: "};
    for (size_t i = 0; i < 5; i++){
        printf("* ");
        printf(text[i]);
        printf(dinamicvector_get_array(&(self->parameters[i])));
        printf("\n");
    }
    if (message_has_body(self)){
        print_body(self);
        printf("\n");
    }
    printf("\n");
}

void message_add_parameter(message_t* self, int parameter_id, char* parameter, size_t length){
    dinamicvector_add(self->parameters + parameter_id, (const char*) parameter, length);
}

size_t message_get_body_length(message_t* self){
    return self->body_length;
}

void message_set_body_length(message_t* self, size_t length){
    self->body_length = length;
}