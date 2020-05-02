#include "common_dinamicvector.h"

enum ids{ID, PATH, DESTINATION, INTERFACE, METHOD, FIRM};
 
typedef struct{
    dinamicvector_t parameters[6];
    size_t body_length;
} message_t;

// Inicializa el mensaje.
void message_create(message_t* self);

// Destruye el mensaje liberando sus recursos.
void message_destroy(message_t* self);

// Imprime el mensaje.
void message_print(message_t* self);

// Se le agrega un parametro al mensaje junto con su contenido.
void message_add_parameter(message_t* self, int id, char* param, size_t len);

// Setea el largo que tendrá el cuerpo del mensaje.
void message_set_body_length(message_t* self, size_t length);

// Obtiene el largo del cuerpo del mensaje
size_t message_get_body_length(message_t* self);

// Devuelve 1 si el mensaje tiene cuerpo, 0 en caso contrario
int message_has_body(message_t* self);
