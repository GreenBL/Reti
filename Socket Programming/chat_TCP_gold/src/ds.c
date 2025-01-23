#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MESSAGE_LEN 64

struct Message {
    char username[32];
    char timestamp[32];
    char message[MESSAGE_LEN];
};

int clients[MAX_CLIENTS] = {0};

void broadcast_message(struct Message *msg, int sender_socket) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != 0) {
            send(clients[i], msg, sizeof(*msg), 0);
        }
    }
}

void handle_client(int client_socket) {
    struct Message msg;
    while (1) {
        int read_size = recv(client_socket, &msg, sizeof(msg), 0);
        if (read_size <= 0) {
            printf("Client disconnected.\n");
            close(client_socket);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i] == client_socket) {
                    clients[i] = 0;
                    break;
                }
            }
            break;
        }
        printf("[%s][%s]: %s\n", msg.timestamp, msg.username, msg.message);
        broadcast_message(&msg, client_socket);
    }
}

int main() {
    int server_socket, client_socket, addr_len;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected.\n");
        for (int i = 0; i < MAX_CLIENTS; i++) {
            puts("");
            fprintf(stdout, "%d", clients[i]);
            puts("");
            fflush(stdout);
            if (clients[i] == 0) {
                clients[i] = client_socket;
                break;
            }
        }

        if (fork() == 0) {
            handle_client(client_socket);
            exit(0);
        }
    }

    close(server_socket);
    return 0;
}
