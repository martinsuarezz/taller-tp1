#include "common_dinamicvector.h"

enum ids{ID, PATH, DESTINATION, INTERFACE, METHOD, FIRM};
 
typedef struct{
    dinamicvector_t parameters[6];
    size_t body_length;
} message_t;

void message_create(message_t* self);

void message_destroy(message_t* self);

void message_print(message_t* self);

void message_add_parameter(message_t* self, int parameter_id, char* parameter, size_t length);

void message_set_body_length(message_t* self, size_t length);

size_t message_get_body_length(message_t* self);

int message_has_body(message_t* self);