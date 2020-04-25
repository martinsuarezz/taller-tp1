#include <stdio.h>
#include <string.h>
#include "client.h"

int main(int argc, char* argv[]) {
    client_t client;
    client_create(&client);
    client_connect(&client, argv[1], argv[2]);
    client_run(&client, argv[3], 4-argc);
    return 0;
}