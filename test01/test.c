#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    printf("waiting for connection . . .\n");
    listen(server_socket, 5);
    //printf("socket has been queried\npending accept\n");

    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    //    if(client_socket =! -1) {
    //        printf("invalid client socket???");
    //        return 0;
    //    }
    //printf("client connection estabilished! sending a message!\n");
    char server_message[256] = "Hello from the server!";
    send(client_socket, server_message, sizeof(server_message), 0);

    close(server_socket);
    return 0;
}
