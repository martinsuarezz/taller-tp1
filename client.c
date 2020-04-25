#include <stdio.h>
#include <string.h>
#include "client.h"

void client_create(client_t* self){
    socket_t socket;
    socket_create(&socket);
    self->socket = socket;
}

void client_destroy(client_t* self){

}

void int32_to_array(char* array, uint32_t number){
    char* number_array = (char*) &number;
    for (int i = 0; i < 4; i++)
        array[i] = number_array[i];
}

size_t length_of_body(char* array){
    size_t index1, index2, length;
    for (index1 = 0; array[index1] != '('; index1++)
        continue;
    for (index2 = index1; array[index2] != ')'; index2++)
        continue;

    length = index2 - index1;
    if (length == 1)
        return 0;
    return length + 4;
}

size_t round_up(size_t number, size_t round_to){
    return ((number + 7) & (-round_to));
}

size_t get_ammount_of_arguments(char* array){
    size_t ammount = 0;
    size_t index = 0;

    while(array[index]!= '(')
        index++;

    while (array[index] != ')'){
        if (array[index] == ',')
            ammount++;
        index++;
    }

    if (ammount == 0)
        return ammount;
    return ammount + 1;
}

size_t get_ammount_of_parameters(char* array){
    if (get_ammount_of_arguments(array) > 0)
        return 5;
    return 4;
}

size_t get_parameter_length(char* array, size_t param_number){
    if (param_number == 4)
        return 2 + get_ammount_of_arguments(array);
        
    size_t param = 0;
    size_t index1 = 0;
    size_t index2;
    while (param < param_number){
        if (array[index1] == ' ')
            param++;
        index1++;
    }
    index2 = index1;
    while (array[index1] != ' ' && array[index1] != '\0')
        index1++;
    return index1 - index2;
}

size_t get_parameter_index(char* array, size_t parameter){
    size_t index = 0;
    size_t current_param = 0;
    while (current_param < parameter){
        if (array[index] == ' ')
            current_param++;
        index++;
    }
    return index;
}

size_t get_array_size(char* array){
    size_t parameters = get_ammount_of_parameters(array);
    size_t size = 8 * parameters;
    for (size_t i = 0; i < parameters - 1; i++)
        size = size + round_up(get_parameter_length(array, i), 8);
    
    return size + get_parameter_length(array, parameters - 1); //El ultimo parametro no se cuenta el padding
}

void encode_parameter(dinamicvector_t* header, dinamicvector_t* line, size_t parameter){
    char parameter_list[5] = {1, 6, 2, 3, 9};
    char data_type_list[5] = {'o', 's', 's', 's', 'g'};
    dinamicvector_add(header, parameter_list + parameter, 1);
    dinamicvector_add(header, parameter_list, 1);
    dinamicvector_add(header, data_type_list + parameter, 1);
    dinamicvector_add(header, "\0", 1);
    size_t copy_length;
    if (parameter != 4){
        char buffer[4];
        size_t parameter_length = get_parameter_length(dinamicvector_get_array(line), parameter);
        int32_to_array(buffer, (uint32_t) parameter_length);
        dinamicvector_add(header, (const char*) buffer, 4);
        dinamicvector_add(header, (const char*) dinamicvector_get_array(line) + get_parameter_index(dinamicvector_get_array(line), parameter), parameter_length);
        copy_length = parameter_length;   
    }
    else{
        char ammount_arguments_char[2];
        char ammount_arguments = (char) get_ammount_of_arguments(dinamicvector_get_array(line));
        sprintf(ammount_arguments_char, "%c", ammount_arguments);
        dinamicvector_add(header, (const char*) ammount_arguments_char, 1);
        for (size_t i = 0; i < ammount_arguments; i++)
            dinamicvector_add(header, "s", 1);
        copy_length = ammount_arguments;
    }
    for (size_t i = 0; i < round_up(copy_length, 8) - copy_length; i++)
        dinamicvector_add(header, "/0", 1);
}

int client_encode_and_send(client_t* self, int msg_number){
    dinamicvector_t vector, header;
    dinamicvector_create(&vector, 0);
    filehandler_readline(&(self->filehandler), &vector);
    size_t array_size = get_array_size(dinamicvector_get_array(&vector));
    dinamicvector_create(&header, array_size);
    char buffer[16] = {0x6c, 0x01, 0x00, 0x01};
    int32_to_array(buffer + 4, (uint32_t) length_of_body((dinamicvector_get_array(&vector))));
    int32_to_array(buffer + 8, (uint32_t) msg_number);
    int32_to_array(buffer + 12, (uint32_t) array_size);
    dinamicvector_add(&header, (const char*) buffer, 16);
    for (size_t i = 0; i < get_ammount_of_parameters(dinamicvector_get_array(&vector)); i++)
        encode_parameter(&header, &vector, i);
    socket_send(&(self->socket), dinamicvector_get_length(&header), (const char*) dinamicvector_get_array(&header));
    return 0;
}

int client_connect(client_t* self, const char*  host, const char* service){
    return socket_connect(&(self->socket), host, service);
}

int client_run(client_t* self, const char* filename, int from_stdin){
    filehandler_t filehandler;
    if (from_stdin)
        filehandler_create(&filehandler, NULL, 32);
    else
        filehandler_create(&filehandler, filename, 32);
    
    self->filehandler = filehandler;
    /*char buffer[32];
    while(filehandler_read(&(self->filehandler), buffer)){
        socket_send(&(self->socket), 32, (const char*) buffer);
    }*/
    client_encode_and_send(self, 1);
    return 0;
}