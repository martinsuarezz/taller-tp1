#include <stdio.h>
#include <string.h>
#include "server.h"

int main(int argc, char* argv[]) {
    server_t server;
    server_create(&server);
    server_listen(&server, argv[1]);
    server_run(&server);
    return 0;
}
