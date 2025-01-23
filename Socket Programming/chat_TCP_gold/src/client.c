#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080
#define MESSAGE_LEN 64

struct Message {
    char username[32];
    char timestamp[32];
    char message[MESSAGE_LEN];
};

void get_timestamp(char *buffer, size_t len) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, len, "%Y-%m-%d %H:%M:%S", t);
}

void *receive_messages(void *sock) {
    int server_socket = *(int *)sock;
    struct Message msg;

    while (1) {
        int read_size = recv(server_socket, &msg, sizeof(msg), 0);
        if (read_size > 0) {
            printf("[%s][%s]: %s\n", msg.timestamp, msg.username, msg.message);
        } else if (read_size == 0) {
            printf("Server disconnected.\n");
            exit(0);
        }
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    struct Message msg;

    printf("Enter your username: ");
    fgets(msg.username, sizeof(msg.username), stdin);
    msg.username[strcspn(msg.username, "\n")] = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type your messages below:\n");

    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock) != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("> ");
        fgets(msg.message, sizeof(msg.message), stdin);
        msg.message[strcspn(msg.message, "\n")] = 0;

        if (strcmp(msg.message, "exit") == 0) {
            printf("Exiting chat...\n");
            break;
        }

        get_timestamp(msg.timestamp, sizeof(msg.timestamp));

        if (send(sock, &msg, sizeof(msg), 0) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sock);
    return 0;
}
